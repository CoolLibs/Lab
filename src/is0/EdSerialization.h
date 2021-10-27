#pragma once

namespace ax::NodeEditor {

template<class Archive, typename EdId>
void save(Archive& archive, const EdId& id)
{
    archive(cereal::make_nvp("id", id.Get()));
}
template<class Archive, typename EdId>
void load(Archive& archive, EdId& id)
{
    uintptr_t val;
    archive(val);
    id = EdId{val};
}

} // namespace ax::NodeEditor