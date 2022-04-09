//
// Created by brucknem on 16.08.21.
//

#include "DataAnnotationTools/Gui.hpp"

namespace data_annotation_tools {

    int Gui::getMaxX() const {
        return image.cols - 1;
    }

    int Gui::getMaxY() const {
        return image.rows - 1;
    }

    cv::Rect Gui::getRoi() const {
        int width = std::max(20, getZoomWidth() - 1);
        int height = std::max(20, getZoomHeight() - 1);

        int posX = std::min(topLeftCorner.x, getMaxX() - width + 1);
        int posY = std::min(topLeftCorner.y, getMaxY() - height + 1);

        auto roi = cv::Rect{posX, posY, width, height};

        //            std::cout << roi << std::endl;
        return roi;
    }

    int Gui::getZoomWidth() const {
        return (int) (image.cols * (zoomLevel / 100.));
    }

    int Gui::getZoomHeight() const {
        return (int) (image.rows * (zoomLevel / 100.));
    }

    void Gui::quickZoom(char c) {
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

    void Gui::basicCommands(char c) {
        if (c == 'd') {
            algorithm->toggleDeleteMode();
        }
        if (c == 'c') {
            algorithm->clear(getRoi());
        }
        if (c == 's') {
            algorithm->save(outputFilename);
        }
    }

    Algorithm *Gui::getAlgorithm() const {
        return algorithm;
    }

    Gui::Gui(std::string inputFilename, std::string outputFilename, data_annotation_tools::Algorithm *algorithm) :
            inputFilename(std::move(inputFilename)),
            outputFilename(std::move(outputFilename)),
            algorithm(algorithm) {
        image = cv::imread(this->inputFilename);
        cv::namedWindow(mainWindowName, cv::WINDOW_NORMAL);

        cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
        cv::cvtColor(imageGray, imageGray, cv::COLOR_GRAY2BGR);

        cv::setMouseCallback(mainWindowName, onMouse, this);
        cv::createTrackbar(posXName, mainWindowName, &topLeftCorner.x, getMaxX());
        cv::createTrackbar(posYName, mainWindowName, &topLeftCorner.y, getMaxY());

        cv::createTrackbar(zoomLevelName, mainWindowName, &zoomLevel, 100);
        cv::createTrackbar(quickZoomLevelName, mainWindowName, &quickZoomLevel, 100);

        algorithm->initialize(image, imageGray);
        algorithm->createTrackbars(mainWindowName);
    }

    void Gui::setTrackbarValues() {
        cv::setTrackbarPos(posXName, mainWindowName, topLeftCorner.x);
        cv::setTrackbarPos(posYName, mainWindowName, topLeftCorner.y);
        cv::setTrackbarPos(zoomLevelName, mainWindowName, zoomLevel);
        algorithm->setTrackbarValues(mainWindowName);
    }

    void Gui::run() {
        for (;;) {
            char c = (char) cv::waitKey(1);
            if (c == 'q') {
                break;
            }

            quickZoom(c);
            basicCommands(c);
            algorithm->onKeyboard(c);

            if (c == 'w' || c == ' ') {
                bool success = algorithm->run();
            }

            setTrackbarValues();

            auto result = algorithm->draw();
            cv::imshow(mainWindowName, result(getRoi()));
        }
    }

    void Gui::onMouse(int event, int x, int y, int flags, void *guiPtr) {
        auto gui = (Gui *) guiPtr;

        auto roi = gui->getRoi();
        int totalX = roi.x + x;
        int totalY = roi.y + y;

        if (totalX < 0 || totalX >= gui->getMaxX() || totalY < 0 || y >= gui->getMaxY()) {
            return;
        }
        gui->hoverPoint = cv::Point{totalX, totalY};
        gui->getAlgorithm()->onMouse(event, totalX, totalY, flags);
    }
}