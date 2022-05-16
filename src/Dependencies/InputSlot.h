#pragma once
#include <Cool/FileWatcher/FileWatcher.h>
#include <reg/reg.hpp>
#include "DirtyFlag.h"

namespace Lab {

template<typename T>
class InputSlot {
public:
    explicit InputSlot(std::string_view name)
        : _name{name}
    {
    }

    std::string_view name() const { return _name; }

public: // This section is used by the InputProvider to do its job
    friend class InputProvider;
    friend class Ui;
    reg::Id<T> id;

private: // This section is really private
    std::string _name;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id);
    }
};

class InputSlot_File {
public:
    InputSlot_File(reg::Id<DirtyFlag> dirty_flag)
        : file_watcher{"",
                       {.on_file_changed = [dirty_flag](std::string_view path) { /*registries.set(dirty_flag, true);*/ },
                        .on_path_invalid = [](std::string_view path) { Cool::Log::ToUser::error("Input File", "Invalid path: \"{}\"", path); }}}
    {
    }

private:
    friend class Ui;
    Cool::FileWatcher file_watcher;

private:
};

using AnyInputSlotRef = std::variant<
    std::reference_wrapper<InputSlot<float>>,
    std::reference_wrapper<InputSlot<glm::vec3>>>;
using AnyInputSlotRefToConst = std::variant<
    std::reference_wrapper<const InputSlot<float>>,
    std::reference_wrapper<const InputSlot<glm::vec3>>>;
using AnyInputSlot = std::variant<
    InputSlot<float>,
    InputSlot<glm::vec3>>;

} // namespace Lab