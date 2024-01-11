#include "shader_dependency.h"
#include "Cool/String/String.h"

namespace Lab {

static auto contains_two_or_more(std::string_view word, std::string_view text) -> bool
{
    auto const pos = Cool::String::find_word(word, text, 0);
    if (pos == std::string_view::npos)
        return false;

    auto const pos2 = Cool::String::find_word(word, text, pos + 1);
    return pos2 != std::string_view::npos;
}

auto ModuleShaderDependencyFlags::reset() -> void
{
    depends_on_time           = false;
    depends_on_audio_volume   = false;
    depends_on_audio_waveform = false;
    depends_on_audio_spectrum = false;
}
auto ModuleShaderDependencyFlags::compute_dependencies(std::string shader_code) -> void
{
    shader_code               = Cool::String::remove_comments(shader_code);
    depends_on_time           = contains_two_or_more("_time", shader_code);
    depends_on_audio_volume   = contains_two_or_more("_audio_volume", shader_code);
    depends_on_audio_waveform = contains_two_or_more("_audio_waveform", shader_code);
    depends_on_audio_spectrum = contains_two_or_more("_audio_spectrum", shader_code);
}

} // namespace Lab
