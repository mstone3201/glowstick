#pragma once

#include <memory>

namespace glowstick {
    class renderer {
    public:
        renderer();
        renderer(const renderer&) = delete;

        renderer& operator=(const renderer&) = delete;

        ~renderer();

    private:
        struct impl;
        std::unique_ptr<impl> p_impl;
    };
}
