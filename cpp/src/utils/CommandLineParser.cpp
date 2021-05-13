//
// Created by brucknem on 09.03.21.
//

#include "DataAnnotationTools/utils/CommandLineParser.hpp"

namespace data_annotation_tools {
    namespace utils {
        template<typename T>
        T CommandLineParser::get(const std::string &key) {
            return options[key].template as<T>();
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

            if (has("help")) {
                std::cout << description << std::endl;
                if (has("debug_parser")) {
                    exit(EXIT_SUCCESS);
                } else {
                    return;
                }
            }

            if (!FileExists(get<std::string>("input"))) {
                std::cout << "Input file does not exist." << std::endl;
                exit(EXIT_FAILURE);
            }

            if (FileExists(get<std::string>("output")) && !overrideOutputFile) {
                std::cout << "Output file does exist and override flag not set." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // TODO descriptions
        void CommandLineParser::createDefaultOptions() {
            description.add_options()
                    ("help,h", "Show this help message.")
                    ("input,i",
                     po::value<std::string>()->default_value("../misc/test_frame.png"),
                     "The path to the input file.")
                    ("output,o",
                     po::value<std::string>()->default_value("./result.yaml"),
                     "The path to the output file.")
                    ("override,r",
                     po::bool_switch(&overrideOutputFile),
                     "The path to the output file.")
                    ("debug_parser",
                     po::bool_switch(&debugParser),
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