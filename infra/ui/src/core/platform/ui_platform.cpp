#include "modian/ui/core/platform/ui_platform.h"

#include "modian/core/logger/logger_service.h"

#include <imgui.h>
#include <imgui_impl_opengl3_loader.h>
#include <iostream>
#include <ostream>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <Windows.h>

namespace modian::infra::ui::core::platform {
	std::shared_ptr<ui_platform> ui_platform::instance() {
		static auto instance = std::make_shared<ui_platform>();
		return instance;
	}

	void ui_platform::start_ui_thread() {
		std::lock_guard lock(thread_mutex_);
		if (!is_thread_running_) {
			ui_thread_ = std::thread(&ui_platform::ui_thread_func, this);
			is_thread_running_ = true;
		}
	}

	void ui_platform::stop_ui_thread() {
		{
			std::lock_guard<std::mutex> lock(thread_mutex_);
			if (is_thread_running_) {
				exit_flag_ = true;
				is_thread_running_ = false;
				thread_condition_.notify_all();
			}
		}

		if (ui_thread_.joinable()) {
			ui_thread_.join();
		}
	}

	void ui_platform::on_candidate_update(const std::vector<std::wstring>& candidates) {
		modian::core::logger_service::logger()->info("Updating candidates to candidate ui");
		{
			std::lock_guard lock(candidate_queue_mutex_);
			candidates_queue_.push(candidates);

			for (const auto& candidate : candidates_queue_.front()) {
				modian::core::logger_service::logger()->info("Updating candidate: {}", candidate);
			}
		}
		thread_condition_.notify_all();
	}

	ui_platform::ui_platform() : is_thread_running_{false}, exit_flag_{false}, font{nullptr} {}

	ui_platform::~ui_platform() {
		stop_ui_thread();
	}

	// 添加中文转换函数
	std::string ui_platform::wstring_to_utf8(const std::wstring& wstr) {
		if (wstr.empty()) return {};

		int size_needed = WideCharToMultiByte(
			CP_UTF8, 0, &wstr[0], (int)wstr.size(),
			nullptr, 0, nullptr, nullptr);

		std::string str(size_needed, 0);
		WideCharToMultiByte(
			CP_UTF8, 0, &wstr[0], (int)wstr.size(),
			&str[0], size_needed, nullptr, nullptr);

		return str;
	}

	// 候选词被选中的回调
	void ui_platform::on_candidate_selected(const std::string& candidate) {
		modian::core::logger_service::logger()->info("Candidate selected: {}", candidate);
		if (!candidates_queue_.empty()) {
			candidates_queue_.pop();
		}
	}

	// 候选词窗口渲染函数
	inline void ui_platform::render_candidates() {
		// 设置窗口位置和样式
		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

		// 动态计算窗口尺寸
		const float vertical_padding = 20.0f;
		float total_buttons_width = 0.0f;
		float max_button_height = 0.0f;

		if (candidates_queue_.empty() || candidates_queue_.front().empty()) {
			return;
		}

		const auto candidates = candidates_queue_.front();

		ImGui::PushFont(font);
		for (const auto& cand : candidates) {
			const ImVec2 text_size = ImGui::CalcTextSize(wstring_to_utf8(cand).c_str());
			if (candidates.size() > 8) {
				total_buttons_width = text_size.x * 4 + 32.0f; // 文本宽度 + 按钮内边距
			} else {
				total_buttons_width = text_size.x * candidates.size() + 32.0f; // 文本宽度 + 按钮内边距
			}
			max_button_height = max(max_button_height, text_size.y);
		}
		if (!candidates.empty()) {
			total_buttons_width -= 32.0f;  // 最后一个按钮不需要右边距
		}
		ImGui::PopFont();

		const float window_width = total_buttons_width + 40.0f;
		const float window_height = max_button_height + vertical_padding * 2;
		ImGui::SetNextWindowSize(ImVec2(window_width, window_height), ImGuiCond_Always);

		// 设置窗口样式
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255, 255, 255, 255));
		ImGui::Begin("候选词", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoDecoration);

		ImGui::PushFont(font);
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

		// 计算居中起始位置
		const float horizontal_start = (window_width - total_buttons_width) / 2;
		ImGui::SetCursorPosX(horizontal_start);
		ImGui::SetCursorPosY(vertical_padding);

		// 绘制按钮组
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		const ImU32 separator_color = IM_COL32(200, 200, 200, 255);

		ImGui::BeginGroup();
		for (size_t i = 0; i < candidates.size(); ++i) {
			if (i > 0) {
				// 绘制分隔线
				const ImVec2 line_start = ImGui::GetItemRectMax();
				const ImVec2 line_end(line_start.x, line_start.y - ImGui::GetItemRectSize().y);
				draw_list->AddLine(line_start, line_end, separator_color, 1.0f);

				ImGui::SameLine(0, 24);
			}

			// 透明按钮样式
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(200, 200, 200, 80));

			auto candidate = wstring_to_utf8(candidates[i]);

			if (ImGui::Button(candidate.c_str())) {
				on_candidate_selected(candidate);
			}

			ImGui::PopStyleColor(2);
		}
		ImGui::EndGroup();

		// 恢复样式
		ImGui::PopStyleColor();
		ImGui::PopFont();
		ImGui::End();
		ImGui::PopStyleColor();
	}

	void ui_platform::ui_thread_func() {
		// 初始化GLFW
		if (!glfwInit()) return;

		// 配置OpenGL上下文
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		// 2. 创建透明无边框窗口
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);      // 无边框
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); // 透明背景
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);      // 禁止调整大小

		GLFWwindow* window = glfwCreateWindow(
			2000, 2000,
			"Input Method Candidate Window",
			nullptr, nullptr
		);

		if (!window) {
			modian::core::logger_service::logger()->error("Failed to create GLFW window");
			glfwTerminate();
			return;
		}

		// 3. 设置窗口位置和置顶
		glfwSetWindowPos(window, 100, 100);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // 启用垂直同步

		// 4. 初始化 ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// 加载中文字体
		font = io.Fonts->AddFontFromFileTTF(
			"C:/Windows/Fonts/msyh.ttc",  // 微软雅黑字体
			40.0f,
			nullptr,
			io.Fonts->GetGlyphRangesChineseFull());

		// 加载默认字体作为fallback
		ImFontConfig config;
		config.SizePixels = 40.0f;
		io.Fonts->AddFontDefault(&config);

		// 构建字体纹理
		io.Fonts->Build();

		// 初始化后端
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		// 配置样式
		ImGui::StyleColorsLight();
		ImGui::GetStyle().WindowRounding = 5.0f;

		// 主循环
		while (!glfwWindowShouldClose(window) && !exit_flag_) {
			glfwPollEvents();

			// 准备新帧
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			render_candidates();

			// 渲染绘制
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}

		// 清理资源
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}
}
