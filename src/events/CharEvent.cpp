
#include "RolUI/IEvent.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/events/CharEvent.hpp"

namespace RolUI {

    static bool codepoint_size(unsigned int codepoint, unsigned* size_out) {
        if (codepoint <= 0x7F)
            *size_out = 1;
        else if (codepoint >= 0x80 && codepoint <= 0x7FF)
            *size_out = 2;
        else if (codepoint >= 0x800 && codepoint <= 0xFFFF)
            *size_out = 3;
        else if (codepoint >= 0x10000 && codepoint <= 0x1FFFFF)
            *size_out = 4;
        else if (codepoint >= 0x200000 && codepoint <= 0x3FFFFFF)
            *size_out = 5;
        else if (codepoint >= 0x4000000 && codepoint <= 0x7FFFFFFF)
            *size_out = 6;
        else
            return false;
        return true;
    }
    static int codepoint_to_c_char(unsigned int cp, char* c_char) {
        unsigned cs;
        bool res = codepoint_size(cp, &cs);
        if (!res) return -1;

        auto it = c_char;
        switch (cs) {
            case 1:
                *it = cp & 0x7F;
                break;
            case 2:
                *it = ((cp >> 6) & 0x1F) | 0xC0;
                *++it = (cp & 0x3F) | 0x80;
                break;
            case 3:
                *it = ((cp >> 12) & 0xF) | 0xE0;
                *++it = ((cp >> 6) & 0x3F) | 0x80;
                *++it = (cp & 0x3F) | 0x80;
                break;
            case 4:
                *it = ((cp >> 18) & 0x7) | 0xF0;
                *++it = ((cp >> 12) & 0x3F) | 0x80;
                *++it = ((cp >> 6) & 0x3F) | 0x80;
                *++it = (cp & 0x3F) | 0x80;
                break;
            case 5:
                *it = ((cp >> 24) & 0x3) | 0xF8;
                *++it = ((cp >> 18) & 0x3F) | 0x80;
                *++it = ((cp >> 12) & 0x3F) | 0x80;
                *++it = ((cp >> 6) & 0x3F) | 0x80;
                *++it = (cp & 0x3F) | 0x80;
                break;
            case 6:
                *it = ((cp >> 30) & 0x1) | 0xFC;
                *++it = ((cp >> 24) & 0x3F) | 0x80;
                *++it = ((cp >> 18) & 0x3F) | 0x80;
                *++it = ((cp >> 12) & 0x3F) | 0x80;
                *++it = ((cp >> 6) & 0x3F) | 0x80;
                *++it = (cp & 0x3F) | 0x80;
                break;
        }

        return cs;
    }

    static bool c_char_size(const char* c_char, unsigned* size_out) {
        const char* it = c_char;

        if ((*it & 0x80) == 0) {
            *size_out = 1;
        } else if ((*it & 0xE0) == 0xC0) {
            if ((*++it & 0xC0) != 0x80)
                return false;
            *size_out = 2;
        } else if ((*it & 0xF0) == 0xE0) {
            if ((*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80)
                return false;
            *size_out = 3;
        } else if ((*it & 0xF8) == 0xF0) {
            if ((*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80)
                return false;
            *size_out = 4;
        } else if ((*it & 0xFC) == 0xF8) {
            if ((*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80)
                return false;
            *size_out = 5;
        } else if ((*it & 0xFE) == 0xFC) {
            if ((*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80 || (*++it & 0xC0) != 0x80)
                return false;
            *size_out = 6;
        } else
            return false;
        return true;
    }
    static int c_char_to_codepoint(const char* c_char, unsigned int* cp) {
        unsigned cs;
        auto res = c_char_size(c_char, &cs);
        if (res) return -1;

        auto it = c_char;

        unsigned int tp;
        switch (cs) {
            case 1:
                tp = *it;
                break;
            case 2:
                tp = *it & 0x1F;
                tp = (tp << 6) + (*++it & 0x3F);
                break;
            case 3:
                tp = *it & 0xF;
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                break;
            case 4:
                tp = *it & 0x7;
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                break;
            case 5:
                tp = *it & 0x3;
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                break;
            case 6:
                tp = *it & 0x1;
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                tp = (tp << 6) + (*++it & 0x3F);
                break;
        }

        *cp = tp;

        return cs;
    }

    RolUI_impl_event_type_in_class(CharEvent);

    CharEvent::CharEvent(Widget* target, unsigned int cp) noexcept
        : IEvent(type(), target), _codepoint(cp) {
        for (int i = 0; i < sizeof(_c_str) / sizeof(char); i++)
            _c_str[i] = 0;

        int s = codepoint_to_c_char(cp, _c_str);
        if (s < 0) {
            for (int i = 0; i < sizeof(_c_str) / sizeof(char); i++)
                _c_str[i] = 0;
        }
    }
    CharEvent::~CharEvent() {}

    unsigned int CharEvent::codepoint() const noexcept { return _codepoint; }
    unsigned int CharEvent::char_() const noexcept { return _codepoint; }
    const char* CharEvent::c_char() const noexcept { return _c_str; }

    void CharEventDispatcher::push_char(unsigned int c) noexcept {
        _char_queue.push(c);
    }
    unsigned int CharEventDispatcher::pop_char() noexcept {
        unsigned int c = _char_queue.front();
        _char_queue.pop();
        return c;
    }
    unsigned int CharEventDispatcher::top_char() const noexcept {
        return _char_queue.front();
    }

    unsigned CharEventDispatcher::char_count() const noexcept {
        return _char_queue.size();
    }
    bool CharEventDispatcher::empty() const noexcept { return _char_queue.empty(); }

    void CharEventDispatcher::dispatch(Window* w) noexcept {
        if (!w || !w->focus_widget()) return;

        Widget* focus_widget = w->focus_widget();

        while (!empty()) {
            CharEvent e{focus_widget, pop_char()};
            send_event(focus_widget, &e);
        }
    }

} // namespace RolUI
