//
// Created by brucknem on 29.06.21.
//


#include <DataAnnotationTools/watershed/CommandLineParser.hpp>
#include <opencv2/opencv.hpp>
#include <DataAnnotationTools/lane_detection/LaneDetection.hpp>

int main(int argc, char const *argv[]) {
    auto parser = data_annotation_tools::utils::CommandLineParser("Watersheder");
    parser.init();
    auto programOptions = parser.parse(argc, argv);

    auto inputImage = cv::imread(programOptions->inputFilename);

    int epsilon = 6;
    int epsilonMax = 100;
    int shapeLower = 2;
    int shapeUpper = 4;

    int cannyLower = 50;
    int cannyLowerMax = cannyLower * 4;
    int cannyUpper = 100;
    int cannyUpperMax = cannyUpper * 4;

    int withLines = 0;
    int vanishingPoints = 3;
    int vanishingPointsMax = 20;

    int rho = 100;
    int rhoMax = 200;
    int theta = 180;
    int thetaMax = 360;
    int threshold = 150;
    int thresholdMax = 300;

    int colorMin = 180;
    int colorMinMax = 255;
    int scale = 50;
    int scaleMax = 100;

    // Canny, Sobel, Laplace
    int detector = 0;
    int detectorMax = 2;

    int drawHoughLines = 0;
    int maskTop = 0;
    int maskTopMax = 100;

    std::string winname = "Lane Detection";
    cv::namedWindow(winname);

    cv::createTrackbar("Scale", winname, &scale, scaleMax);
    cv::createTrackbar("Mask Top", winname, &maskTop, maskTopMax);
    cv::createTrackbar("Detector", winname, &detector, detectorMax);
    cv::createTrackbar("Epsilon", winname, &epsilon, epsilonMax);
    cv::createTrackbar("Color Min", winname, &colorMin, colorMinMax);
    cv::createTrackbar("Shape Lower", winname, &shapeLower, 4);
    cv::createTrackbar("Shape Upper", winname, &shapeUpper, 20);
    cv::createTrackbar("Canny Lower", winname, &cannyLower, cannyLowerMax);
    cv::createTrackbar("Canny Upper", winname, &cannyUpper, cannyUpperMax);
    cv::createTrackbar("With Lines", winname, &withLines, 1);
    cv::createTrackbar("Vanishing Points", winname, &vanishingPoints, vanishingPointsMax);

    cv::createTrackbar("Rho", winname, &rho, rhoMax);
    cv::createTrackbar("Theta", winname, &theta, thetaMax);
    cv::createTrackbar("Threshold", winname, &threshold, thresholdMax);
    cv::createTrackbar("Draw Hough Lines", winname, &drawHoughLines, 1);

    while (true) {

        auto result = data_annotation_tools::lane_detection::findContours(inputImage, epsilon * 1. / epsilonMax,
                                                                          shapeLower,
                                                                          shapeUpper, cannyLower, cannyUpper,
                                                                          withLines == 1, vanishingPoints,
                                                                          rho * 2.0 / rhoMax, theta, threshold,
                                                                          colorMin, scale * 1.0 / scaleMax, detector,
                                                                          drawHoughLines, maskTop * 1. / maskTopMax);
        cv::imshow(winname, result);
        cv::waitKey(1);

//        cv::imshow("Original", inputImage);
//        cv::cvtColor(inputImage, result, cv::COLOR_BGR2HLS);
//        cv::imshow("HLS", result);
//        cv::cvtColor(inputImage, result, cv::COLOR_BGR2HSV);
//        cv::imshow("HSV", result);
//        cv::cvtColor(inputImage, result, cv::COLOR_BGR2YUV);
//        cv::imshow("Grayscale", result);

    }
}
