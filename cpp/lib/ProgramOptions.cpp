//
// Created by brucknem on 09.03.21.
//

#include "ProgramOptions.hpp"

namespace dataannotationtools {

	template<typename T>
	T ProgramOptions::get(const std::string &key) {
		return options[key].template as<T>();
	}

	bool ProgramOptions::has(const std::string &key) {
		return options.count(key);
	}

	ProgramOptions::ProgramOptions(const std::string &toolname) :
			description(std::make_shared<po::options_description>(toolname)) {
		createDefaultOptions();
	}

	void ProgramOptions::parse(int argc, const char **argv) {
		po::store(po::parse_command_line(argc, argv, *description), options);
		po::notify(options);

		if (has("help")) {
			std::cout << description << std::endl;
			throw std::logic_error("Leaving gracefully, nothing to worry! :)");
		}

		if (!FileExists(get<std::string>("input"))) {
			throw std::invalid_argument("Input file does not exist.");
		}

		if (FileExists(get<std::string>("output")) && !overrideOutputFile) {
			throw std::invalid_argument("Output file does exist and override flag not set.");
		}
	}

	void ProgramOptions::createDefaultOptions() {
		description->add_options()
				("help,h", "Show this help message.")
				("input,i",
				 po::value<std::string>()->default_value("../misc/test_frame.png"),
				 "The path to the input file.")
				("output,o",
				 po::value<std::string>()->default_value("./result.yaml"),
				 "The path to the output file.")
				("override,r",
				 po::bool_switch(&overrideOutputFile),
				 "The path to the output file.");
	}

	po::options_description_easy_init ProgramOptions::addOption() {
		return description->add_options();
	}

	template std::string ProgramOptions::get<std::string>(const std::string &key);

}