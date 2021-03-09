//
// Created by brucknem on 09.03.21.
//

#include "gtest/gtest.h"
#include <boost/program_options.hpp>
#include "ProgramOptions.hpp"

namespace po = boost::program_options;

namespace dataannotationtools {
	namespace tests {
		/**
		 * Tests for the program options.
		 */
		class ProgramOptionsTests : public ::testing::Test {
		protected:

			/**
			 * The default options description.
			 */
			po::options_description description = dataannotationtools::GetDefaultProgramOptions("Test");
		};

		/**
		 * Tests the help text for the default options description.
		 */
		TEST_F(ProgramOptionsTests, testDefaultArguments) {
			std::vector<char *> argv;

			std::stringstream actual;
			actual << description;

			std::string expected = "Test:\n"
								   "  -h [ --help ]                         Show this help message.\n"
								   "  -i [ --input ] arg (=../misc/test_frame.png)\n"
								   "                                        The path to the input file.\n"
								   "  -o [ --output ] arg (=./result.yaml)  The path to the output file.\n";

			ASSERT_STREQ(actual.str().c_str(), expected.c_str());
		}

		/**
		 * Tests the getting the help text for the default options.
		 */
		TEST_F(ProgramOptionsTests, testGetHelp) {
			std::vector<const char *> argv;

			argv = {const_cast<const char *>("test_name"), const_cast<const char *>("-h")};
			ASSERT_THROW(dataannotationtools::ParseOptions(description, argv.size(), argv.data()), std::logic_error);

			argv[1] = const_cast<const char *>("--help");
			ASSERT_THROW(dataannotationtools::ParseOptions(description, argv.size(), argv.data()), std::logic_error);
		}

	}
}