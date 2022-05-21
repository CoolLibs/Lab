#pragma once
#include <Cool/FileWatcher/FileWatcher.h>
#include <reg/reg.hpp>
#include "Dirty.h"
#include "Registries.h"

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
    InputSlot_File() = default; // For serialization :( Remove whenever possible
    explicit InputSlot_File(DirtyFlag dirty_flag)
        : _dirty_flag{dirty_flag}
    {
    }

    void update(SetDirtyFlag set_dirty)
    {
        file_watcher.update(
            {.on_file_changed = [&](std::string_view) { set_dirty(_dirty_flag); },
             .on_path_invalid = [](std::string_view path) { Cool::Log::ToUser::error("Input File", "Invalid path: \"{}\"", path); }});
    }

private:
    friend class Ui;
    friend class InputProvider;
    Cool::FileWatcher file_watcher{};
    DirtyFlag         _dirty_flag;

private:
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(cereal::make_nvp("File Path", file_watcher.path().string()),
                _dirty_flag);
    }
    template<class Archive>
    void load(Archive& archive)
    {
        std::string path;
        archive(path,
                _dirty_flag);
        file_watcher.set_path(path);
    }
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