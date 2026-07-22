#pragma once

#include <Windows.h>

namespace scriptorium::brush::infra::tsf::dll {
	inline HINSTANCE scriptorium_instance{nullptr};

	struct com_registration {
		com_registration() = delete;

		static bool register_profiles();
		static void unregister_profiles();

		static bool register_categories();
		static void unregister_categories();

		static bool register_server();
		static void unregister_server();
	};
}