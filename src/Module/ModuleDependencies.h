#pragma once
#include <set>
#include "Cool/Midi/MidiChannel.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/OSC/OSCChannel.h"

namespace Lab {

// We don't want to rerender when audio / time changes if we don't depend on them.
/// Also, audio features are costly to compute, so we only set these uniforms in the shader if we actually need them.
struct ModuleDependencies {
    bool                        time{false};
    bool                        audio_volume{false};
    bool                        audio_waveform{false};
    bool                        audio_spectrum{false};
    std::set<Cool::OSCChannel>  osc_channels{};
    std::set<Cool::MidiChannel> midi_channels{};

    auto audio() const -> bool { return audio_volume || audio_waveform || audio_spectrum; }
    auto osc_channel(Cool::OSCChannel const& osc_channel) const -> bool { return osc_channels.find(osc_channel) != osc_channels.end(); }
    auto midi_channel(Cool::MidiChannel const& midi_channel) const -> bool { return midi_channels.find(midi_channel) != midi_channels.end(); }
};

void update_dependencies_from_shader_code(ModuleDependencies& dependencies, std::string shader_code);
void update_dependencies_from_nodes_graph(ModuleDependencies& dependencies, Cool::NodesGraph const&);

} // namespace Lab
