//
// Created by brucknem on 16.08.21.
//

#ifndef DATAANNOTATIONTOOLS_GUI_HPP
#define DATAANNOTATIONTOOLS_GUI_HPP

#include <utility>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "boost/filesystem.hpp"
#include "Algorithm.hpp"

namespace data_annotation_tools {
    class Gui {

        /**
         * The input and output filenames.
         */
        std::string inputFilename, outputFilename;

        /**
         * The name of the main window where drawing the watershedRegions is performed.
         */
        std::string mainWindowName = "DataAnnotationTools";

        /**
         * The image buffers.
         */
        cv::Mat image, imageGray;


        /**
         * The top left corner of the region of interest.
         */
        cv::Point topLeftCorner{0, 0};

        /**
         * The name of the trackbar for the top left corner x position.
         */
        std::string posXName = "Pos X:";
        /**
         * The name of the trackbar for the top left corner y position.
         */
        std::string posYName = "Pos Y:";

        /**
         * The point that holds the mouse position used to quickZoom in and out of the image.
         */
        cv::Point hoverPoint{0, 0};

        /**
         * The quickZoom level.
         */
        int zoomLevel = 100;
        /**
         * The quickZoom level.
         */
        int quickZoomLevel = 10;

        /**
         * The name of the trackbar for the width.
         */
        std::string zoomLevelName = "Zoom:";

        /**
         * The name of the trackbar for the width.
         */
        std::string quickZoomLevelName = "Quick Zoom:";

        data_annotation_tools::Algorithm *algorithm;

        /**
         * @get The number of pixels in x direction.
         */
        int getMaxX() const;

        /**
         * @get The number of pixels in y direction.
         */
        int getMaxY() const;

        /**
         * @get The region of interest used to quickZoom into the image.
         */
        cv::Rect getRoi() const;

        /**
         * Gets the fraction of the width after zoom.
         */
        int getZoomWidth() const;

        /**
         * Gets the fraction of the height after zoom.
         */
        int getZoomHeight() const;

        /**
         * Zooms in and out of the image base on the mouse position.
         */
        void quickZoom(char c);

        /**
         * Performs basic commands like closing and saving.
         */
        void basicCommands(char c);

    public:

        Gui(std::string inputFilename, std::string outputFilename, data_annotation_tools::Algorithm *algorithm);

        void setTrackbarValues();

        void run();

        static void onMouse(int event, int x, int y, int flags, void *guiPtr);

        Algorithm *getAlgorithm() const;

    };
}

#endif //DATAANNOTATIONTOOLS_GUI_HPP
