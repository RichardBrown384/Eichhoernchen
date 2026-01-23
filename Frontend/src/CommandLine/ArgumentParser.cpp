#include "Frontend/CommandLine/ArgumentParser.h"

#include <argparse/argparse.hpp>

#include "Frontend/Systems/Systems.h"

using namespace rbrown::platform;
using namespace rbrown::frontend;

namespace rbrown::frontend {
auto ParseArguments(const int argc, const char** argv) -> ProgramArguments {

    std::string system;
    std::filesystem::path mediaPath;
    std::filesystem::path dataDirectory;
    int scale{};

    argparse::ArgumentParser parser("frontend");

    parser.add_argument("system")
        .help("The name of the system to emulate.")
        .choices(ARCHIMEDES)
        .required()
        .store_into(system);

    parser.add_argument("media")
        .help("The media to load into the emulator")
        .nargs(0, 1)
        .store_into(mediaPath);

    parser.add_argument("--dataDirectory")
        .help("The emulator's working directory.")
        .nargs(0, 1)
        .store_into(dataDirectory);

    parser.add_argument("--scale")
        .help("The amount to scale the graphics by.")
        .default_value(1)
        .scan<'i', int>()
        .choices(1, 2, 3, 4, 5)
        .store_into(scale);

    try {
        parser.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }

    return {
        .system = system,
        .mediaPath = mediaPath,
        .dataDirectory = dataDirectory,
        .scale = scale
    };
}
}