#include <imgui.h>
#include <imgui_impl_opengl3_loader.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>

// 候选词被选中的回调
void OnCandidateSelected(const std::string& candidate) {
    std::cout << "候选词被选择：" << candidate << std::endl;
}

// 候选词窗口渲染函数
void RenderCandidateWindow(const std::vector<std::string>& candidates, ImFont* font) {
    // 设置窗口位置和样式
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

    // 1. 设置固定窗口大小（宽度=候选词总宽度+边距，高度=字体高度+边距）
    const float window_width = candidates.size() * 100 + 20; // 假设每个候选词宽度100px
    const float window_height = 80;
    ImGui::SetNextWindowSize(ImVec2(window_width, window_height), ImGuiCond_Always);

    // 3. 设置窗口背景色为白色
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255, 255, 255, 255));

    ImGui::Begin("候选词", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoDecoration);

    ImGui::PushFont(font);

    // 4. 设置字体颜色为黑色
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

    // 2. 强制所有按钮在同一行
    ImGui::BeginGroup();
    for (size_t i = 0; i < candidates.size(); i++) {
        if (i > 0) ImGui::SameLine(0, 20); // 水平间距20像素

        // 设置透明按钮背景（保留hover效果）
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f,0.8f,0.8f,0.5f));

        if (ImGui::Button(candidates[i].c_str())) {
            OnCandidateSelected(candidates[i]);
        }

        // 如果不是最后一个候选词，则绘制竖线分隔符
        if (i != candidates.size() - 1) {
            // 获取当前按钮的矩形范围
            ImVec2 btn_min = ImGui::GetItemRectMin();
            ImVec2 btn_max = ImGui::GetItemRectMax();
            // 计算分隔线的位置，X 坐标位于按钮右侧偏右 4 像素
            float separator_x = btn_max.x + 4.0f;
            ImVec2 line_start = ImVec2(separator_x, btn_min.y);
            ImVec2 line_end   = ImVec2(separator_x, btn_max.y);
            // 设置浅灰色（例如RGB=200,200,200，Alpha=255）
            ImU32 line_color = IM_COL32(200, 200, 200, 255);
            float thickness = 1.0f;
            // 通过窗口的 DrawList 绘制分隔线
            ImGui::GetWindowDrawList()->AddLine(line_start, line_end, line_color, thickness);
            // 使用 SameLine() 将下一个按钮绘制在同一行
            ImGui::SameLine();
        }

        ImGui::PopStyleColor(2);
    }
    ImGui::EndGroup();

    // 恢复样式
    ImGui::PopStyleColor(); // 弹出字体颜色
    ImGui::PopFont();
    ImGui::End();
    ImGui::PopStyleColor(); // 弹出窗口背景色
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 配置 OpenGL 版本（这里设置为 OpenGL 3.0，适用于 Windows 和 macOS）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 2. 创建透明无边框窗口
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);      // 无边框
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1); // 透明背景
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);      // 禁止调整大小

    GLFWwindow* window = glfwCreateWindow(
        2000, 2000,
        "Input Method Candidate Window",
        nullptr, nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 3. 设置窗口位置和置顶
    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 启用垂直同步

    // 4. 初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.SizePixels = 40;
    config.OversampleH = config.OversampleV = 1;
    config.PixelSnapH = true;
    ImFont* largerFont = io.Fonts->AddFontDefault(&config);
    (void)io;

    // 初始化 GLFW + OpenGL3 后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 5. 配置输入法专用样式
    ImGui::StyleColorsDark(); // 使用暗色主题
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f; // 窗口圆角
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.95f); // 半透明背景

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // 开始新帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 生成测试候选词
        std::vector<std::string> candidates = {
            "candidate 1", "candidate 2", "candidate 3", "candidate 4",
            "candidate 5", "candidate 6", "candidate 7", "candidate 8",
            "candidate 9", "candidate 10", "candidate 11", "candidate 12",
        };

        // 渲染候选窗口
        RenderCandidateWindow(candidates, largerFont);

        // 渲染绘制
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 透明背景
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}