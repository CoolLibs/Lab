#include "ComputeShader.h"

static constexpr unsigned int WORK_GROUP_SIZE = 256;

ComputeShader::ComputeShader(const std::string& filepath)
{
    m_shader.addShader(ShaderType::Compute, filepath);
    m_shader.createProgram();
}

ComputeShader::ComputeShader(ComputeShader&& o) noexcept
    : m_shader(std::move(o.m_shader))
{}
void ComputeShader::operator=(ComputeShader&& o) {
    m_shader = std::move(o.m_shader);
}

void ComputeShader::setSrcCode(const std::string& sourceCode) {
    m_shader.addShaderBySrcCode(ShaderType::Compute, sourceCode);
    m_shader.createProgram();
}

void ComputeShader::compute(unsigned int nbComputations) {
    m_shader.bind();
    glDispatchCompute(nbComputations / WORK_GROUP_SIZE + ((nbComputations % WORK_GROUP_SIZE == 0) ? 0 : 1), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    m_shader.unbind();
}