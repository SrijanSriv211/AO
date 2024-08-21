#include "aopch.h"
#include "readf.h"

namespace console
{
    void readf::handle_ctrl_enter()
    {
        std::cout << "Test!";
    }

    void readf::handle_backspace()
    {
        if (this->vector3.i <= 0)
            return;

        this->vector3.i--;
        this->vector3.x--;
        text_buffer.erase(this->vector3.i, 1);

        this->update_console();
    }

    void readf::handle_left_arrow()
    {
        if (this->vector3.i <= 0)
            return;

        this->vector3.i--;
        this->vector3.x--;
    }

    void readf::handle_ctrl_left_arrow()
    {
        if (this->vector3.i <= 0)
            return;

        size_t whitespace_pos = text_buffer.rfind(' ', this->vector3.i - 1);
        if (whitespace_pos != std::string::npos && whitespace_pos == this->vector3.i - 1)
            handle_left_arrow();

        size_t previous_word_idx = text_buffer.rfind(' ', this->vector3.i - 1);
        if (previous_word_idx != std::string::npos)
        {
            int length = this->vector3.i - previous_word_idx - 1;

            this->vector3.x -= length;
            this->vector3.i -= length;
        }
    }

    void readf::handle_right_arrow()
    {
        if (this->vector3.i >= text_buffer.size())
            return;

        this->vector3.i++;
        this->vector3.x++;
    }

    void readf::handle_ctrl_right_arrow()
    {
        if (this->vector3.i >= text_buffer.size())
            return;

        size_t whitespace_pos = text_buffer.find(' ', this->vector3.x - init_cursor_pos.X);
        if (whitespace_pos != std::string::npos && whitespace_pos == this->vector3.x - init_cursor_pos.X)
            handle_right_arrow();

        size_t next_word_idx = text_buffer.find(' ', this->vector3.x - init_cursor_pos.X);
        if (next_word_idx != std::string::npos)
        {
            int length = next_word_idx == -1 ? text_buffer.size() - (this->vector3.x - init_cursor_pos.X) : next_word_idx - (this->vector3.x - init_cursor_pos.X);

            this->vector3.x += length;
            this->vector3.i += length;
        }
    }
}
