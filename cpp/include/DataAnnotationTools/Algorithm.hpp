//
// Created by brucknem on 16.08.21.
//

#ifndef DATAANNOTATIONTOOLS_ALGORITHM_HPP
#define DATAANNOTATIONTOOLS_ALGORITHM_HPP

#include <opencv2/opencv.hpp>

namespace data_annotation_tools {
    class Algorithm {
    protected:

        /**
         * Flag indicating whether deletion mode is on.
         */
        bool isDeleteModeOn = false;
        cv::Mat frame;
        cv::Mat grayscale;

    public:

        virtual void createTrackbars(const std::string &windowName) = 0;

        virtual void setTrackbarValues(const std::string &windowName) = 0;

        virtual bool run() = 0;

        virtual void initialize(const cv::Mat &frame, const cv::Mat &grayscale) = 0;

        virtual void clear(const cv::Rect &roi) = 0;

        virtual cv::Mat draw() = 0;

        virtual void save(const std::string &outputFilename) = 0;

        virtual void onMouse(int event, int x, int y, int flags) = 0;

        virtual void onKeyboard(char c) = 0;

        void toggleDeleteMode() {
            isDeleteModeOn = !isDeleteModeOn;
        }
    };
}


#endif //DATAANNOTATIONTOOLS_ALGORITHM_HPP
