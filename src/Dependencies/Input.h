#pragma once
#include <Cool/FileWatcher/FileWatcher.h>
#include <reg/reg.hpp>
#include "Dependencies/VariableId.h"
#include "Dirty.h"
#include "VariableRegistries.h"

namespace Lab {

template<typename T>
class Input {
public:
    Input() = default; // For serialization

    auto name() const -> const std::string& { return _name; }

    void set_dirty_if_depends_on(const VariableId<T>& variable_id, SetDirtyFlag_Ref set_dirty) const
    {
        if (_current_variable_id == variable_id ||
            (_current_variable_id.underlying_uuid().is_nil() && _default_variable_id == variable_id))
        {
            set_dirty(_dirty_flag);
        }
    }

private:
    friend class InputFactory_Ref;
    explicit Input(const DirtyFlag& dirty_flag, std::string_view name, const VariableId<T> default_variable_id) // Use InputFactory_Ref::make() to create an input
        : _dirty_flag{dirty_flag}
        , _name{name}
        , _default_variable_id{default_variable_id}
    {
    }

private:
    friend class InputProvider_Ref;
    friend class Ui_Ref;
    friend class InputDestructor_Ref;
    DirtyFlag     _dirty_flag;
    std::string   _name; // NB: both the Input and the Variables have a name, because they are two different things and the current variable could be coming from a global pool and not be related to this Input at all.
    VariableId<T> _default_variable_id;
    VariableId<T> _current_variable_id;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", _name),
            cereal::make_nvp("Default Variable ID", _default_variable_id),
            cereal::make_nvp("Current Variable ID", _current_variable_id),
            cereal::make_nvp("Dirty Flag", _dirty_flag)
        );
    }
};

class Input_File {
public:
    Input_File() = default; // For serialization :( Remove whenever possible
    explicit Input_File(DirtyFlag dirty_flag)
        : _dirty_flag{dirty_flag}
    {
    }

    void update(SetDirtyFlag_Ref set_dirty)
    {
        file_watcher.update(
            {.on_file_changed = [&](std::string_view) { set_dirty(_dirty_flag); },
             .on_path_invalid = [](std::string_view path) {
                 Cool::Log::ToUser::error("Input File", "Invalid path: \"{}\"", path);
             }}
        );
    }

private:
    friend class Ui_Ref;
    friend class InputProvider_Ref;
    friend class InputDestructor_Ref;
    Cool::FileWatcher file_watcher{};
    DirtyFlag         _dirty_flag;

private:
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("File Path", file_watcher.path().string()),
            _dirty_flag
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        std::string path;
        archive(
            path,
            _dirty_flag
        );
        file_watcher.set_path(path);
    }
};

struct Input_AspectRatio {
};
struct Input_Time {
};

#include "generated/AnyInput.inl"
#include "generated/AnyInputRef.inl"
#include "generated/AnyInputRefToConst.inl"

class InputDestructor_Ref {
public:
    explicit InputDestructor_Ref(VariableRegistries& registries)
        : _variable_registries{registries}
    {
    }

    void operator()(const AnyInput& input)
    {
        std::visit([&](auto&& input) { _variable_registries.get().destroy(input._default_variable_id); }, input);
    }

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
};

using AllInputRefsToConst = std::vector<AnyInputRefToConst>;

} // namespace Lab