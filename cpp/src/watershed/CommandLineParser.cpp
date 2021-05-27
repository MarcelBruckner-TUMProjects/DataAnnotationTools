//
// Created by brucknem on 09.03.21.
//

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "DataAnnotationTools/watershed/CommandLineParser.hpp"

namespace data_annotation_tools {
    namespace utils {
        const std::string HELP_OPTION_NAME = "help";
        const std::string INPUT_OPTION_NAME = "input";
        const std::string OUTPUT_OPTION_NAME = "output";
        const std::string OVERRIDE_OUTPUT_OPTION_NAME = "override_output";
        const std::string DEBUG_PARSER_OPTION_NAME = "debug_parser";
        const std::string KEEP_BIGGEST_COMPONENT_OPTION_NAME = "keep_biggest_component";

        bool keepBiggestComponent;

        /**
         * Flag if the output file should be overridden.
         */
        bool overrideOutputFile = true;

        /**
         * Flag if the parser is in debug mode.
         */
        bool debugParser = true;

        CommandLineParser::CommandLineParser(const std::string &toolname) :
                description(toolname) {
            createOptions();
        }

        ProgramOptions CommandLineParser::parse(int argc, const char **argv) {
            po::store(po::parse_command_line(argc, argv, description), options);
            po::notify(options);

            if (options.count(HELP_OPTION_NAME) != 0) {
                std::cout << description << std::endl;
                if (options.count(DEBUG_PARSER_OPTION_NAME) != 0) {
                    exit(EXIT_SUCCESS);
                } else {
                    return {};
                }
            }

            auto inputFilename = options[INPUT_OPTION_NAME].as<std::string>();
            if (!fileExists(inputFilename)) {
                std::cout << "Input file does not exist." << std::endl;
                exit(EXIT_FAILURE);
            }

            auto outfileName = options[OUTPUT_OPTION_NAME].as<std::string>();
            if (outfileName.empty()) {
                outfileName = inputFilename + "_result.yaml";
            }
            if (fileExists(outfileName) && !overrideOutputFile) {
                std::cout << "Output file does exist and override flag not set." << std::endl;
                exit(EXIT_FAILURE);
            }
            boost::filesystem::path outfilePath(outfileName);
            if (!boost::filesystem::exists(outfilePath.parent_path())) {
                boost::filesystem::create_directories(outfilePath.parent_path());
            }

            return ProgramOptions{
                    inputFilename,
                    outfileName,
                    keepBiggestComponent
            };
        }

        void CommandLineParser::createOptions() {
            description.add_options()
                    ((HELP_OPTION_NAME + ",h").c_str(), "Show this help message.")
                    ((INPUT_OPTION_NAME + ",i").c_str(),
                     po::value<std::string>()->default_value("../misc/test_frame.png"),
                     "The path to the input file. The path can be relative or absolute. The type of the file is tool dependent.")
                    ((OUTPUT_OPTION_NAME + ",o").c_str(),
                     po::value<std::string>()->default_value(""),
                     "The path to the output file. The path can be relative or absolute. The type of the output file is tool dependent.")
                    ((OVERRIDE_OUTPUT_OPTION_NAME + ",r").c_str(),
                     po::bool_switch(&overrideOutputFile),
                     "Flag if the output file should be overridden if it already exists.")
                    ((KEEP_BIGGEST_COMPONENT_OPTION_NAME + ",k").c_str(), po::bool_switch(&keepBiggestComponent),
                     "Flag to keep the biggest component when writing to the result file. The biggest component, i.e. the background, is by default not written to minimize file write options.")(
                    DEBUG_PARSER_OPTION_NAME.c_str(), po::bool_switch(&debugParser),
                    "Flag if the parser is in debug mode.");
        }

        std::ostream &operator<<(std::ostream &os, const CommandLineParser &options) {
            os << options.description;
            return os;
        }
    }
}