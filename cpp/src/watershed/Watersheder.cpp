//
// Created by brucknem on 09.02.21.
//

#include "DataAnnotationTools/watershed/Watersheder.hpp"

#include <utility>
#include "yaml-cpp/yaml.h"
#include "DataAnnotationTools/watershed/Formatter.hpp"

namespace data_annotation_tools {
    namespace watersheder {
        Watersheder::Watersheder(bool keepBiggestComponent) : keepBiggestComponent(keepBiggestComponent) {}

        void Watersheder::save(const std::string &outputFilename) {
            std::ofstream outputFile;
            outputFile.open(outputFilename);
            outputFile << data_annotation_tools::watersheder::toYAML(watershedRegions, componentCount,
                                                                     keepBiggestComponent);
            outputFile.close();

            auto tmpDraw = drawWatershedMask;
            drawWatershedMask = 4;
            cv::imwrite(outputFilename + ".png", draw());
            drawWatershedMask = tmpDraw;
        }

        int Watersheder::findMarkerContours() {
            cv::Mat markerMask;
            cv::cvtColor(drawnMarkers, markerMask, cv::COLOR_BGR2GRAY);
            findContours(markerMask, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
            if (contours.empty()) {
                return 0;
            }

            int compCount = 0;
            watershedRegions = {drawnMarkers.size(), CV_32S};
            watershedRegions = cv::Scalar::all(0);
            int idx = 0;
            for (; idx >= 0; idx = hierarchy[idx][0], compCount++) {
                drawContours(watershedRegions, contours, idx, cv::Scalar::all(compCount + 1), -1, 8, hierarchy,
                             INT_MAX);
            }

            return compCount;
        }

        void Watersheder::reset() {
            watershedMask = cv::Mat::zeros(drawnMarkers.size(), CV_8UC3);
            markerIds = cv::Mat::zeros(drawnMarkers.size(), CV_8UC3);
        }

        void Watersheder::createWatershedMask() {
            std::vector<cv::Vec3b> colorTab = generateRandomColors(componentCount);
            watershedMask = cv::Mat(watershedRegions.size(), CV_8UC3);
            markerIds = cv::Mat::zeros(watershedRegions.size(), CV_8UC3);

            std::set<int> alreadyWrittenMarkerIds;
            // paint the watershed image
            for (int i = 0; i < watershedRegions.rows; i++) {
                for (int j = 0; j < watershedRegions.cols; j++) {
                    int index = watershedRegions.at<int>(i, j);
                    if (alreadyWrittenMarkerIds.find(index) == alreadyWrittenMarkerIds.end()) {
                        if (index != -1) {
                            cv::putText(markerIds, std::to_string(index), {j, i + 24}, cv::FONT_HERSHEY_SIMPLEX, 1,
                                        {255, 255, 255}, 1, cv::LINE_AA);
                        }
                        alreadyWrittenMarkerIds.emplace(index);
                    }
                    if (index == -1) {
                        watershedMask.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
                    } else if (index <= 0 || index > componentCount) {
                        watershedMask.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
                    } else {
                        watershedMask.at<cv::Vec3b>(i, j) = colorTab[index - 1];
                    }
                }
            }
        }

        std::vector<cv::Vec3b> Watersheder::generateRandomColors(int amount) {
            std::vector<cv::Vec3b> colorTab;
            for (size_t i = colorTab.size(); i < amount; i++) {
                int b = cv::theRNG().uniform(0, 255);
                int g = cv::theRNG().uniform(0, 255);
                int r = cv::theRNG().uniform(0, 255);
                colorTab.emplace_back((uchar) b, (uchar) g, (uchar) r);
            }
            return colorTab;
        }


        bool Watersheder::run() {
            reset();
            componentCount = findMarkerContours();
            if (componentCount <= 0) {
                return false;
            }

            watershed(frame, watershedRegions);
            createWatershedMask();
            return true;
        }

        cv::Mat Watersheder::draw() {
            cv::Mat result;

            switch (drawWatershedMask) {
                case 0:
                    result = frame;
                    break;
                case 1:
                    result = frame + drawnMarkers;
                    break;
                case 2:
                    result = (watershedMask * 0.5 + grayscale * 0.5);
                    result = result + drawnMarkers;
                    break;
                case 3:
                    result = (watershedMask * 0.5 + grayscale * 0.5);
                    result = result + drawnMarkers + markerIds;
                    break;
                default:
                    result = (watershedMask * 0.5 + grayscale * 0.5);
                    result = result + markerIds;
                    break;
            }
            return result.clone();
        }

        void Watersheder::clear(const cv::Rect &roi) {
            drawnMarkers(roi) = cv::Scalar::all(0);
        }

        void Watersheder::onMouse(int event, int x, int y, int flags) {

            if (event == cv::EVENT_LBUTTONUP || !(flags & cv::EVENT_FLAG_LBUTTON)) {
                /// End of drawing
                drawingPointBuffer = cv::Point(-1, -1);
            } else if (event == cv::EVENT_LBUTTONDOWN) {
                /// Start of drawing
                drawingPointBuffer = cv::Point(x, y);
            } else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON)) {
                /// Drawing
                if (thickness <= 0) {
                    return;
                }
                cv::Point pt(x, y);
                if (drawingPointBuffer.x < 0) {
                    drawingPointBuffer = pt;
                }
                cv::Scalar result = cv::Scalar::all(255);
                if (isDeleteModeOn) {
                    result = cv::Scalar::all(0);
                }
                cv::line(drawnMarkers, drawingPointBuffer, pt, result, thickness, 8, 0);
                drawingPointBuffer = pt;
            }
        }

        void Watersheder::onKeyboard(char c) {
            if (c >= '0' && c <= '9') {
                thickness = (int) (c - '0');
            }

            if (c == 'r') {
                drawWatershedMask++;
                drawWatershedMask %= 5;
            }
        }

        void Watersheder::createTrackbars(const std::string &windowName) {
            cv::createTrackbar(thicknessName, windowName, &thickness, 9);
        }

        void Watersheder::setTrackbarValues(const std::string &windowName) {
            cv::setTrackbarPos(thicknessName, windowName, thickness);
        }

        void Watersheder::initialize(const cv::Mat &frame, const cv::Mat &grayscale) {
            this->frame = frame;
            this->grayscale = grayscale;
            drawnMarkers = cv::Mat(frame.size(), CV_8UC3, cv::Scalar::all(0));
            watershedMask = cv::Mat(frame.size(), CV_8UC3, cv::Scalar::all(0));
        }
    }
}