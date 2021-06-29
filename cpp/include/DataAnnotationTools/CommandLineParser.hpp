//
// Created by brucknem on 09.03.21.
//

#ifndef DATAANNOTATIONTOOLS_COMMAND_LINE_PARSER_H
#define DATAANNOTATIONTOOLS_COMMAND_LINE_PARSER_H

#include <boost/program_options.hpp>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <utility>
#include "DataAnnotationTools/ProgramOptions.hpp"

namespace po = boost::program_options;

namespace data_annotation_tools {
    namespace utils {

        /**
         * Checks if the file with the given name exists.
         *
         * @param name The filename.
         *
         * @return true if file exists, false else.
         */
        inline bool fileExists(const std::string &name) {
            return std::ifstream{name.c_str()}.good();
        }

        /**
         * Base class for command line options parsing.
         */
        class CommandLineParser {
        protected:

            /**
             * The boost options description.
             */
            po::options_description description;

            /**
             * The boost variables map.
             */
            po::variables_map options;

        public:
            /**
             * Creates the options used in the watersheder.
             */
            virtual void init();

            /**
             * @constructor
             *
             * @param toolname The name of the tool
             */
            explicit CommandLineParser(const std::string &toolname);

            /**
             * @destructor
             */
            virtual ~CommandLineParser() = default;

            virtual /**
             * Parses the input options and checks for the default values.
             *
             * @param description The argument options description.
             * @param argc The number of options.
             * @param argv The options.
             */
            ProgramOptions *parse(int argc, const char **argv);

            /**
             * Output stream operator.
             *
             * @param os The stream.
             * @param options The object.
             *
             * @return The stream.
             */
            friend std::ostream &operator<<(std::ostream &os, const CommandLineParser &options);

        };
    }
}

#endif //DATAANNOTATIONTOOLS_COMMAND_LINE_PARSER_H
