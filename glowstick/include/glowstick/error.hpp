#pragma once

#include <exception>
#include <string_view>
#include <memory>

namespace glowstick {
    class error : public std::exception {
    public:
        explicit error(std::string_view message);
        error(const error& other) noexcept;

        error& operator=(const error& other) noexcept;

        const char* what() const noexcept override;
        
    private:
        std::shared_ptr<std::string> message;
    };
}
