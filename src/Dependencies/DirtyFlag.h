#pragma once

namespace Lab {

struct DirtyFlag {
    bool is_dirty = true;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Is dirty", is_dirty));
    }
};

} // namespace Lab
