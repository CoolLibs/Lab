#pragma once
#include <vcruntime.h>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "get_webcam_texture.h"

namespace Lab {

struct Webcam {
    std::shared_ptr<Cool::Texture> m_texture{};
    cv::VideoCapture               m_capture{};
    cv::Mat                        m_mat{};
};

void update_webcam(Webcam& webcam);

class WebcamManager {
public:
    WebcamManager()  = default;
    ~WebcamManager() = default;

    auto get_webcam_texture(size_t index)
    {
        return m_list_webcam[index].m_texture;
        // return list_webcam_textures[m_active_cam_id];
        // return Cool::Texture{{1, 1}}; // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    }

    void update();

    auto is_webcam_used() -> bool { return m_number_of_webcam == 0; };

private:
    static auto get_number_of_camera() -> int;
    void        add_webcam();
    void        update_webcams();

public:
    bool m_is_webcam_used;

private:
    // int                 m_active_cam_id{}; // DEPENDS ON THE MODE SO NO NEED IN THE MANAGER
    int                 m_number_of_webcam{};
    std::vector<Webcam> m_list_webcam{};
};

} // namespace Lab