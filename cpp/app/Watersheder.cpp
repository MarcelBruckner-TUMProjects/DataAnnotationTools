//
// Created by brucknem on 13.01.21.
//
#include "DataAnnotationTools/watershed/Watersheder.hpp"
#include <boost/program_options.hpp>
#include <DataAnnotationTools/utils/CommandLineParser.hpp>

namespace po = boost::program_options;

/**
 * Run the watersheder.
g */
int main(int argc, char const *argv[]) {
    auto programOptions = data_annotation_tools::utils::CommandLineParser("Watersheder");
    bool keepBiggestComponent;
    programOptions.addOption()
            ("keep_biggest_component,k",
             po::bool_switch(&keepBiggestComponent),
             "Flag to keep the biggest component when writing to the result file."
            );
    try {
        programOptions.parse(argc, argv);
    } catch (const std::invalid_argument &exception) {
        return 0;
    } catch (const std::logic_error &exception) {
        return 0;
    }

    auto watersheder = data_annotation_tools::watersheder::Watersheder(
            programOptions.get<std::string>("input"),
            programOptions.get<std::string>("output"),
            keepBiggestComponent);
    watersheder.run();

    return 0;
}
