//
// Created by brucknem on 09.02.21.
//

#include "DataAnnotationTools/watershed/Watersheder.hpp"

#include <utility>
#include "yaml-cpp/yaml.h"
#include "DataAnnotationTools/watershed/Formatter.hpp"

namespace data_annotation_tools {
    namespace watersheder {

        int Watersheder::getMaxX() const {
            return image.cols - 1;
        }

        int Watersheder::getMaxY() const {
            return image.rows - 1;
        }

        void Watersheder::onMouse(int event, int x, int y, int flags, void *rawWatersheder) {
            auto watersheder = (Watersheder *) rawWatersheder;
            int totalX = watersheder->topLeftCorner.x + x;
            int totalY = watersheder->topLeftCorner.y + y;
            if (totalX < 0 || totalX >= watersheder->getMaxX() || totalY < 0 || y >= watersheder->getMaxY()) {
                return;
            }
            watersheder->hoverPoint = cv::Point{totalX, totalY};
            if (event == cv::EVENT_LBUTTONUP || !(flags & cv::EVENT_FLAG_LBUTTON)) {
                /// End of drawing
                watersheder->drawingPointBuffer = cv::Point(-1, -1);
            } else if (event == cv::EVENT_LBUTTONDOWN) {
                /// Start of drawing
                watersheder->drawingPointBuffer = cv::Point(totalX, totalY);
            } else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON)) {
                /// Drawing
                if (watersheder->thickness <= 0) {
                    return;
                }
                cv::Point pt(totalX, totalY);
                if (watersheder->drawingPointBuffer.x < 0) {
                    watersheder->drawingPointBuffer = pt;
                }
                cv::Scalar result = cv::Scalar::all(255);
                if (watersheder->isDeleteModeOn) {
                    result = cv::Scalar::all(0);
                }
                cv::line(watersheder->drawnMarkers, watersheder->drawingPointBuffer, pt, result, watersheder->thickness,
                         8, 0);
                watersheder->drawingPointBuffer = pt;
            }
        }

        Watersheder::Watersheder(const std::string &inputFilename, std::string outputFilename,
                                 bool keepBiggestComponent) : inputFilename(inputFilename), outputFilename
                (std::move(outputFilename)), keepBiggestComponent(keepBiggestComponent) {
            image = cv::imread(inputFilename);
            cv::namedWindow(mainWindowName, cv::WINDOW_NORMAL);

            cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
            cv::cvtColor(imageGray, imageGray, cv::COLOR_GRAY2BGR);
            drawnMarkers = cv::Mat(image.size(), CV_8UC3, cv::Scalar::all(0));
            watershedMask = cv::Mat(image.size(), CV_8UC3, cv::Scalar::all(0));

            cv::setMouseCallback(mainWindowName, onMouse, this);
            cv::createTrackbar(posXName, mainWindowName, &topLeftCorner.x, getMaxX());
            cv::createTrackbar(posYName, mainWindowName, &topLeftCorner.y, getMaxY());

            cv::createTrackbar(zoomLevelName, mainWindowName, &zoomLevel, 100);
            cv::createTrackbar(quickZoomLevelName, mainWindowName, &quickZoomLevel, 100);
            cv::createTrackbar(thicknessName, mainWindowName, &thickness, 9);

            setTrackbarValues();
        }

        void Watersheder::run() {
            for (;;) {
                char c = (char) cv::waitKey(1);
                if (c == 'q') {
                    break;
                }

                basicCommands(c);
                quickZoom(c);
                setThickness(c);
                performAlgorithm(c);

                setTrackbarValues();

                auto frame = draw();
                cv::imshow(mainWindowName, frame);
            }
        }

        void Watersheder::basicCommands(char c) {
            if (c == 'r') {
                drawWatershedMask = !drawWatershedMask;
            }
            if (c == 'd') {
                isDeleteModeOn = !isDeleteModeOn;
            }
            if (c == 'c') {
                drawnMarkers(getRoi()) = cv::Scalar::all(0);
            }
            if (c == 's') {
                save();
            }
        }

        void Watersheder::save() {
            std::ofstream outputFile;
            outputFile.open(outputFilename);
            outputFile
                    << data_annotation_tools::watersheder::toYAML(watershedRegions, componentCount,
                                                                  keepBiggestComponent);
            outputFile.close();

            cv::imwrite(outputFilename + ".png", draw());
        }

        void Watersheder::quickZoom(char c) {
            if (c == 'n') {
                zoomLevel = quickZoomLevel;
                topLeftCorner.x = std::max(0, hoverPoint.x - getZoomWidth() / 2);
                topLeftCorner.y = std::max(0, hoverPoint.y - getZoomHeight() / 2);
            }
            if (c == 'b') {
                topLeftCorner.x = 0;
                topLeftCorner.y = 0;
                zoomLevel = 100;
            }
        }

        void Watersheder::setThickness(char c) {
            if (c >= '0' && c <= '9') {
                thickness = (int) (c - '0');
            }
        }

        void Watersheder::performAlgorithm(char c) {
            if (c == 'w' || c == ' ') {
                drawWatershedMask = algorithm();
            }
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

        bool Watersheder::algorithm() {
            componentCount = findMarkerContours();
            if (componentCount <= 0) {
                return false;
            }

            watershed(image, watershedRegions);
            createWatershedMask();
            return true;
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
            for (int i = 0; i < amount; i++) {
                int b = cv::theRNG().uniform(0, 255);
                int g = cv::theRNG().uniform(0, 255);
                int r = cv::theRNG().uniform(0, 255);
                colorTab.emplace_back((uchar) b, (uchar) g, (uchar) r);
            }
            return colorTab;
        }

        void Watersheder::setTrackbarValues() {
            cv::setTrackbarPos(posXName, mainWindowName, topLeftCorner.x);
            cv::setTrackbarPos(posYName, mainWindowName, topLeftCorner.y);
            cv::setTrackbarPos(zoomLevelName, mainWindowName, zoomLevel);
            cv::setTrackbarPos(thicknessName, mainWindowName, thickness);
        }

        cv::Mat Watersheder::draw() const {
            cv::Mat result;

            if (!drawWatershedMask) {
                result = image;
            } else {
                result = (watershedMask * 0.5 + imageGray * 0.5);
            }
            auto roi = getRoi();
            result = (result + drawnMarkers + markerIds)(roi);
            return result.clone();
        }

        cv::Rect Watersheder::getRoi() const {
            int width = std::max(20, getZoomWidth());
            int height = std::max(20, getZoomHeight());

            int posX = std::min(topLeftCorner.x, getMaxX() - width + 1);
            int posY = std::min(topLeftCorner.y, getMaxY() - height + 1);

            auto roi = cv::Rect{posX, posY, width, height};

            std::cout << roi << std::endl;
            return roi;
        }

        int Watersheder::getZoomWidth() const {
            return (int) (image.cols * (zoomLevel / 100.));
        }

        int Watersheder::getZoomHeight() const {
            return (int) (image.rows * (zoomLevel / 100.));
        }

        double Watersheder::getAspect() const {
            return image.cols * 1. / image.rows;
        }
    }
}