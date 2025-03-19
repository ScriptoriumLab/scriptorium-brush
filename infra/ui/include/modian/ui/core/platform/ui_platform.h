#pragma once

#include <imgui.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#include "modian/core/observer/candidate_observer.h"

namespace modian::infra::ui::core::platform {
	class ui_platform final : public modian::core::candidate_observer {
	public:
        ui_platform();
        ~ui_platform() override;

    	static std::shared_ptr<ui_platform> instance();
        void start_ui_thread();
        void stop_ui_thread();
        void on_candidate_update(const std::vector<std::wstring>& candidates) override;

        ui_platform(const ui_platform&) = delete;
        ui_platform(ui_platform&&) = delete;
        ui_platform& operator=(const ui_platform&) = delete;
        ui_platform& operator=(ui_platform&&) = delete;
	private:
        void ui_thread_func();
        inline void render_candidates();
		void on_candidate_selected(const std::string& candidate);
		static std::string wstring_to_utf8(const std::wstring& wstr);

        std::thread ui_thread_;
        std::atomic<bool> is_thread_running_;
		std::atomic<bool> exit_flag_;
        std::mutex thread_mutex_;
        std::condition_variable thread_condition_;
		std::mutex candidate_queue_mutex_;
        std::queue<std::vector<std::wstring>> candidates_queue_{};

		ImFont* font{};
	};
}
