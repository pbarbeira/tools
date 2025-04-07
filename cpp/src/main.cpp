#include <iostream>
#include "Options.h"

int main(int argc, char* argv[]) {
    const auto logger = std::make_unique<ConsoleLogger>();
    auto options = OptionsParser::parse(argc, argv, logger.get());



    return 0;
}