#include "get_webcam_texture.h"
#include <glpp/Enums/Interpolation.h>
#include <stdint.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "Cool/Gpu/Texture.h"

namespace Lab {

// uint8_t* get_data_from_mat(const cv::Mat& mat)
// {
//     int     size;
//     uint8_t data[mat.total()];
//     for(unsigned int i=0; i<mat.r)
// }

auto get_webcam_texture(const int index) -> Cool::Texture
{
    // return Cool::Texture{{10, 10}};

    static cv::VideoCapture cap{index};

    if (!cap.isOpened())
    {
        return Cool::Texture{{10, 10}};
    }

    cv::Mat image;
    cap >> image;
    // cv::waitKey(25);
    // image = cv::Mat::conver

    const auto width  = static_cast<unsigned int>(image.cols);
    const auto height = static_cast<unsigned int>(image.rows);

    return Cool::Texture{{width, height}, 3, reinterpret_cast<uint8_t*>(image.ptr()), {.interpolation_mode = glpp::Interpolation::NearestNeighbour}}; // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)

    // return Cool::Texture{{width, height}, 3, reinterpret_cast<uint8_t*>(image.data)}; // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)

} // namespace Lab

} // namespace Lab
