#pragma once

enum class ShaderType {
	Vertex,
	Fragment,
	Compute
};

class Shader {
public:
	Shader(ShaderType type, const std::string& filepathOrSourceCode, bool bIsFilepath = true);
	Shader(Shader&& other) noexcept;
	~Shader();
	void operator=(Shader&& o);

	inline unsigned int getID() const { return m_shaderID; }
	inline const std::string& getFilepath() const { return m_bIsFilepath ? m_filepathOrSourceCode : "error..."; }

private:
friend class ShaderPipeline;
	void compile();
	std::string parseFile(const std::string& filepath);

private:
	unsigned int m_type;
	std::string m_filepathOrSourceCode;
	bool m_bIsFilepath;
	unsigned int m_shaderID;
};