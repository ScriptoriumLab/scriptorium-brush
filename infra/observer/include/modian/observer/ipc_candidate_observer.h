#pragma once

#include <vector>
#include <string>
#include <format>

#include "modian/core/observer/candidate_observer.h"
#include "modian/ipc/ipc_client.h"
#include "modian/core/utils/utils.h"

namespace modian::brush::infra::observer {
	class ipc_candidate_observer final : public core::candidate_observer {
	public:
		void on_candidate_update(const std::vector<std::wstring>& candidates) override {
			// 1. 处理空列表
			if (candidates.empty()) {
				// 发送一个特殊的清空指令
				client_.send(std::string{1, '\x1B'});
				return;
			}

			// 2. 序列化协议
			// 格式: DATA|候选词1|候选词2|...
			std::string payload = "DATA";
			for (const auto& cand : candidates) {
				payload += "|";
				payload += core::utils::to_utf8(cand);
			}

			// 3. 发送
			client_.send(payload);
		}

	private:
		ipc::ipc_client client_;
	};
}