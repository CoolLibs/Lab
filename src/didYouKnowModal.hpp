#pragma once

#include <string>
#include <vector>
class DidYouKnowModal {
public:
    DidYouKnowModal() = default;

    void open();
    void close();

    bool is_open() const;

    void displayModalContent();

private:
    std::vector<std::string> _all_tips = {
        "You can hold SHIFT to disable docking. Useful when you try to move a window around and docking gets in your way.",
        "The undo-history (CTRL+Z) of your modifications is preserved even when you close and re-open CoolLab! You can control its size in 'Settings > History Size'"};

    std::string _id              = "Did you know?";
    bool        _has_been_opened = false;
    std::string _text            = _all_tips[0];
};

void test_did_you_know(DidYouKnowModal& _did_you_know_);