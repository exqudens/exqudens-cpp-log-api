#pragma once

#include <cstddef>
#include <string>
#include <map>
#include <sstream>

#ifndef TEST_LOG_FATAL
#define TEST_LOG_FATAL(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, 1)
#endif

#ifndef TEST_LOG_ERROR
#define TEST_LOG_ERROR(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, 2)
#endif

#ifndef TEST_LOG_WARNING
#define TEST_LOG_WARNING(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, 3)
#endif

#ifndef TEST_LOG_INFO
#define TEST_LOG_INFO(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, 4)
#endif

#ifndef TEST_LOG_DEBUG
#define TEST_LOG_DEBUG(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, 5)
#endif

#ifndef TEST_LOG_TRACE
#define TEST_LOG_TRACE(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, 6)
#endif

class TestLog {

    public:

        class Writer {

            private:

                std::string file = "";
                size_t line = 0;
                std::string function = "";
                std::string id = "";
                unsigned short level = 0;
                std::ostringstream stream = {};

            public:

                Writer(
                    std::string file,
                    size_t line,
                    std::string function,
                    std::string id,
                    unsigned short level
                );

                Writer() = delete;
                Writer(const Writer&) = delete;
                Writer& operator=(const Writer&) = delete;

                template <typename T>
                inline Writer& operator<<(const T& value) {
                    stream << value;
                    return *this;
                }

                ~Writer();

        };

    public:

        static std::map<unsigned short, std::string> levelNameMap();

        static void write(
                const std::string& file,
                const size_t& line,
                const std::string& function,
                const std::string& id,
                const unsigned short& level,
                const std::string& message
        );

};
