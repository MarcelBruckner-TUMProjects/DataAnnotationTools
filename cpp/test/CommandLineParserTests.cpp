//
// Created by brucknem on 09.03.21.
//

#include "gtest/gtest.h"
#include "DataAnnotationTools/watershed/CommandLineParser.hpp"

namespace po = boost::program_options;

namespace data_annotation_tools {
    namespace tests {
        /**
         * Tests for the command line options parser.
         */
        class CommandLineParserTests : public ::testing::Test {
            // Empty by choice, i.e just for naming the test suite.

        public:

            /**
             * @destructor
             */
            virtual ~CommandLineParserTests() = default;
        };

        /**
         * Tests the help text for the default options description.
         */
        TEST_F(CommandLineParserTests, testDefaultArguments) {
            std::vector<char *> argv;

            auto programOptions = data_annotation_tools::utils::CommandLineParser("Test");

            std::stringstream actual;
            actual << programOptions;

            std::string expected = "Test:\n"
                                   "  -h [ --help ]                         Show this help message.\n"
                                   "  -i [ --input ] arg (=../misc/test_frame.png)\n"
                                   "                                        The path to the input file. The path \n"
                                   "                                        can be relative or absolute. The type \n"
                                   "                                        of the file is tool dependent.\n"
                                   "  -o [ --output ] arg (=./result.yaml)  The path to the output file. The path \n"
                                   "                                        can be relative or absolute. The type \n"
                                   "                                        of the output file is tool dependent.\n"
                                   "  -r [ --override_output ]              Flag if the output file should be \n"
                                   "                                        overridden if it already exists.\n"
                                   "  --debug_parser                        Flag if the parser is in debug mode.\n";

            ASSERT_STREQ(actual.str().c_str(), expected.c_str());
        }

        /**
         * Tests the getting the help text for the default options.
         */
        TEST_F(CommandLineParserTests, testGetHelp) {
            std::vector<const char *> argv;

            argv = {const_cast<const char *>("test_name"), const_cast<const char *>("-h"),
                    const_cast<const char *>("-debug_parser")};
            auto programOptions = data_annotation_tools::utils::CommandLineParser("Test");
            ASSERT_THROW(programOptions.parse(argv.size(), argv.data()), std::logic_error);

            argv[1] = const_cast<const char *>("--help");
            ASSERT_THROW(programOptions.parse(argv.size(), argv.data()), std::logic_error);
        }

    }
}