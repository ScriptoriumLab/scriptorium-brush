#pragma once

#include <windows.h>

#include "modian/common/core/protocol/v1/input/key_event.h"

namespace modian::brush::infra::tsf::utils {
    common::core::protocol::input::v1::key_event translate_os_key(WPARAM key);
}
