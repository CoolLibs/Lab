#include "Module_FeedbackLoop.hpp"

namespace Lab {

Module_FeedbackLoop::Module_FeedbackLoop()
    : Module{"Feedback Loop"}
{
}

// TODO(FeedbackLoop) each time the module before you renders, you need to request a rerender for the next frame

void Module_FeedbackLoop::on_time_reset()
{
    // TODO(FeedbackLoop) Reset the textures
}

void Module_FeedbackLoop::render(DataToPassToShader const& data)
{
    // store the texture from the predecessor module
    // return the texture of the previous frame
}

} // namespace Lab
