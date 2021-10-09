#pragma once

#include <Cool/FileWatcher/FileWatcher.h>
#include <Cool/Parameters/ParameterDynamicList.h>
#include "ShaderManager.h"

class ShaderManager_FromText : public ShaderManager {
public:
    ShaderManager_FromText();

    void setup_for_rendering(const Cool::Camera& camera, float time) override;

    inline void update() override { _file_watcher.update(); }
    void        imgui_window() override;

    void set_shader_path(std::string_view path);

private:
    void compile_shader(std::string_view path);
    void parse_shader_for_params(std::string_view path);

private:
    Cool::FileWatcher          _file_watcher;
    Cool::ParameterDynamicList _parameters;

private:
    //Serialization
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