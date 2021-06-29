//
// Created by brucknem on 13.01.21.
//
#include "DataAnnotationTools/watershed/Watersheder.hpp"
#include <boost/program_options.hpp>
#include <DataAnnotationTools/watershed/CommandLineParser.hpp>

namespace po = boost::program_options;

/**
 * Run the watersheder.
g */
int main(int argc, char const *argv[]) {
    auto parser = data_annotation_tools::watersheder::utils::CommandLineParser("Watersheder");
    parser.init();
    auto programOptions = parser.parse(argc, argv);

    auto watersheder = data_annotation_tools::watersheder::Watersheder(
            programOptions->inputFilename,
            programOptions->outputFilename,
            programOptions->keepBiggestKomponent);
    delete programOptions;

    watersheder.run();

    return 0;
}
