//
// Created by brucknem on 09.02.21.
//

#ifndef DATAANNOTATIONTOOLS_WATERSHEDER_HPP
#define DATAANNOTATIONTOOLS_WATERSHEDER_HPP

#include <utility>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "boost/filesystem.hpp"

namespace data_annotation_tools {
    namespace watersheder {

        /**
         * Wrapper for the watershed algorithm to ease the segmentation of objects.
         */
        class Watersheder {
        private:

            /**
             * The input and output filenames.
             */
            std::string inputFilename, outputFilename;

            /**
             * The name of the main window where drawing the watershedRegions is performed.
             */
            std::string mainWindowName = "Watersheder";

            /**
             * The image buffers.
             */
            cv::Mat image, imageGray, drawnMarkers, watershedMask, watershedRegions, markerIds;

            /**
             * The number of found components.
             */
            int componentCount = -1;

            /**
             * The contours.
             */
            std::vector<std::vector<cv::Point> > contours;

            /**
             * The contour hierarchy.
             */
            std::vector<cv::Vec4i> hierarchy;

            /**
             * The point that holds the mouse position used to create the watershedRegions.
             */
            cv::Point drawingPointBuffer{-1, -1};
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

            /**
             * The drawing line thickness.
             */
            int thickness = 2;
            /**
             * The name of the trackbar for the height.
             */
            std::string thicknessName = "Thickness:";

            /**
             * Flag indicating whether deletion mode is on.
             */
            bool isDeleteModeOn = false;

            /**
             * Flag indicating whether to draw the watershed mask and marker ids.
             * 0: draw non
             * 1: only draw mask
             * 2: draw both
             */
            int drawWatershedMask = 2;

            /**
             * Flag if the biggest component should be skipped when writing to file.
             */
            bool keepBiggestComponent = false;

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
             * Callback for mouse events.
             * @param event The event code.
             * @param x Mouse X position.
             * @param y Mouse Y position.
             * @param flags Additional flags.
             * @param _watersheder The watersheder object used to apply the performed mouse action.
             */
            static void onMouse(int event, int x, int y, int flags, void *watersheder);

            /**
             * Zooms in and out of the image base on the mouse position.
             */
            void quickZoom(char c);

            /**
             * Performs basic commands like closing and saving.
             */
            void basicCommands(char c);

            /**
             * Performs the algorithm.
             */
            void performAlgorithm(char c);

            /**
             * Sets the trackbar values programmatically.
             */
            void setTrackbarValues();

            /**
             * Finds the marker contours in the drawn markers.
             * @return The number of found marker contours.
             */
            int findMarkerContours();

            /**
             * Generates some random colors for the marker areas.
             * @param amount The number of colors to generate.
             */
            static std::vector<cv::Vec3b> generateRandomColors(int amount);

            /**
             * Creates the color coded watershed mask.
             * @param regions The number of found distinct regions.
             */
            void createWatershedMask();

            /**
             * Sets the thickness based on the input keyboard number.
             */
            void setThickness(char c);

            /**
             * Gets the fraction of the width after zoom.
             */
            int getZoomWidth() const;

            /**
             * Gets the fraction of the height after zoom.
             */
            int getZoomHeight() const;

            /**
             * Gets the aspect ratio.
             */
            double getAspect() const;

            /**
             * The algorithm implementation.
             */
            bool algorithm();

            void reset();

        public:
            /**
             * @constructor.
             */
            Watersheder(const std::string &inputFilename, std::string outputFilename, bool keepBiggestComponent = true);

            /**
             * @destructor
             */
            virtual ~Watersheder() = default;

            /**
             * Main loop.
             */
            void run();

            /**
             * Renders the image and overlays the watershedRegions.
             */
            cv::Mat draw() const;

            /**
             * Saves the marker pixels to the output file.
             */
            void save();
        };
    }
}
#endif //DATAANNOTATIONTOOLS_WATERSHEDER_HPP
