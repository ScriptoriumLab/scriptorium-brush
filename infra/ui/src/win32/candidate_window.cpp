#include "modian/ui/win32/candidate_window.h"

namespace modian::infra::ui::win32 {
    // 窗口类名
    static constexpr auto CANDIDATE_WINDOW_CLASS_NAME = L"CandidateWindowClass";

    candidate_window::candidate_window(HINSTANCE h_instance, const std::shared_ptr<core::renderer::candidate_renderer>& renderer)
        : h_instance_(h_instance), hwnd_(nullptr), hdc_(nullptr), renderer_(renderer) {
    }

    candidate_window::~candidate_window() {
        if (hwnd_) {
            DestroyWindow(hwnd_);
            UnregisterClass(CANDIDATE_WINDOW_CLASS_NAME, h_instance_);
        }
    }

    bool candidate_window::create() {
        // 注册窗口类
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = wnd_proc;
        wc.hInstance = h_instance_;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = CANDIDATE_WINDOW_CLASS_NAME;
        RegisterClassEx(&wc);

        // 创建窗口（无边框、简单样式）
        hwnd_ = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                                  CANDIDATE_WINDOW_CLASS_NAME,
                                  L"Candidate Window",
                                  WS_POPUP,
                                  100, 100, 300, 200,
                                  nullptr, nullptr, h_instance_, this);

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

    LRESULT candidate_window::handle_message(UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_PAINT:
                if (renderer_) {
                    PAINTSTRUCT ps;
                    hdc_ = BeginPaint(hwnd_, &ps);
                    renderer_->begin_frame(hdc_);
                    renderer_->draw_list(candidates_);
                    renderer_->end_frame();
                    EndPaint(hwnd_, &ps);
                }
                return 0;
            default:
                return DefWindowProc(hwnd_, message, wParam, lParam);
        }
    }
}
