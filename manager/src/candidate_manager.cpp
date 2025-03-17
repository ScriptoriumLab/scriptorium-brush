#include "modian/manager/candidate_manager.h"

#include <iostream>
#include <ostream>

#include "modian/core/logger/logger_service.h"

bool modian::manager::candidate_manager::update_candidates(const std::vector<std::wstring>& candidates) {
	candidates_ = candidates;

	if (!candidates.empty()) {
		core::logger_service::logger()->info("Get potential candidates");
		for (const auto& candidate : candidates) {
			core::logger_service::logger()->info("Candidates: {}", candidate);
		}

		notify_observers();

		return true;
	}

	return false;
}

void modian::manager::candidate_manager::add_observer(const std::shared_ptr<core::candidate_observer>& observer) {
	if (std::ranges::find(observers_, observer) == observers_.end()) {
		observers_.emplace_back(observer);
	}
}

void modian::manager::candidate_manager::remove_observer(const std::shared_ptr<core::candidate_observer>& observer) {
	std::erase(observers_, observer);
}

void modian::manager::candidate_manager::notify_observers() const {
	for (const auto& observer : observers_) {
		observer->on_candidate_update(candidates_);
	}
}
