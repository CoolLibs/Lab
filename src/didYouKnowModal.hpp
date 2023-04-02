#pragma once

#include <string>
#include <vector>
class DidYouKnowModal {
public:
    DidYouKnowModal() = default;

    void open();
    void prepare_next_tip();

    bool is_open() const;

    void display_modal_content();

    void all_tips();

private:
    std::vector<std::string> _all_tips = {
        "You can hold SHIFT to disable docking. Useful when you try to move a window around and docking gets in your way.",
        "The undo-history (CTRL+Z) of your modifications is preserved even when you close and re-open CoolLab! You can control its size in 'Settings > History Size'"};

    std::string _id              = "Did you know?";
    bool        _has_been_opened = false;
    std::string _text            = _all_tips[0];

    size_t _current_tip_index = 0;

    __time64_t _current_timestamp;

    friend void debug_did_you_know(DidYouKnowModal& _did_you_know_);

    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Last tip", _current_tip_index),
            cereal::make_nvp("Last timestamp", _current_timestamp)
        );
    }
};

void test_did_you_know(DidYouKnowModal& _did_you_know_);