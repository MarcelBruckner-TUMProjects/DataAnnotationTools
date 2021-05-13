//
// Created by brucknem on 09.03.21.
//

#ifndef DATAANNOTATIONTOOLS_FORMATTER_HPP
#define DATAANNOTATIONTOOLS_FORMATTER_HPP

#include <string>
#include <vector>

#include "opencv2/opencv.hpp"
#include "Watersheder.hpp"

namespace data_annotation_tools {
    namespace watersheder {

        /**
         * Scans the watershed regions for the biggest component, i.e. the background.
         *
         * @param watershedRegions The resulting regions of the watersheder.
         * @param componentCount The number of components.
         *
         * @return The component id of the biggest component.
         */
        int getBiggestComponent(const cv::Mat &watershedRegions, int componentCount);

        /**
         * Converts the watershed regions to a YAML compliant string.
         *
         * @param watershedRegions The resulting regions of the watersheder.
         * @param componentCount The number of components.
         * @param keepBiggestComponent Flag to keep the biggest component.
         *                              Set to false for faster and memory efficient usage.
         *
         * @return The YAML compliant representation of the watershed regions.
         */
        std::string toYAML(const cv::Mat &watershedRegions, int componentCount, bool keepBiggestComponent =
        false);
    }
}

#endif //DATAANNOTATIONTOOLS_FORMATTER_HPP
