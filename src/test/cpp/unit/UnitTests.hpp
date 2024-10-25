#pragma once

#include <cstddef>
#include <string>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TestUtils.hpp"
#include "TestLog.hpp"

#include "exqudens/log/api/Logging.hpp"

#define LOG_DEBUG(id) EXQUDENS_LOG_DEBUG(id)

namespace exqudens::log::api {

    Logging::Writer::Writer(
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
        level(level) {
    }

    Logging::Writer::~Writer() {
        Logging::write(
            file,
            line,
            function,
            id,
            level,
            stream.str()
        );
    }

    std::string Logging::getKey() {
        return "--logging-config";
    }

    std::string Logging::configure(const std::vector<std::string>& arguments) {
        Logging::configured = true;
        return "";
    }

    bool Logging::isConfigured() {
        return Logging::configured;
    }

    void Logging::reset() {
        Logging::configured = false;
    }

    void Logging::write(
        const std::string& file,
        const size_t line,
        const std::string& function,
        const std::string& id,
        const unsigned short level,
        const std::string& message
    ) {
        TestLog::Writer(file, line, function, id, level) << message;
    }

}

namespace exqudens::fpga::debugger {

    class UnitTests: public testing::Test {

        protected:

            inline static const char* LOGGER_ID = "exqudens.log.api.UnitTests";

    };

    TEST_F(UnitTests, test1) {
        try {
            std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
            std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
            TEST_LOG_I(LOGGER_ID) << "'" << testGroup << "." << testCase << "' bgn";

            LOG_DEBUG(LOGGER_ID) << "AAA";

            TEST_LOG_I(LOGGER_ID) << "'" << testGroup << "." << testCase << "' end";
        } catch (const std::exception& e) {
            std::string errorMessage = TestUtils::toString(e);
            TEST_LOG_E(LOGGER_ID) << errorMessage;
            FAIL() << errorMessage;
        }
    }

}
