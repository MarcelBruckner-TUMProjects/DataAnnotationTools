//
// Created by brucknem on 09.03.21.
//

#ifndef DATAANNOTATIONTOOLS_WATERSHED_COMMAND_LINE_PARSER_H
#define DATAANNOTATIONTOOLS_WATERSHED_COMMAND_LINE_PARSER_H

#include <boost/program_options.hpp>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <utility>

#include "DataAnnotationTools/ProgramOptions.hpp"
#include "DataAnnotationTools/CommandLineParser.hpp"

namespace po = boost::program_options;

namespace data_annotation_tools {
    namespace watersheder {
        namespace utils {
            class ProgramOptions : public data_annotation_tools::utils::ProgramOptions {
            public:
                bool keepBiggestKomponent;

                ProgramOptions(data_annotation_tools::utils::ProgramOptions *programOptions, bool keepBiggestKomponent)
                        : data_annotation_tools::utils::ProgramOptions(programOptions),
                          keepBiggestKomponent(keepBiggestKomponent) {}
            };

            /**
             * Base class for command line options parsing.
             */
            class CommandLineParser : data_annotation_tools::utils::CommandLineParser {

            public:
                /**
                 * Creates the options used in the watersheder.
                 */
                void init() override;

                /**
                 * @constructor
                 */
                explicit CommandLineParser(const std::string &toolname);

                /**
                 * @destructor
                 */
                ~CommandLineParser() override = default;

                /**
                 * Parses the input options and checks for the default values.
                 *
                 * @param description The argument options description.
                 * @param argc The number of options.
                 * @param argv The options.
                 */
                ProgramOptions *parse(int argc, const char **argv);
            };
        }
    }
}

#endif //DATAANNOTATIONTOOLS_WATERSHED_COMMAND_LINE_PARSER_H
