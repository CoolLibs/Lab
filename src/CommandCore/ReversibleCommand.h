#pragma once

namespace Lab {

class IReversibleCommand;

using ReversibleCommand = std::shared_ptr<IReversibleCommand const>;

} // namespace Lab
