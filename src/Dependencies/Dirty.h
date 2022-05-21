#pragma once

#include <reg/reg.hpp>

namespace Lab {

namespace internal {
struct IsDirty {
    bool is_dirty = true;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Is Dirty", is_dirty));
    }
};

inline auto to_string(IsDirty is_dirty) -> std::string
{
    return is_dirty.is_dirty ? "Dirty" : "Clean";
}
} // namespace internal

class DirtyFlag {
public:
    DirtyFlag() = default; // For serialization :( Remove whenever possible
private:
    friend class DirtyManager;
    friend class SetDirtyFlag;
    friend class DirtyFlagFactory;

    explicit DirtyFlag(reg::Id<internal::IsDirty> id)
        : id{id}
    {
    }

    reg::Id<internal::IsDirty> id;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id);
    }
};

using DirtyRegistry = reg::Registry<internal::IsDirty>;

class DirtyFlagFactory {
public:
    explicit DirtyFlagFactory(DirtyRegistry& registry)
        : _registry{registry}
    {
    }

    auto create()
    {
        return DirtyFlag{_registry.get().create({})};
    }

private:
    std::reference_wrapper<DirtyRegistry> _registry;
};

class SetDirtyFlag {
public:
    explicit SetDirtyFlag(DirtyRegistry& registry)
        : _registry{registry}
    {
    }

    void operator()(DirtyFlag flag)
    {
        _registry.get().set(flag.id, {true});
    }

private:
    std::reference_wrapper<DirtyRegistry> _registry;
};

class DirtyManager {
public:
    explicit DirtyManager(DirtyRegistry& registry)
        : _registry{registry}
    {
    }

    auto is_dirty(DirtyFlag flag) const -> bool
    {
        const auto dirty = _registry.get().get(flag.id);
        if (!dirty.has_value()) {
            throw std::runtime_error{"DirtyFlag has been deleted but someone still had a handle to it!"};
        }
        return dirty->is_dirty;
    }

    void set_clean(DirtyFlag flag)
    {
        _registry.get().set(flag.id, {false});
    }

private:
    std::reference_wrapper<DirtyRegistry> _registry;
};

} // namespace Lab
