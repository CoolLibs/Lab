#pragma once

namespace Lab {

struct TestCommand {
    int bob{5};

    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Bob", bob)
        );
    }
};

} // namespace Lab