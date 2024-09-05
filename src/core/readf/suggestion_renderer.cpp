#include "aopch.h"
#include "readf.h"

#include "console/console.h"
#include "strings/strings.h"
#include "array/array.h"

namespace console
{
    void readf::clear_suggestions()
    {
        if (array::is_empty(this->suitable_suggestions))
            return;

        std::string suitable_suggestions_s = strings::join("    ", this->suitable_suggestions); // _s means converted to string
        std::cout << "\n" + std::string(suitable_suggestions_s.size(), ' ');
        this->set_cursor_position(this->vector3.x);
    }

    void readf::render_suggestions()
    {
        if (strings::is_empty(this->text_buffer))
            return;

        std::cout << std::endl;
        console::color clr = console::color::GRAY;
        for (std::vector<std::string>::size_type i = 0; i < this->suitable_suggestions.size(); i++)
        {
            if (i == this->suggestion_idx)
            {
                suggestion = suitable_suggestions[i];
                clr = console::color::LIGHT_BLUE;
            }

            else
                clr = console::color::GRAY;

            console::print(this->suitable_suggestions[i], clr, false);
            std::cout << "    ";
        }
    }

    void readf::get_suitable_suggestions(const int& num_suggestions)
    {
        this->suitable_suggestions = {};
        int iter_size = std::min(12, static_cast<int>(suggestion_list.size()));

        for (int i = 0; i < iter_size; i++)
        {
            if (strings::contains_eachother(text_buffer, this->suggestion_list[i]))
                suitable_suggestions.push_back(this->suggestion_list[i]);
        }
    }
}
