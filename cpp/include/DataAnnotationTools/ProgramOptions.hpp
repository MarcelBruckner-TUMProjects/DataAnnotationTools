//
// Created by brucknem on 29.06.21.
//

#ifndef DATAANNOTATIONTOOLS_PROGRAMOPTIONS_HPP
#define DATAANNOTATIONTOOLS_PROGRAMOPTIONS_HPP

#include <string>
#include <utility>

namespace data_annotation_tools {
    namespace utils {
        class ProgramOptions {
        public:
            std::string inputFilename;
            std::string outputFilename;

            ProgramOptions(std::string inputFilename, std::string outputFilename) : inputFilename(std::move(
                    inputFilename)), outputFilename(std::move(outputFilename)) {}

            explicit ProgramOptions(const ProgramOptions *programOptions) {
                inputFilename = programOptions->inputFilename;
                outputFilename = programOptions->outputFilename;
            }
        };
    }
}

#endif //DATAANNOTATIONTOOLS_PROGRAMOPTIONS_HPP
