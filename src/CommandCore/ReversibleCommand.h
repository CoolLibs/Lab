#pragma once

namespace Lab {

class IReversibleCommand;

using ReversibleCommand = std::shared_ptr<const IReversibleCommand>;

} // namespace Lab
