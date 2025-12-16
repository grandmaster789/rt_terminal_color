// Catch2 v3 (provided via CMake FetchContent)
#include <catch2/catch_test_macros.hpp>

#include "rt_terminal_color.h"

#include <cstdio>
#include <string>

#if defined(_WIN32)
    #include <io.h>
    #define RT_FILENO _fileno
    #define RT_DUP    _dup
    #define RT_DUP2   _dup2
    #define RT_CLOSE  _close
#else
    #include <unistd.h>
    #define RT_FILENO fileno
    #define RT_DUP    dup
    #define RT_DUP2   dup2
    #define RT_CLOSE  close
#endif

namespace {
    // Captures what is written to stdout during `fn()`.
    template <class Fn>
    std::string capture_stdout(Fn&& fn) {
        std::fflush(stdout);

        const int stdout_fd = RT_FILENO(stdout);
        const int saved_fd = RT_DUP(stdout_fd);
        REQUIRE(saved_fd != -1);

        FILE* tmp = std::tmpfile();
        REQUIRE(tmp != nullptr);

        const int tmp_fd = RT_FILENO(tmp);
        REQUIRE(tmp_fd != -1);

        REQUIRE(RT_DUP2(tmp_fd, stdout_fd) != -1);

        fn();

        std::fflush(stdout);
        std::fseek(tmp, 0, SEEK_SET);

        std::string out;
        char buffer[256];
        while (std::fgets(buffer, sizeof(buffer), tmp) != nullptr) {
            out += buffer;
        }

        REQUIRE(RT_DUP2(saved_fd, stdout_fd) != -1);
        RT_CLOSE(saved_fd);
        std::fclose(tmp);

        return out;
    }

    constexpr char ESC = '\x1B';
}

TEST_CASE("set_fg_color prints correct ANSI SGR sequence", "[terminal][color][fg]") {
    using rt::terminal::e_Color;
    using rt::terminal::set_fg_color;

    struct Case { e_Color c; std::string expected; };

    const Case cases[] = {
        {e_Color::black,         std::string() + ESC + "[30m\n"},
        {e_Color::red,           std::string() + ESC + "[31m\n"},
        {e_Color::green,         std::string() + ESC + "[32m\n"},
        {e_Color::yellow,        std::string() + ESC + "[33m\n"},
        {e_Color::blue,          std::string() + ESC + "[34m\n"},
        {e_Color::magenta,       std::string() + ESC + "[35m\n"},
        {e_Color::cyan,          std::string() + ESC + "[36m\n"},
        {e_Color::gray,          std::string() + ESC + "[37m\n"},
        {e_Color::white,         std::string() + ESC + "[38;2;255;255;255m\n"},
        {e_Color::default_color, std::string() + ESC + "[39m\n"},
    };

    for (auto const& tc : cases) {
        auto out = capture_stdout([&] { set_fg_color(tc.c); });
        REQUIRE(out == tc.expected);
    }
}

TEST_CASE("set_bg_color prints correct ANSI SGR sequence", "[terminal][color][bg]") {
    using rt::terminal::e_Color;
    using rt::terminal::set_bg_color;

    struct Case { e_Color c; std::string expected; };

    const Case cases[] = {
        {e_Color::black,         std::string() + ESC + "[40m\n"},
        {e_Color::red,           std::string() + ESC + "[41m\n"},
        {e_Color::green,         std::string() + ESC + "[42m\n"},
        {e_Color::yellow,        std::string() + ESC + "[43m\n"},
        {e_Color::blue,          std::string() + ESC + "[44m\n"},
        {e_Color::magenta,       std::string() + ESC + "[45m\n"},
        {e_Color::cyan,          std::string() + ESC + "[46m\n"},
        {e_Color::gray,          std::string() + ESC + "[47m\n"},
        {e_Color::white,         std::string() + ESC + "[48;2;255;255;255m\n"},
        {e_Color::default_color, std::string() + ESC + "[49m\n"},
    };

    for (auto const& tc : cases) {
        auto out = capture_stdout([&] { set_bg_color(tc.c); });
        REQUIRE(out == tc.expected);
    }
}

TEST_CASE("set_style prints correct ANSI SGR sequence", "[terminal][style]") {
    using rt::terminal::e_Style;
    using rt::terminal::set_style;

    struct Case { e_Style s; std::string expected; };

    const Case cases[] = {
        {e_Style::regular,   std::string() + ESC + "[0m\n"},
        {e_Style::bold,      std::string() + ESC + "[1m\n"},
        {e_Style::dark,      std::string() + ESC + "[2m\n"},
        {e_Style::italic,    std::string() + ESC + "[3m\n"},
        {e_Style::underline, std::string() + ESC + "[4m\n"},
        {e_Style::blink,     std::string() + ESC + "[5m\n"},
        {e_Style::reverse,   std::string() + ESC + "[7m\n"},
        {e_Style::concealed, std::string() + ESC + "[8m\n"},
    };

    for (auto const& tc : cases) {
        auto out = capture_stdout([&] { set_style(tc.s); });
        REQUIRE(out == tc.expected);
    }
}

TEST_CASE("ScopedStyle sets then restores previous attributes (print order matters)", "[terminal][scoped]") {
    using rt::terminal::ScopedStyle;
    using rt::terminal::e_Color;
    using rt::terminal::e_Style;
    using rt::terminal::set_bg_color;
    using rt::terminal::set_fg_color;
    using rt::terminal::set_style;

    // Establish a known "previous" state.
    capture_stdout([&] {
        set_fg_color(e_Color::red);
        set_bg_color(e_Color::default_color);
        set_style(e_Style::underline);
    });

    auto out = capture_stdout([&] {
        {
            ScopedStyle scoped(e_Color::green, e_Color::blue, e_Style::bold);
        }
    });

    // Constructor: fg, bg, style
    // Destructor: style, bg, fg (restores stored values)
    const std::string expected =
        std::string() + ESC + "[32m\n" +   // fg green
        ESC + "[44m\n" +                   // bg blue
        ESC + "[1m\n" +                    // bold
        ESC + "[4m\n" +                    // restore underline
        ESC + "[49m\n" +                   // restore default bg
        ESC + "[31m\n";                    // restore red fg

    REQUIRE(out == expected);
}