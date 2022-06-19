#pragma once

#include <Cool/FileWatcher/FileWatcher.h>
#include <Cool/Parameter/ParameterList.h>
#include "ShaderManager.h"

class ShaderManager_FromText : public ShaderManager {
public:
    ShaderManager_FromText();

    void setup_for_rendering(const Cool::Camera& camera, float time, float aspect_ratio) override;
    void set_image_in_shader(std::string_view name, int slot, GLuint texture_id);

    inline void update() override { _file_watcher.update(); }
    void        imgui_windows() override;

    void set_shader_path(std::string_view path);

private:
    void compile_shader(std::string_view path);
    void parse_shader_for_params(std::string_view path);

private:
    Cool::FileWatcher   _file_watcher;
    Cool::ParameterList _parameters;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Shader Path", _file_watcher.path().string()),
            cereal::make_nvp("Parameters", _parameters));
    }
    template<class Archive>
    void load(Archive& archive)
    {
        std::string path;
        archive(
            path,
            _parameters);
        set_shader_path(path);
    }
};