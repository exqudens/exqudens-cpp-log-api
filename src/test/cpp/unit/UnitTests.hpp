#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <any>
#include <vector>
#include <map>
#include <utility>
#include <filesystem>
#include <stdexcept>
#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TestUtils.hpp"

#ifndef EXQUDENS_LOG_API_EXPORT
#define EXQUDENS_LOG_API_EXPORT
#endif
#include "exqudens/log/api/Logging.hpp"

#define CALL_INFO std::string(__FUNCTION__) + "(" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"

namespace exqudens::log::api {

    class UnitTests: public testing::Test {

        public:

            inline static std::vector<std::any> writeEvents = {};

        protected:

            inline static const char* LOGGER_ID = "exqudens.log.api.UnitTests";

    };

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

    std::string Logging::configure(const std::any& input) {
        Logging::data = input;
        return "input";
    }

    bool Logging::isConfigured() {
        return Logging::data.has_value();
    }

    void Logging::write(
        const std::string& file,
        const size_t line,
        const std::string& function,
        const std::string& id,
        const unsigned short level,
        const std::string& message
    ) noexcept {
        try {
            if (!Logging::isConfigured()) {
                return;
            }
            std::map<unsigned short, std::string> levelNameMap = std::any_cast<std::map<unsigned short, std::string>>(Logging::data);
            if (!levelNameMap.contains(level)) {
                return;
            }
            std::map<std::string, std::any> event = {};
            event.insert({"file", std::filesystem::path(file).filename().generic_string()});
            event.insert({"line", std::to_string(line)});
            event.insert({"function", function});
            event.insert({"id", id});
            event.insert({"level", levelNameMap.at(level)});
            event.insert({"message", message});
            UnitTests::writeEvents.emplace_back(event);
        } catch (...) {
            return;
        }
    }

    void Logging::reset() {
        Logging::data = {};
    }

    TEST_F(UnitTests, test1) {
        try {
            std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
            std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
            std::cout << LOGGER_ID << " " << '"' << testGroup << '.' << testCase << '"' << " bgn" << std::endl;

            std::map<std::string, std::any> currentInfo = {
                {"file", std::filesystem::path(__FILE__).filename().generic_string()},
                {"function", __FUNCTION__},
                {"id", LOGGER_ID}
            };

            std::vector<std::any> expectedEvents = {};
            std::map<std::string, std::any> expectedEvent = {};
            std::vector<std::any> actualEvents = {};
            std::map<std::string, std::any> actualEvent = {};

            // configure
            std::map<unsigned short, std::string> levelNameMap = {
                {1, "FATAL"},
                {2, "ERROR"},
                {3, "WARNING"},
                {4, "INFO"},
                {5, "DEBUG"},
                {6, "TRACE"}
            };
            Logging::configure(levelNameMap);

            // 0
            expectedEvent = currentInfo;
            EXQUDENS_LOG(LOGGER_ID, 6) << (nullptr_t) nullptr; expectedEvent.insert({"line", (size_t) __LINE__});
            expectedEvent.insert({"message", (nullptr_t) nullptr});
            expectedEvent.insert({"level", "TRACE"});
            expectedEvents.emplace_back(expectedEvent);

            // 1
            expectedEvent = currentInfo;
            EXQUDENS_LOG(LOGGER_ID, 5) << (intmax_t) -999; expectedEvent.insert({"line", (size_t) __LINE__});
            expectedEvent.insert({"message", (intmax_t) -999});
            expectedEvent.insert({"level", "DEBUG"});
            expectedEvents.emplace_back(expectedEvent);

            // 2
            expectedEvent = currentInfo;
            EXQUDENS_LOG(LOGGER_ID, 4) << (size_t) 999; expectedEvent.insert({"line", (size_t) __LINE__});
            expectedEvent.insert({"message", (size_t) 999});
            expectedEvent.insert({"level", "INFO"});
            expectedEvents.emplace_back(expectedEvent);

            // 3
            expectedEvent = currentInfo;
            EXQUDENS_LOG(LOGGER_ID, 3) << (long double) 1.5; expectedEvent.insert({"line", (size_t) __LINE__});
            expectedEvent.insert({"message", (long double) 1.5});
            expectedEvent.insert({"level", "WARNING"});
            expectedEvents.emplace_back(expectedEvent);

            // 4
            expectedEvent = currentInfo;
            EXQUDENS_LOG(LOGGER_ID, 2) << (const char *) "Abc"; expectedEvent.insert({"line", (size_t) __LINE__});
            expectedEvent.insert({"message", (const char *) "Abc"});
            expectedEvent.insert({"level", "ERROR"});
            expectedEvents.emplace_back(expectedEvent);

            // 5
            expectedEvent = currentInfo;
            EXQUDENS_LOG(LOGGER_ID, 1) << (std::string) "Abc123"; expectedEvent.insert({"line", (size_t) __LINE__});
            expectedEvent.insert({"message", (std::string) "Abc123"});
            expectedEvent.insert({"level", "FATAL"});
            expectedEvents.emplace_back(expectedEvent);

            actualEvents = writeEvents;

            std::cout << "expectedEvents.size: " << expectedEvents.size() << std::endl;
            std::cout << "actualEvents.size: " << actualEvents.size() << std::endl;

            ASSERT_EQ(expectedEvents.size(), actualEvents.size());

            for (size_t i = 0; i < expectedEvents.size(); i++) {
                std::cout << "i: " << i << std::endl;

                expectedEvent = std::any_cast<std::map<std::string, std::any>>(expectedEvents.at(i));
                actualEvent = std::any_cast<std::map<std::string, std::any>>(actualEvents.at(i));

                std::cout << "expectedEvent.size: " << expectedEvent.size() << std::endl;
                std::cout << "actualEvent.size: " << actualEvent.size() << std::endl;

                ASSERT_EQ(expectedEvent.size(), actualEvent.size());

                for (const std::pair<std::string, std::any>& expectedEntry : expectedEvent) {
                    std::string key = expectedEntry.first;
                    std::cout << "key: " << '"' << key << '"' << std::endl;

                    ASSERT_TRUE(actualEvent.contains(key));

                    std::string expectedValue = TestUtils::toString(expectedEntry.second);
                    std::string actualValue = TestUtils::toString(actualEvent.at(key));

                    std::cout << "expectedValue: " << expectedValue << std::endl;
                    std::cout << "actualValue: " << actualValue << std::endl;

                    ASSERT_EQ(expectedValue, actualValue);
                }
            }

            std::cout << LOGGER_ID << " " << '"' << testGroup << '.' << testCase << '"' << " end" << std::endl;
        } catch (const std::exception& e) {
            std::string errorMessage = TestUtils::toString(e);
            std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
            FAIL() << errorMessage;
        }
    }

}

#undef CALL_INFO
