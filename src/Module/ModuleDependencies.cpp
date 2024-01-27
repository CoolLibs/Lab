#include "ModuleDependencies.h"
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Cool/String/String.h"
#include "Nodes/Node.h"

namespace Lab {

static auto contains_two_or_more(std::string_view word, std::string_view text) -> bool
{
    auto const pos = Cool::String::find_word(word, text, 0);
    if (pos == std::string_view::npos)
        return false;

    auto const pos2 = Cool::String::find_word(word, text, pos + 1);
    return pos2 != std::string_view::npos;
}

void update_dependencies_from_shader_code(ModuleDependencies& dependencies, std::string shader_code)
{
    shader_code = Cool::String::remove_comments(shader_code);

    dependencies.time           = contains_two_or_more("_time", shader_code);
    dependencies.audio_volume   = contains_two_or_more("_audio_volume", shader_code);
    dependencies.audio_waveform = contains_two_or_more("_audio_waveform", shader_code);
    dependencies.audio_spectrum = contains_two_or_more("_audio_spectrum", shader_code);
}

void update_dependencies_from_nodes_graph(ModuleDependencies& dependencies, Cool::NodesGraph const& graph, Cool::InputProvider_Ref input_provider)
{
    dependencies.osc_channels.clear();
    dependencies.midi_channels.clear();

    graph.for_each_node<Node>([&](Node const& node) { // TODO(Modules) Only check for the nodes that are actually compiled in the graph. Otherwise we might pick up dependencies from other modules or from unused nodes.
        for (auto const& value_input : node.value_inputs())
        {
            {
                auto const* osc_channel = std::get_if<Cool::Input<Cool::OSCChannel>>(&value_input);
                if (osc_channel)
                    dependencies.osc_channels.insert(input_provider(*osc_channel));
            }
            {
                auto const* midi_channel = std::get_if<Cool::Input<Cool::MidiChannel>>(&value_input);
                if (midi_channel)
                    dependencies.midi_channels.insert(input_provider(*midi_channel));
            }
        }
    });
}

} // namespace Lab
