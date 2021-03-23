//
// Created by brucknem on 09.03.21.
//

#ifndef DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H
#define DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H

#include <boost/program_options.hpp>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <utility>

namespace po = boost::program_options;

namespace dataannotationtools {

	inline bool FileExists(const std::string &name) {
		return std::ifstream{name.c_str()}.good();
	}

	class ProgramOptions {
	private:
		std::shared_ptr<po::options_description> description;
		po::variables_map options;

		bool overrideOutputFile = true;

		/**
		 * Creates the default options used in the data annotation tools.
		 */
		void createDefaultOptions();

	public:

		/**
		 * @constructor
		 *
		 * @param toolname The name of the tool
		 */
		explicit ProgramOptions(const std::string &toolname);;

		/**
		 * Parses the input options and checks for the default values.
		 *
		 * @param description The argument options description.
		 * @param argc The number of options.
		 * @param argv The options.
		 */
		void parse(int argc, const char **argv);

		bool has(const std::string &key);

		template<typename T>
		T get(const std::string &key);

		po::options_description_easy_init addOption();

		friend std::ostream &operator<<(std::ostream &os, const ProgramOptions &options);
	};

}

#endif //DATAANNOTATIONTOOLS_PROGRAMOPTIONS_H
