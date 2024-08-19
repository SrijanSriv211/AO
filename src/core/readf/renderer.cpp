#include "aopch.h"
#include "readf.h"

#include "console/console.h"
#include "strings/strings.h"

namespace console
{
    void readf::render_tokens()
    {
        // get all white points
        this->get_white_points();

        // poop through each token starting from first different token
        render_token(diff_token_idx.first, diff_token_idx.second);
        for (std::vector<lex::token>::size_type i = diff_token_idx.first + 1; i < this->lexer.tokens.size(); i++)
            render_token(i, 0);

        this->ren_text_buffer = this->text_buffer;
    }

    void readf::render_token(const int& token_idx, const int& char_idx)
    {
        lex::token token = this->lexer.tokens[token_idx];

        // EOL is useless so don't render it.
        if (token.type == lex::token_type::EOL)
            return;

        std::string Token = token.name.substr(char_idx, token.name.length());
        console::color c = console::color::WHITE;

        // print those tokens in color they are associated with
        if (this->color_codes.find(token.type) != this->color_codes.end())
            c = this->color_codes[token.type];

        else if (strings::any(token_idx, white_points))
            c = console::color::LIGHT_WHITE;

        else
            c = console::color::WHITE;

        console::print(Token, c, false);
    }

    void readf::clear_console()
    {
        // find the position where text buffer and rendered text buffer differ at.
        this->diff_token_idx = this->get_token_diff(text_buffer, ren_text_buffer);
        std::string concatenated_names;

        // concatenate names of tokens up to diff_token_idx.first
        for (int i = 0; i < diff_token_idx.first; i++)
            concatenated_names += this->lexer.tokens[i].name;

        // calculate diff_start by adding the length and diff_token_idx.second
        int diff_start = concatenated_names.length() + diff_token_idx.second;
        // get starting point of the difference to start clearing the screen from.
        int total_dist = this->init_cursor_pos.X + diff_start;

        // set cursor to the place where text buffer and rendered text buffer mismatched
        // then print whitespaces of length rendered text buffer's difference point
        // after that reset the cursor pos.
        this->set_cursor_position(total_dist);
        const int n_whitespaces = ren_text_buffer.substr(diff_start, ren_text_buffer.length()).length();
        std::cout << std::string(n_whitespaces, ' ');
        this->set_cursor_position(total_dist);
        // clear the error message log that was printed (if it was printed).
        this->clear_error_msg();
        this->set_cursor_position(total_dist);
    }

    void readf::update_console(const bool& render_suggestions)
    {
        if (this->text_buffer == this->ren_text_buffer)
            return;

        this->clear_console();
        this->render_tokens();

        if (!render_suggestions)
            return;

        // if the cursor has reached the bottom of the window, then move it up by one point.
        // to ensure that the cursor is not going beyond the window which will crash the program.
        if (this->vector3.y >= this->console_window_height() - 1)
        {
            this->vector3.y--;
            this->set_cursor_position(this->vector3.x);
            std::cout << std::endl;
        }
    }

    void readf::clear_error_msg()
    {
        std::cout << "\n" << std::string(this->console_window_width(), ' ');
    }

    // render tokens with white color on these points such as the very first token or the tokens successor of semicolon token.
    // this doesn't include whitespaces or the EOL token at the last of the tokens array, it only applies on renderable tokens.
    // basically they are white points
    void readf::get_white_points()
    {
        std::vector<lex::token_type> types(this->lexer.tokens.size());
        std::transform(this->lexer.tokens.begin(), this->lexer.tokens.end(), types.begin(), [](const lex::token& token) { return token.type; });

        bool pause_adding_points = false;

        for (std::vector<lex::token_type>::size_type i = 0; i < types.size(); i++)
        {
            lex::token_type tok_type = types[i];
            if (tok_type == lex::token_type::EOL)
            {
                white_points.push_back(i);
                break;
            }

            else if (tok_type != lex::token_type::WHITESPACE && tok_type != lex::token_type::SEMICOLON && !pause_adding_points)
            {
                white_points.push_back(i);
                pause_adding_points = true;
            }

            else if (tok_type == lex::token_type::SEMICOLON && pause_adding_points)
                pause_adding_points = false;
        }
    }
}
