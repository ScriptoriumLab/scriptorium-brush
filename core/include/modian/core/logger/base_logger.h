#pragma once

#include <string>

namespace modian::core {
    static constexpr auto ascii_modian_ime = R"(
        =======================================================================================================================

         ,ggg, ,ggg,_,ggg,                                                               ,a8a,  ,ggg, ,ggg,_,ggg,     ,ggggggg,
        dP""Y8dP""Y88P""Y8b                     8I                                      ,8" "8,dP""Y8dP""Y88P""Y8b  ,dP""""""Y8b
        Yb, `88'  `88'  `88                     8I                                      d8   8bYb, `88'  `88'  `88  d8'    a  Y8
         `"  88    88    88                     8I   gg                                 88   88 `"  88    88    88  88     "Y8P'
             88    88    88                     8I   ""                                 88   88     88    88    88  `8baaaa
             88    88    88    ,ggggg,    ,gggg,8I   gg     ,gggg,gg   ,ggg,,ggg,       Y8   8P     88    88    88 ,d8P""""
             88    88    88   dP"  "Y8gggdP"  "Y8I   88    dP"  "Y8I  ,8" "8P" "8,      `8, ,8'     88    88    88 d8"
             88    88    88  i8'    ,8I i8'    ,8I   88   i8'    ,8I  I8   8I   8I 8888  "8,8"      88    88    88 Y8,
             88    88    Y8,,d8,   ,d8',d8,   ,d8b,_,88,_,d8,   ,d8b,,dP   8I   Yb,`8b,  ,d8b,      88    88    Y8,`Yba,,_____,
             88    88    `Y8P"Y8888P"  P"Y8888P"`Y88P""Y8P"Y8888P"`Y88P'   8I   `Y8  "Y88P" "Y8     88    88    `Y8  `"Y8888888

        =======================================================================================================================
    )";

	class base_logger {
	public:
		virtual ~base_logger() = default;
        virtual void info(const std::string& message) = 0;
        virtual void debug(const std::string& message) = 0;
        virtual void error(const std::string& message) = 0;
	};
}