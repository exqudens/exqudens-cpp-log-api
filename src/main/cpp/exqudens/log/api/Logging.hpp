/*!
* @file Logging.hpp
*/

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <sstream>

#ifndef EXQUDENS_LOG_FATAL
#define EXQUDENS_LOG_FATAL(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) exqudens::log::api::Logging::Level::FATAL)
#endif

#ifndef EXQUDENS_LOG_ERROR
#define EXQUDENS_LOG_ERROR(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) exqudens::log::api::Logging::Level::ERROR)
#endif

#ifndef EXQUDENS_LOG_WARNING
#define EXQUDENS_LOG_WARNING(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) exqudens::log::api::Logging::Level::WARNING)
#endif

#ifndef EXQUDENS_LOG_INFO
#define EXQUDENS_LOG_INFO(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) exqudens::log::api::Logging::Level::INFO)
#endif

#ifndef EXQUDENS_LOG_DEBUG
#define EXQUDENS_LOG_DEBUG(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) exqudens::log::api::Logging::Level::DEBUG)
#endif

#ifndef EXQUDENS_LOG_TRACE
#define EXQUDENS_LOG_TRACE(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) exqudens::log::api::Logging::Level::TRACE)
#endif

namespace exqudens::log::api {

    class Logging {

        public:

            enum class Level : unsigned short {
                FATAL = 1,
                ERROR = 2,
                WARNING = 3,
                INFO = 4,
                DEBUG = 5,
                TRACE = 6
            };

            class Writer {

                private:

                    std::string file;
                    size_t line;
                    std::string function;
                    std::string id;
                    unsigned short level;
                    std::ostringstream stream;

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

        private:

            inline static bool configured = false;

        public:

            static std::string getKey();

            static std::string configure(const std::vector<std::string>& arguments);

            static bool isConfigured();

            static void reset();

        private:

            static void write(
                const std::string& file,
                const size_t line,
                const std::string& function,
                const std::string& id,
                const unsigned short level,
                const std::string& message
            );

    };

}
