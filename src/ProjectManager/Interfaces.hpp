#pragma once

namespace Lab {

using SetWindowTitle  = std::function<void(std::string_view)>;
using OnProjectLoaded = std::function<void()>;

} // namespace Lab