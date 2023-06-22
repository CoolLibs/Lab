#pragma once
#include <Webcam/webcam_manager.h>
#include <opencv2/core/mat.hpp>
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
        if (res)
        {
            return i;
        }
    }

    return maxTested;
}

void WebcamManager::update()
{
    m_number_of_webcam = get_number_of_camera();
    for (int i = 0; i < m_number_of_webcam; i++)
    {
    }
}

void WebcamManager::add_webcam()
{
    cv::VideoCapture cam{m_number_of_webcam};
    cv::Mat          image;

    cv::cvtColor(image, image, cv::COLOR_BGR2RGB); // TODO(TD) in shader
    // image = cv::Mat::conver

    const auto width  = static_cast<unsigned int>(image.cols);
    const auto height = static_cast<unsigned int>(image.rows);

    m_list_webcam.push_back(Webcam{
        .m_texture = Cool::Texture{{width, height}, 3, reinterpret_cast<uint8_t*>(image.ptr()), {.interpolation_mode = glpp::Interpolation::NearestNeighbour}},
        .m_capture = cam,
        .m_mat     = cv::Mat{}});
}

void update_webcam(Webcam& webcam)
{
    webcam.m_capture >> webcam.m_mat;

    const auto width  = static_cast<unsigned int>(webcam.m_mat.cols);
    const auto height = static_cast<unsigned int>(webcam.m_mat.rows);

    webcam.m_texture.set_image({
                                   width,
                                   height,
                               },
                               3, reinterpret_cast<uint8_t*>(webcam.m_mat.ptr()));
}

} // namespace Lab