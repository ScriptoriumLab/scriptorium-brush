#pragma once

#include <memory>
#include <vector>
#include <string>

#include "modian/core/observer/candidate_observer.h"

namespace modian::brush::manager {
	class candidate_manager {
	public:
		bool update_candidates(const std::vector<std::wstring>& candidates);

		void add_observer(const std::shared_ptr<core::candidate_observer>& observer);
		void remove_observer(const std::shared_ptr<core::candidate_observer>& observer);
	private:
		std::vector<std::wstring> candidates_;

		std::vector<std::shared_ptr<core::candidate_observer>> observers_;
		void notify_observers() const;
	};
}