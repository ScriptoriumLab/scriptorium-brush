#include "modian/ui/win32/candidate_window.h"
#include "modian/ui/win32/renderer/candidate_renderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    modian::infra::ui::win32::candidate_window wnd{hInstance, std::make_shared<modian::infra::ui::win32::renderer::win32_candidate_renderer>()};

    // 测试候选词
    std::vector<std::wstring> candidates = {
        L"1. 测试", L"2. 示例", L"3. 候选词",
        L"4. 输入法", L"5. Windows", L"6. Modian IME",
        L"7. 你好墨点"
    };
    wnd.create();
    wnd.update_candidates(candidates);
    wnd.show();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
