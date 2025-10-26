#include "Common/Config/Config.h"

#include "Toolkit/Toolkit.h"

#include <argparse/argparse.hpp>
#include <memory>
#include <functional>

using namespace rbrown::config;
using namespace rbrown::toolkit;

using EmulatorUniquePtr = std::unique_ptr<Emulator>;
using EmulatorFactory = std::function<EmulatorUniquePtr(const SystemConfiguration&)>;

auto CreateArchimedes(const SystemConfiguration&) -> EmulatorUniquePtr;

struct IndividualConfiguration {
    std::vector<std::string> m_zipSearchExtensions {};
    std::vector<std::string> m_biosFiles {};
};

constexpr auto AddBiosFilesArgument(
    argparse::ArgumentParser& parser,
    const std::initializer_list<std::string>& biosFiles,
    IndividualConfiguration& config) {
    parser.add_argument("--bios-files")
        .help("The locations of the bios file(s).")
        .default_value(std::vector<std::string>{ biosFiles })
        .nargs(argparse::nargs_pattern::at_least_one)
        .store_into(config.m_biosFiles);
}

constexpr auto AddZipSearchExtensionsArgument(
    argparse::ArgumentParser& parser,
    const std::initializer_list<std::string>& extensions,
    IndividualConfiguration& config) {
    parser.add_argument("--zip-search-extensions")
        .help("The file extension(s) to search inside a ZIP file for.")
        .default_value(std::vector<std::string>{ extensions })
        .nargs(argparse::nargs_pattern::at_least_one)
        .store_into(config.m_zipSearchExtensions);
}

auto MergeConfiguration(const SystemConfiguration& global, const IndividualConfiguration& individual) -> SystemConfiguration {
    return {
        .m_scale = global.m_scale,
        .m_filename = global.m_filename,
        .m_zipSearchExtensions = individual.m_zipSearchExtensions,
        .m_biosFiles = individual.m_biosFiles
    };
}

auto ParseArgumentsForFactory(int argc, char** argv) -> std::pair<SystemConfiguration, EmulatorFactory> {

    SystemConfiguration globalConfig{};
    
    IndividualConfiguration archimedesConfig{};

    argparse::ArgumentParser frontendParser("frontend");

    argparse::ArgumentParser scaleParser("scale", "1.0", argparse::default_arguments::none);
    scaleParser.add_argument("--scale")
        .help("The amount to scale the graphics by.")
        .default_value(1)
        .scan<'i', int>()
        .choices(1, 2, 3, 4, 5)
        .store_into(globalConfig.m_scale);

    argparse::ArgumentParser filenameParser("filename", "1.0", argparse::default_arguments::none);
    filenameParser.add_argument("filename")
        .help("The filename of the application to load into the emulator")
        .nargs(0, 1)
        .store_into(globalConfig.m_filename);

    argparse::ArgumentParser archimedesParser("archimedes");
    AddBiosFilesArgument(archimedesParser, {
        "./bios/riscos-200/IC24.ROM",
        "./bios/riscos-200/IC25.ROM",
        "./bios/riscos-200/IC26.ROM",
        "./bios/riscos-200/IC27.ROM",
    }, archimedesConfig);
    AddZipSearchExtensionsArgument(archimedesParser, {
        ".adf", ".ADF",
    }, archimedesConfig);
    archimedesParser.add_parents(scaleParser, filenameParser);

    frontendParser.add_subparser(archimedesParser);

    try {
        frontendParser.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << frontendParser;
        std::exit(1);
    }

    if (frontendParser.is_subcommand_used(archimedesParser)) {
        return { MergeConfiguration(globalConfig, archimedesConfig), CreateArchimedes };
    }

    std::exit(1);
}

int main(int argc, char** argv) {

    const auto& [config, factory] = ParseArgumentsForFactory(argc, argv);

    if (Init()) {
        auto emulator = factory(config);
        if (!emulator) {
            Quit();
            return 1;
        }
        emulator->StartUp();
        emulator->Reset();
        auto running = true;
        while (running) {
            Event event{};
            while (PollEvent(event)) {
                switch (event.type) {
                    case EventType::QUIT: running = false; break;
                    case EventType::KEY_DOWN: emulator->KeyDown(event.key); break;
                    case EventType::KEY_UP: emulator->KeyUp(event.key); break;
                    case EventType::MOUSE_MOTION: emulator->MouseMotion(event.motion); break;
                    case EventType::MOUSE_BUTTON_DOWN: emulator->MouseButtonDown(event.button); break;
                    case EventType::MOUSE_BUTTON_UP: emulator->MouseButtonUp(event.button); break;
                    default: break;
                }
            }
            emulator->RunForFrame();
        }
        emulator->ShutDown();
    } else {
        return 1;
    }

    Quit();
    return 0;
}