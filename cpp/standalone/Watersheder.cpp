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
	po::options_description description = dataannotationtools::GetDefaultProgramOptions("Watersheder");
	auto variableMap = dataannotationtools::ParseOptions(description, argc, argv);

	auto watersheder = dataannotationtools::Watersheder(
			variableMap["input"].as<std::string>(),
			variableMap["output"].as<std::string>()
	);
	watersheder.run();

	return 0;
}
