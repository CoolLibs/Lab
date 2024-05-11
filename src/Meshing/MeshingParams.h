#pragma once

namespace Meshing {

struct MeshingParams {
    glm::uvec3 sampling_count{75};

    void imgui();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("sampling count", sampling_count)
        );
    }
};

} // namespace Meshing
