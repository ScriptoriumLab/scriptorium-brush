#include "modian/ui/candidate_window.h"
#include <windows.h>

// 窗口类名
static const wchar_t* kCandidateWindowClassName = L"CandidateWindowClass";

// 实现默认的绘制器
void win32_candidate_renderer::render(HWND hwnd, const std::vector<std::wstring>& candidates) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    // 简单绘制，每行显示一个候选词
    int y = 10;
    for (const auto& candidate : candidates) {
        TextOutW(hdc, 10, y, candidate.c_str(), static_cast<int>(candidate.size()));
        y += 20;
    }
    EndPaint(hwnd, &ps);
}

candidate_window::candidate_window(HINSTANCE h_instance, const std::shared_ptr<candidate_renderer>& renderer)
    : h_instance_(h_instance), hwnd_(nullptr), listbox_(nullptr), renderer_(renderer) {
}

candidate_window::~candidate_window() {
    if (hwnd_) {
        DestroyWindow(hwnd_);
    }
}

bool candidate_window::create() {
    // 注册窗口类
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = h_instance_;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = kCandidateWindowClassName;
    RegisterClassEx(&wc);

    // 创建窗口（无边框、简单样式）
    hwnd_ = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                              kCandidateWindowClassName,
                              L"Candidate Window",
                              WS_POPUP,
                              100, 100, 300, 200,
                              nullptr, nullptr, h_instance_, this);

    listbox_ = CreateWindowW(
        L"LISTBOX", L"",
        WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
        0, 0, 400, 150,
        hwnd_, nullptr,
        GetModuleHandle(nullptr), nullptr
    );

    HFONT hFont = CreateFontW(
        20, 0, 0, 0, FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"微软雅黑"  // 使用中文字体
    );
    SendMessageW(listbox_, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

    return hwnd_ != nullptr;
}

void candidate_window::show() const {
    if (hwnd_) {
        ShowWindow(hwnd_, SW_SHOW);
        UpdateWindow(hwnd_);
    }
}

void candidate_window::hide() const {
    if (hwnd_) {
        ShowWindow(hwnd_, SW_HIDE);
    }
}

void candidate_window::update_candidates(const std::vector<std::wstring>& candidates) {
    candidates_ = candidates;
    if (hwnd_) {
        InvalidateRect(hwnd_, nullptr, TRUE);
        UpdateWindow(hwnd_);
    }
     SendMessage(listbox_, LB_RESETCONTENT, 0, 0);
     for (const auto& str : candidates_) {
         SendMessage(listbox_, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.c_str()));
     }
     // 自动调整窗口大小
     int itemHeight = SendMessage(listbox_, LB_GETITEMHEIGHT, 0, 0);
     RECT rc = {0, 0, 400, min(10, static_cast<int>(candidates_.size())) * itemHeight + 4};
     AdjustWindowRectEx(&rc,
                 static_cast<DWORD>(GetWindowLongW(hwnd_, GWL_STYLE)),
                 FALSE,
                 static_cast<DWORD>(GetWindowLongW(hwnd_, GWL_EXSTYLE)));
     SetWindowPos(hwnd_, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                 SWP_NOZORDER | SWP_NOMOVE);
}

LRESULT CALLBACK candidate_window::wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    candidate_window* pThis = nullptr;
    if (message == WM_NCCREATE) {
        // 从创建数据中获得 this 指针
        LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = reinterpret_cast<candidate_window*>(pcs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hwnd_ = hwnd;
    } else {
        pThis = reinterpret_cast<candidate_window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handle_message(message, wParam, lParam);
    } else {
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

LRESULT candidate_window::handle_message(UINT message, WPARAM wParam, LPARAM lParam) const {
    switch (message) {
        case WM_PAINT:
            if (renderer_) {
                renderer_->render(hwnd_, candidates_);
            }
            return 0;
        default:
            return DefWindowProc(hwnd_, message, wParam, lParam);
    }
}
