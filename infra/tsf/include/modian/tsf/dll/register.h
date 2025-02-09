#pragma once

namespace modian::tsf::dll {
	bool register_server();
	void unregister_server();

	bool register_profiles();
	void unregister_profiles();

	bool register_categories();
	void unregister_categories();
}