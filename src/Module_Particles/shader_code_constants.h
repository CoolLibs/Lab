#pragma once

namespace Lab {

auto constexpr version = R"glsl()glsl";

auto constexpr uniforms = R"glsl(
)glsl";

auto constexpr includes = R"glsl()glsl";

auto constexpr buffers = R"glsl(
layout(std430, binding = 0) buffer _positions_buffer
{
    float _positions[];
};

layout(std430, binding = 1) buffer _velocities_buffer
{
    float _velocities[];
};

layout(std430, binding = 2) buffer _sizes_buffer
{
    float _sizes[];
};

layout(std430, binding = 3) buffer _lifetimes_buffer
{
    float _lifetimes[];
};

layout(std430, binding = 4) buffer _lifetime_maxs_buffer
{
    float _lifetime_maxs[];
};

layout(std430, binding = 5) buffer _colors_buffer
{
    float _colors[];
};

)glsl";

} // namespace Lab