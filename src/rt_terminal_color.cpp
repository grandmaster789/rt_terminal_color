#include "rt_terminal_color.h"
#include <cstdio>
#include <iostream>
#include <ostream>

namespace rt::terminal {
    // semi-global (not accessible in other sources)
    static e_Color g_ForegroundColor = e_Color::default_color;
    static e_Color g_BackgroundColor = e_Color::default_color;
    static e_Style g_Style = e_Style::regular;

    void set_fg_color(e_Color color) {
        switch (color) {
            case e_Color::black:         puts("\033[30m"); break;
            case e_Color::red:           puts("\033[31m"); break;
            case e_Color::green:         puts("\033[32m"); break;
            case e_Color::yellow:        puts("\033[33m"); break;
            case e_Color::blue:          puts("\033[34m"); break;
            case e_Color::magenta:       puts("\033[35m"); break;
            case e_Color::cyan:          puts("\033[36m"); break;
            case e_Color::gray:          puts("\033[37m"); break;
                // 38 is a custom foreground color, 2 modes for selecting a color
                // (here we use a full rgb assignment)
            case e_Color::white:         puts("\033[38;2;255;255;255m"); break;
            case e_Color::default_color: puts("\033[39m"); break;
            default:
                std::cerr << "Unknown color requested\n";
                return;
        }

        g_ForegroundColor = color;
    }

    void set_bg_color(e_Color color) {
        switch (color) {
            case e_Color::black:         puts("\033[40m"); break;
            case e_Color::red:           puts("\033[41m"); break;
            case e_Color::green:         puts("\033[42m"); break;
            case e_Color::yellow:        puts("\033[43m"); break;
            case e_Color::blue:          puts("\033[44m"); break;
            case e_Color::magenta:       puts("\033[45m"); break;
            case e_Color::cyan:          puts("\033[46m"); break;
            case e_Color::gray:          puts("\033[47m"); break;
                // 48 is a custom background color, 2 modes for selecting a color
                // (here we use a full rgb assignment)
            case e_Color::white:         puts("\033[48;2;255;255;255m"); break;
            case e_Color::default_color: puts("\033[49m"); break;
            default:
                std::cerr << "Unknown color requested\n";
                return;
        }

        g_BackgroundColor = color;
    }

    void set_style(e_Style style) {
        switch (style) {
            case e_Style::regular:   puts("\033[0m"); break;
            case e_Style::bold:      puts("\033[1m"); break;
            case e_Style::dark:      puts("\033[2m"); break;
            case e_Style::italic:    puts("\033[3m"); break;
            case e_Style::underline: puts("\033[4m"); break;
            case e_Style::blink:     puts("\033[5m"); break;
            case e_Style::reverse:   puts("\033[7m"); break;
            case e_Style::concealed: puts("\033[8m"); break;
            default:
                std::cerr << "Unknown style requested\n";
        }

        g_Style = style;
    }

    void reset_color_and_style() {
        set_style(e_Style::regular);
    }

    ScopedStyle::ScopedStyle(
        e_Color fg,
        e_Color bg,
        e_Style style
    ):
        m_StoredForegroundColor(g_ForegroundColor),
        m_StoredBackgroundColor(g_BackgroundColor),
        m_StoredStyle(g_Style)
    {
        set_fg_color(fg);
        set_bg_color(bg);
        set_style(style);
    }

    ScopedStyle::~ScopedStyle() {
        set_style(m_StoredStyle);
        set_bg_color(m_StoredBackgroundColor);
        set_fg_color(m_StoredForegroundColor);
    }

    std::ostream& operator << (std::ostream& os, e_Color color) {
        switch (color) {
            case e_Color::black:         os << "black";   break;
            case e_Color::gray:          os << "gray";    break;
            case e_Color::red:           os << "red";     break;
            case e_Color::green:         os << "green";   break;
            case e_Color::yellow:        os << "yellow";  break;
            case e_Color::blue:          os << "blue";    break;
            case e_Color::magenta:       os << "magenta"; break;
            case e_Color::cyan:          os << "cyan";    break;
            case e_Color::white:         os << "white";   break;
            case e_Color::default_color: os << "default"; break;
            default:
                os << "[UNKNOWN COLOR]";
        }

        return os;
    }

    std::ostream& operator << (std::ostream& os, e_Style style) {
        switch (style) {
            case e_Style::regular:   os << "regular";   break;
            case e_Style::bold:      os << "bold";      break;
            case e_Style::dark:      os <<"dark";       break;
            case e_Style::italic:    os << "italic";    break;
            case e_Style::underline: os << "underline"; break;
            case e_Style::blink:     os << "blink";     break;
            case e_Style::reverse:   os << "reverse";   break;
            case e_Style::concealed: os << "concealed"; break;
            default:
                os << "[UNKNOWN STYLE]";
        }

        return os;
    }
}
