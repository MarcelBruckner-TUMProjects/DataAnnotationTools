//
// Created by brucknem on 09.02.21.
//

#ifndef DATAANNOTATIONTOOLS_WATERSHEDER_HPP
#define DATAANNOTATIONTOOLS_WATERSHEDER_HPP

#include <utility>
#include <vector>
#include <string>
#include <DataAnnotationTools/Algorithm.hpp>
#include "opencv2/opencv.hpp"
#include "boost/filesystem.hpp"

namespace data_annotation_tools {
    namespace watersheder {

        /**
         * Wrapper for the watershed algorithm to ease the segmentation of objects.
         */
        class Watersheder : public data_annotation_tools::Algorithm {
        private:

            /**
             * The image buffers.
             */
            cv::Mat drawnMarkers, watershedMask, watershedRegions, markerIds;

            /**
             * The point that holds the mouse position used to create the watershedRegions.
             */
            cv::Point drawingPointBuffer{-1, -1};


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
             * The drawing line thickness.
             */
            int thickness = 2;

            /**
             * The name of the trackbar for the height.
             */
            std::string thicknessName = "Thickness:";

            /**
             * Flag indicating whether to draw the watershed mask and marker ids.
             * 0: draw original image
             * 1: draw original image, watershed markers
             * 2: draw original image, watershed markers, watershed mask
             * 3: draw original image, watershed markers, watershed mask, marker ids
             * 4: draw original image, watershed mask, marker ids
             */
            int drawWatershedMask = 3;

            /**
             * Flag if the biggest component should be skipped when writing to file.
             */
            bool keepBiggestComponent = false;

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

            void reset();

        public:
            /**
             * @constructor.
             */
            explicit Watersheder(bool keepBiggestComponent = true);

            /**
             * @destructor
             */
            virtual ~Watersheder() = default;

            /**
             * Main loop.
             */
            bool run() override;

            /**
             * Renders the image and overlays the watershedRegions.
             */
            cv::Mat draw() override;

            /**
             * Saves the marker pixels to the output file.
             */
            void save(const std::string &outputFilename) override;

            void clear(const cv::Rect &roi) override;

            void onMouse(int event, int x, int y, int flags) override;

            void onKeyboard(char c) override;

            void createTrackbars(const std::string &windowName) override;

            void setTrackbarValues(const std::string &windowName) override;

            void initialize(const cv::Mat &frame, const cv::Mat &grayscale) override;
        };
    }
}
#endif //DATAANNOTATIONTOOLS_WATERSHEDER_HPP
