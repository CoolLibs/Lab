#pragma once
#include <Webcam/webcam_manager.h>
#include <algorithm>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
#include "Cool/Gpu/Texture.h"
#include "get_webcam_texture.h"

namespace Lab {

auto WebcamManager::get_number_of_camera() -> int // code from se0kjun : https://gist.github.com/se0kjun/f4b0fdf395181b495f79
{
    int maxTested = 10;

    for (int i = 0; i < maxTested; i++)
    {
        cv::VideoCapture temp_camera(i);
        bool             res = (!temp_camera.isOpened());
        temp_camera.release();
        if (!res)
        {
            return i;
        }
    }

    return maxTested;
}

void WebcamManager::update()
{
    if (m_number_of_webcam < 1)
    {
        m_number_of_webcam++;
        add_webcam();
    }

    const int diff = m_number_of_webcam - m_list_webcam.size(); // check if there is closed or open cameras

    update_webcams();
}

void WebcamManager::add_webcam()
{
    int              id = std::max(m_number_of_webcam - 1, 0);
    cv::VideoCapture cam{id};
    cv::Mat          image;
    cam >> image;

    const auto width  = static_cast<unsigned int>(image.cols);
    const auto height = static_cast<unsigned int>(image.rows);

    // Do i need to fill the texture at first ?
    m_list_webcam.push_back(Webcam{
        .m_texture = std::make_shared<Cool::Texture>(Cool::Texture{{width, height}, 3, reinterpret_cast<uint8_t*>(image.ptr()), {.interpolation_mode = glpp::Interpolation::NearestNeighbour}}),
        .m_capture = cam,
        .m_name    = cam.getBackendName()});
}

void WebcamManager::update_webcams()
{
    for (Webcam& webcam : m_list_webcam)
        update_webcam(webcam);
}

void update_webcam(Webcam& webcam)
{
    cv::Mat mat;
    webcam.m_capture >> mat;

    const auto width  = static_cast<unsigned int>(mat.cols);
    const auto height = static_cast<unsigned int>(mat.rows);

    webcam.m_texture->set_image({
                                    width,
                                    height,
                                },
                                3, reinterpret_cast<uint8_t*>(mat.ptr()));
}

} // namespace Lab