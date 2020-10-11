#pragma once

namespace glm {
    template<class Archive>
    void serialize(Archive& archive, glm::vec2& v)
    {
        archive(v.x, v.y);
    }

    template<class Archive>
    void serialize(Archive& archive, glm::vec3& v)
    {
        archive(v.x, v.y, v.z);
    }
}