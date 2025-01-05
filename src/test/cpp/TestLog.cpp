#include <optional>
#include <filesystem>
#include <stdexcept>
#include <utility>
#include <iostream>

#include "TestLog.hpp"

#define CALL_INFO std::string(__FUNCTION__) + "(" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"

TestLog::Writer::Writer(
        std::string file,
        size_t line,
        std::string function,
        std::string id,
        unsigned short level
):
        file(std::move(file)),
        line(line),
        function(std::move(function)),
        id(std::move(id)),
        level(level)
{}

TestLog::Writer::~Writer() {
    TestLog::write(
            file,
            line,
            function,
            id,
            level,
            stream.str()
    );
}

std::map<unsigned short, std::string> TestLog::levelNameMap() {
    return {
        {1, "FATAL"},
        {2, "ERROR"},
        {3, "WARNING"},
        {4, "INFO"},
        {5, "DEBUG"},
        {6, "TRACE"}
    };
}

void TestLog::write(
        const std::string& file,
        const size_t& line,
        const std::string& function,
        const std::string& id,
        const unsigned short& level,
        const std::string& message
) {
    try {
        std::map<unsigned short, std::string> levelName = levelNameMap();
        if (levelName.contains(level)) {
            std::string internalLevel = levelName.at(level);
            std::string internalFile = std::filesystem::path(file).filename().string();
            std::cout << internalLevel << " " << id << " (" << internalFile << ":" << line << ") [" << function << "]: " << message << std::endl;
        }
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

#undef CALL_INFO
