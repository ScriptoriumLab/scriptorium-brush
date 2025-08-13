#pragma once

#include <Windows.h>

namespace modian::infra::tsf::dll {
	inline HINSTANCE modian_instance{nullptr};

	struct auto_com {
		auto_com();
		~auto_com();
	private:
		enum class apartment_type {
			APARTMENT_UNKNOWN,
			APARTMENT_NEWLY_INIT,  // 本次调用实际初始化了COM
			APARTMENT_EXISTING     // COM已被其他代码初始化
		};
		apartment_type apartment_type_;
	};

	struct com_registration {
		com_registration() = delete;

		static bool register_profiles();
		static void unregister_profiles();

		static bool register_categories();
		static void unregister_categories();

		static bool register_server();
		static void unregister_server();
	private:
		static LONG recurse_delete_key(_In_ const HKEY& h_parent_key, _In_ const LPCTSTR& lpsz_key);
	};
}