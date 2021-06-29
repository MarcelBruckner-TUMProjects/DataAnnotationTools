//
// Created by brucknem on 09.03.21.
//

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "DataAnnotationTools/watershed/CommandLineParser.hpp"

namespace data_annotation_tools {
    namespace watersheder {
        namespace utils {
            const std::string KEEP_BIGGEST_COMPONENT_OPTION_NAME = "keep_biggest_component";

            bool keepBiggestComponent;


            ProgramOptions *CommandLineParser::parse(int argc, const char **argv) {
                auto programOptions = data_annotation_tools::utils::CommandLineParser::parse(argc, argv);
                po::store(po::parse_command_line(argc, argv, description), options);
                po::notify(options);
                return new ProgramOptions{
                        programOptions,
                        keepBiggestComponent
                };
            }

            void CommandLineParser::init() {
                data_annotation_tools::utils::CommandLineParser::init();

                description.add_options()
                        ((KEEP_BIGGEST_COMPONENT_OPTION_NAME + ",k").c_str(), po::bool_switch(&keepBiggestComponent),
                         "Flag to keep the biggest component when writing to the result file. The biggest component, i.e. the background, is by default not written to minimize file write options.");
            }

            CommandLineParser::CommandLineParser(const std::string &toolname)
                    : data_annotation_tools::utils::CommandLineParser(toolname) {}
        }
    }
}