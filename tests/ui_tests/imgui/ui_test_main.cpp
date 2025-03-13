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
    ImGui::SetNextWindowSize(ImVec2(400, 80), ImGuiCond_FirstUseEver);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));

    ImGui::Begin("候选词", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoDecoration);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));

    ImGui::PushFont(font);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));

    // 获取当前样式
    ImGuiStyle& style = ImGui::GetStyle();

    // 保存原始颜色
    ImVec4 original_button_color = style.Colors[ImGuiCol_Button];

    // 设置按钮颜色为透明
    style.Colors[ImGuiCol_Button].w = 0.0f;        // 正常状态

    // 渲染候选词
    for (size_t i = 0; i < candidates.size(); i++) {
        if (i > 0) ImGui::SameLine();

        if (ImGui::Button(candidates[i].c_str())) {
            OnCandidateSelected(candidates[i]);
        }
    }

    // 恢复原始颜色
    style.Colors[ImGuiCol_Button] = original_button_color;

    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::PopStyleVar();
    ImGui::End();

    ImGui::PopStyleColor();
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
        800, 600,
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