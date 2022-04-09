//
// Created by brucknem on 13.01.21.
//
#include "DataAnnotationTools/watershed/Watersheder.hpp"
#include "DataAnnotationTools/Gui.hpp"
#include <DataAnnotationTools/watershed/CommandLineParser.hpp>

/**
 * Run the watersheder.
g */
int main(int argc, char const *argv[]) {
    auto parser = data_annotation_tools::watersheder::utils::CommandLineParser("Watersheder");
    parser.init();
    auto programOptions = parser.parse(argc, argv);

    auto watersheder = data_annotation_tools::watersheder::Watersheder(programOptions->keepBiggestKomponent);
    auto gui = data_annotation_tools::Gui(programOptions->inputFilename, programOptions->outputFilename, &watersheder);
    delete programOptions;

    gui.run();
    return 0;
}
