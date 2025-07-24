/*!
* @file Logging.hpp
*/

#pragma once

#include <cstddef>
#include <string>
#include <any>
#include <map>
#include <sstream>

#ifndef EXQUDENS_LOG
#define EXQUDENS_LOG(id, level) exqudens::log::api::Logging::Writer(__FILE__, __LINE__, __FUNCTION__, id, level)
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

            inline static std::any data = {};

        public:

            static std::string configure(const std::any& input);

            static bool isConfigured();

            static void write(
                const std::string& file,
                const size_t line,
                const std::string& function,
                const std::string& id,
                const unsigned short level,
                const std::string& message
            ) noexcept;

            static void reset();

    };

}
