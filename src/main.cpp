#include "App.h"
//
#include <Cool/Core/run.h> // Must be included last otherwise it slows down compilation because it includes <cereal/archives/json.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

auto main() -> int
{
    // Cool::run<Lab::App>(
    //     {Cool::WindowConfig{
    //         .title                  = "Coollab",
    //         .maximize_on_startup_if = true,
    //     }},
    //     Cool::InitConfig{
    //         .default_texture_path = Cool::compute_root_path() / "res/images/logo.png",
    //     }
    // );

    cv::Mat image;
    cv::namedWindow("webcam");

    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        std::cout << "cannot open camera";
    }

    while (true)
    {
        cap >> image;
        image *= 2;

        cv::Mat img_gray;
        cvtColor(image, img_gray, cv::COLOR_BGR2GRAY);

        cv::imshow("Display window", img_gray);
        cv::waitKey(25);
    }
    return 0;
}