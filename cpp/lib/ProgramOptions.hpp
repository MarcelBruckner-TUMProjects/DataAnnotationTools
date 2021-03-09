//
// Created by brucknem on 09.03.21.
//

#ifndef DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H
#define DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H

#include <boost/program_options.hpp>
#include <string>
#include <cstdlib>
#include <iostream>

namespace po = boost::program_options;

namespace dataannotationtools {

	/**
	 * Creates the default arguments used in the data annotation tools.
	 */
	po::options_description GetDefaultProgramOptions(std::string toolname) {
		po::options_description desc(toolname);
		desc.add_options()
				("help,h", "Show this help message.")
				("input,i",
				 po::value<std::string>()->default_value("../misc/test_frame.png"),
				 "The path to the input file.")
				("output,o",
				 po::value<std::string>()->default_value("./result.yaml"),
				 "The path to the output file.");
		return desc;
	}

	/**
	 * Parses the input arguments and checks for the default values.
	 *
	 * @param description The argument options description.
	 * @param argc The number of arguments.
	 * @param argv The arguments.
	 */
	po::variables_map ParseOptions(const po::options_description &description, int argc, const char **argv) {
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, description), vm);
		po::notify(vm);

		if (vm.count("help") || vm.count("h")) {
			std::cout << description << std::endl;
			throw std::logic_error("Leaving gracefully, nothing to worry! :)");
		}

		return vm;
	}
}

#endif //DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H
