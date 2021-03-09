//
// Created by brucknem on 13.01.21.
//
#include "Watersheder.hpp"
#include <boost/program_options.hpp>
#include <ProgramOptions.hpp>

namespace po = boost::program_options;

/**
 * Run the watersheder.
g */
int main(int argc, char const *argv[]) {
	auto programOptions = dataannotationtools::ProgramOptions("Watersheder");
	bool keepBiggestComponent;
	programOptions.addOption()
			("keep_biggest_component,k",
			 po::bool_switch(&keepBiggestComponent),
			 "Flag to keep the biggest component when writing to the result file."
			);
	programOptions.parse(argc, argv);

	auto watersheder = dataannotationtools::Watersheder(
			programOptions.get<std::string>("input"),
			programOptions.get<std::string>("output"),
			keepBiggestComponent);
	watersheder.run();

	return 0;
}
