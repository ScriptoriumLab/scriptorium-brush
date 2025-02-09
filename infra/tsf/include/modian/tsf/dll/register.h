#pragma once

#include <Windows.h>

namespace modian::tsf::dll {
	struct auto_com {
		auto_com();
		~auto_com();

		bool b_init;
	};

	bool register_server();
	void unregister_server();

	bool register_profiles();
	void unregister_profiles();

	bool register_categories();
	void unregister_categories();
}