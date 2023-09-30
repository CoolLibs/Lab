#include <Cool/Gpu/OpenGL/SSBO.h>
#include <Cool/Gpu/OpenGL/ComputeShader.h>

#include <glm/gtx/component_wise.hpp>

namespace Lab {

void meshing_test(std::string const& shader_code) {
    Cool::Log::Debug::info("Meshing", "test");
    std::cout << shader_code << std::endl;

    ImGui::SetClipboardText(shader_code.c_str());
    constexpr glm::uvec3 workGroupSize(8, 8, 8);


    //TODO set sampling_count as it make sense
    const glm::vec3 boxSize = glm::vec3(2.f);
    const float meshing_step_size = 0.2f;
    glm::uvec3 sampling_count = glm::uvec3(3);

    if constexpr (COOL_OPENGL_VERSION < 430) {
        Cool::Log::Debug::info("Meshing", "OpenGL version is lower than 430. Meshing need compute shaders, which are not supported by this version.");
        return;    
    }

    size_t ssbo_size = glm::compMul(sampling_count);
    Cool::SSBO<float> ssbo(0);
    ssbo.upload_data(ssbo_size, nullptr);

    Cool::OpenGL::ComputeShader computeShader(workGroupSize, shader_code);
    computeShader.bind();
    computeShader.set_uniform("_time", 0.f);
    computeShader.set_uniform("_box_size", boxSize);
    computeShader.set_uniform("_step_size", meshing_step_size);

    // TODO set uniform for each node (like in file Module_Nodes.cpp)
    // _nodes_editor.graph().for_each_node<Node>([&](Node const& node) { // TODO(Nodes) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work.
    //     for (auto const& value_input : node.value_inputs())
    //     {
    //         std::visit([&](auto&& value_input) {
    //             send_uniform(value_input, shader, in.provider);
    //         },
    //                    value_input);
    //     }
    // });

	computeShader.compute(sampling_count);
	
    // CPU get data back
    std::vector<float> v;
    v.resize(ssbo_size);
	ssbo.download_data(v);

    for (size_t i = 0; i < v.size(); i++) {
        std::string s = "v[" + std::to_string(i) + "] = " + std::to_string(v[i]);
        Cool::Log::Debug::info("Meshing", s);
    }
}

}  // namespace Lab