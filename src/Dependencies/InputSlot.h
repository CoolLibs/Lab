#pragma once
#include <Cool/FileWatcher/FileWatcher.h>
#include <reg/reg.hpp>
#include "AllVariableTypes.h"
#include "Dirty.h"
#include "VariableRegistries.h"

namespace Lab {

template<typename T>
class InputSlot {
public:
    explicit InputSlot(DirtyFlag dirty_flag = {}, std::string_view name = "")
        : _dirty_flag{dirty_flag}
        , _name{name}
    {
    }

    std::string_view name() const { return _name; }

    void set_dirty_if_depends_on(const reg::Id<T>& variable_id, SetDirtyFlag_Ref set_dirty) const
    {
        auto name          = reg::to_string(id);
        auto variable_name = reg::to_string(variable_id);
        if (id == variable_id) {
            set_dirty(_dirty_flag);
        }
    }

public: // This section is used by the InputProvider to do its job
    friend class InputProvider;
    friend class Ui;
    friend class InputSlotDestructorRef;
    mutable reg::Id<T> id;
    DirtyFlag          _dirty_flag;

private: // This section is really private
    std::string _name;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id, _name, _dirty_flag);
    }
};

class InputSlot_File {
public:
    InputSlot_File() = default; // For serialization :( Remove whenever possible
    explicit InputSlot_File(DirtyFlag dirty_flag)
        : _dirty_flag{dirty_flag}
    {
    }

    void update(SetDirtyFlag_Ref set_dirty)
    {
        file_watcher.update(
            {.on_file_changed = [&](std::string_view) { set_dirty(_dirty_flag); },
             .on_path_invalid = [](std::string_view path) { Cool::Log::ToUser::error("Input File", "Invalid path: \"{}\"", path); }});
    }

private:
    friend class Ui;
    friend class InputProvider;
    friend class InputSlotDestructorRef;
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

struct InputSlot_AspectRatio {
};
struct InputSlot_Time {
};

using AnyInputSlotRef =
    AllVariableTypes::
        wrap<InputSlot>::
            wrap<std::reference_wrapper>::
                to<std::variant>;

namespace internal {
template<typename T>
struct make_ref_to_const_input_slot {
    using type = std::reference_wrapper<const InputSlot<T>>;
};
} // namespace internal

using AnyInputSlotRefToConst =
    AllVariableTypes::
        map<internal::make_ref_to_const_input_slot>::
            to<std::variant>;

using AnyInputSlot =
    AllVariableTypes::
        wrap<InputSlot>::
            to<std::variant>;

class InputSlotDestructorRef {
public:
    explicit InputSlotDestructorRef(VariableRegistries& registries)
        : _variable_registries{registries}
    {
    }

    void operator()(const AnyInputSlot& slot)
    {
        std::visit([&](auto&& slot) { _variable_registries.get().destroy(slot.id); }, slot);
    }

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
};

using AllInputSlots = std::vector<AnyInputSlotRefToConst>;

} // namespace Lab