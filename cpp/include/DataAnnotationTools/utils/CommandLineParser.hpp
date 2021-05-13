//
// Created by brucknem on 09.03.21.
//

#ifndef DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H
#define DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H

#include <boost/program_options.hpp>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <utility>

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
        inline bool FileExists(const std::string &name) {
            return std::ifstream{name.c_str()}.good();
        }

        /**
         * Base class for command line options parsing.
         */
        class CommandLineParser {
        private:

            /**
             * The boost options description.
             */
            po::options_description description;

            /**
             * The boost variables map.
             */
            po::variables_map options;

            /**
             * Flag if the output file should be overridden.
             */
            bool overrideOutputFile = true;

            /**
             * Creates the default options used in the data annotation tools.
             */
            void createDefaultOptions();

        public:

            /**
             * @constructor
             *
             * @param toolname The name of the tool
             */
            explicit CommandLineParser(const std::string &toolname);;

            /**
             * Parses the input options and checks for the default values.
             *
             * @param description The argument options description.
             * @param argc The number of options.
             * @param argv The options.
             */
            void parse(int argc, const char **argv);

            /**
             * Checks if the given key is in the parsed command line options.
             *
             * @param key The name of the key.
             *
             * @return true if there was a command line option given with the key, false else.
             */
            bool has(const std::string &key);

            /**
             * Gets the command line option value of the given key.
             *
             * @tparam T The type of the value.
             * @param key The key of the command line option.
             *
             * @return The command line option value.
             */
            template<typename T>
            T get(const std::string &key);

            /**
             * Easy adding of additional options.
             *
             * @return The options description adding struct.
             */
            po::options_description_easy_init addOption();

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

#endif //DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H
