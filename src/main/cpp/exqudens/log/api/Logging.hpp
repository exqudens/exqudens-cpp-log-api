/*!
* @file Logging.hpp
*/

#pragma once

#include <cstddef>
#include <string>
#include <any>
#include <vector>
#include <map>
#include <sstream>

#ifndef EXQUDENS_LOG_FATAL
#define EXQUDENS_LOG_FATAL(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, 1)
#endif

#ifndef EXQUDENS_LOG_ERROR
#define EXQUDENS_LOG_ERROR(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, 2)
#endif

#ifndef EXQUDENS_LOG_WARNING
#define EXQUDENS_LOG_WARNING(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, 3)
#endif

#ifndef EXQUDENS_LOG_INFO
#define EXQUDENS_LOG_INFO(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, 4)
#endif

#ifndef EXQUDENS_LOG_DEBUG
#define EXQUDENS_LOG_DEBUG(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, 5)
#endif

#ifndef EXQUDENS_LOG_TRACE
#define EXQUDENS_LOG_TRACE(id) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, 6)
#endif

#ifndef EXQUDENS_LOG_API_EXPORT
#define EXQUDENS_LOG_API_EXPORT
#endif

namespace exqudens::log::api {

    class EXQUDENS_LOG_API_EXPORT Logging {

        public:

            class EXQUDENS_LOG_API_EXPORT Writer {

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

        private:

            inline static bool configured = false;

        public:

            static std::map<unsigned short, std::string> levelNameMap();

            static void write(
                const std::string& file,
                const size_t line,
                const std::string& function,
                const std::string& id,
                const unsigned short level,
                const std::string& message
            );

            static std::string configure(const std::any& input);

            static bool isConfigured();

            static void reset();

            static std::string commandLineKey();

            static std::string configureCommandLine(const std::vector<std::string>& arguments);

    };

}
