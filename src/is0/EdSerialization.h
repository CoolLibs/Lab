#pragma once

namespace ax::NodeEditor {

template<class Archive>
void save(Archive& archive, const PinId& id)
{
    archive(cereal::make_nvp("id", id.Get()));
}
template<class Archive>
void load(Archive& archive, PinId& id)
{
    uintptr_t val;
    archive(val);
    id = ed::PinId{val};
}

template<class Archive>
void serialize(Archive& archive, LinkId& id)
{
    archive(static_cast<uint64_t>(id));
}

} // namespace ax::NodeEditor