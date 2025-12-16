#pragma once

#include <iosfwd>

namespace rt::terminal {
    // based on ANSI 'Select Graphics Rendition' commands
    // https://en.wikipedia.org/wiki/ANSI_escape_code#SGR
    //

    enum class e_Color {
        black,
        gray,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white,
        default_color
    };

    enum class e_Style {
        regular,    // all attributes get turned off
        bold,       // either bold or increased intensity
        dark,       // may be implemented as a light font weight (similar to bold) or decreased intensity
        italic,     // not widely supported
        underline,  //
        blink,      //
        reverse,    // swap foreground and background color
        concealed   // or hide, not widely supported
    };

    void set_fg_color(e_Color color);
    void set_bg_color(e_Color color);
    void set_style(e_Style style);
    void reset_color_and_style();

    struct ScopedStyle {
        ScopedStyle(
            e_Color fg = e_Color::gray,
            e_Color bg = e_Color::black,
            e_Style style = e_Style::regular
        );
        ~ScopedStyle();

        ScopedStyle(ScopedStyle const&) = delete;
        ScopedStyle& operator=(ScopedStyle const&) = delete;
        ScopedStyle(ScopedStyle&&) noexcept = delete;
        ScopedStyle& operator = (ScopedStyle&&) noexcept = delete;

    private:
        e_Color m_StoredForegroundColor;
        e_Color m_StoredBackgroundColor;
        e_Style m_StoredStyle;
    };

    std::ostream& operator<<(std::ostream& os, e_Color color);
    std::ostream& operator<<(std::ostream& os, e_Style style);
}