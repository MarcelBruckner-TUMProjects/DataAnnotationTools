//
// Created by brucknem on 09.03.21.
//

#include "DataAnnotationTools/utils/CommandLineParser.hpp"

namespace data_annotation_tools {
    namespace utils {
        const std::string HELP_OPTION_NAME = "help";
        const std::string INPUT_OPTION_NAME = "input";
        const std::string OUTPUT_OPTION_NAME = "output";
        const std::string OVERRIDE_OUTPUT_OPTION_NAME = "override_output";
        const std::string DEBUG_PARSER_OPTION_NAME = "debug_parser";

        template<typename T>
        T CommandLineParser::get(const std::string &key) {
            return options[key].as<T>();
        }

        bool CommandLineParser::has(const std::string &key) {
            return options.count(key);
        }

        CommandLineParser::CommandLineParser(const std::string &toolname) :
                description(toolname) {
            createDefaultOptions();
        }

        void CommandLineParser::parse(int argc, const char **argv) {
            po::store(po::parse_command_line(argc, argv, description), options);
            po::notify(options);

            if (has(HELP_OPTION_NAME)) {
                std::cout << description << std::endl;
                if (has(DEBUG_PARSER_OPTION_NAME)) {
                    exit(EXIT_SUCCESS);
                } else {
                    return;
                }
            }

            if (!fileExists(get<std::string>(INPUT_OPTION_NAME))) {
                std::cout << "Input file does not exist." << std::endl;
                exit(EXIT_FAILURE);
            }

            if (fileExists(get<std::string>(OUTPUT_OPTION_NAME)) && !overrideOutputFile) {
                std::cout << "Output file does exist and override flag not set." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        void CommandLineParser::createDefaultOptions() {
            description.add_options()
                    ((HELP_OPTION_NAME + ",h").c_str(), "Show this help message.")
                    ((INPUT_OPTION_NAME + ",i").c_str(),
                     po::value<std::string>()->default_value("../misc/test_frame.png"),
                     "The path to the input file. The path can be relative or absolute. The type of the file is tool dependent.")
                    ((OUTPUT_OPTION_NAME + ",o").c_str(),
                     po::value<std::string>()->default_value("./result.yaml"),
                     "The path to the output file. The path can be relative or absolute. The type of the output file is tool dependent.")
                    ((OVERRIDE_OUTPUT_OPTION_NAME + ",r").c_str(),
                     po::bool_switch(&overrideOutputFile),
                     "Flag if the output file should be overridden if it already exists.")
                    (DEBUG_PARSER_OPTION_NAME.c_str(), po::bool_switch(&debugParser),
                     "Flag if the parser is in debug mode.");
        }

        po::options_description_easy_init CommandLineParser::addOption() {
            return description.add_options();
        }

        template std::string CommandLineParser::get<std::string>(const std::string &key);

        std::ostream &operator<<(std::ostream &os, const CommandLineParser &options) {
            os << options.description;
            return os;
        }
    }
}