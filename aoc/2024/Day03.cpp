module aoc;

import std;

namespace aoc
{
    struct parser
    {
        int state = 0, do_state = 0;
        int64_t left_num = 0, right_num = 0, sum = 0;
        bool mul_enabled = true;

        // Evaluate mul(XXX,XXX), where 0 < XXX < 1000
        void handle_mul(char chr)
        {
            if (!mul_enabled) return;
            switch (state)
            {
            case 0: state = (chr == 'm') ? 1 : 0; break;
            case 1: state = (chr == 'u') ? 2 : 0; break;
            case 2: state = (chr == 'l') ? 3 : 0; break;
            case 3: state = (chr == '(') ? 4 : 0; left_num = 0; break;
            case 4:
                if (chr == ',')
                {
                    state = 5;
                    right_num = 0;
                }
                else if (chr >= '0' && chr <= '9' && left_num < 100)
                {
                    left_num = (left_num * 10) + (chr - '0');
                }
                else
                {
                    state = 0;
                }
                break;
            case 5:
                if (chr >= '0' && chr <= '9' && right_num < 100)
                {
                    right_num = (right_num * 10) + (chr - '0');
                }
                else
                {
                    state = 0;
                }

                if (chr == ')')
                {
                    sum += left_num * right_num;
                }
                break;
            }
        }

        // Enable/disable multiplication with do() and don't()
        void handle_do(char chr)
        {
            switch (do_state)
            {
            case 0: do_state = (chr == 'd') ? 1 : 0; break;
            case 1: do_state = (chr == 'o') ? 2 : 0; break;
            case 2: do_state = (chr == '(') ? 3 : (chr == 'n') ? 4 : 0; break;
            case 3: if (chr == ')') mul_enabled = true; do_state = 0; break;
            case 4: do_state = (chr == '\'') ? 5 : 0; break;
            case 5: do_state = (chr == 't') ? 6 : 0; break;
            case 6: do_state = (chr == '(') ? 7 : 0; break;
            case 7: if (chr == ')') mul_enabled = false; do_state = 0; break;
            }
        }
    };

    create_solver eval_muls(2024, 3, 1, +[](BinFile input) -> int64_t
    {
        parser p;
        for (char chr : input)
        {
            p.handle_mul(chr);
        }

        return p.sum;
    });

    create_solver eval_muls_conditionally(2024, 3, 2, +[](BinFile input) -> int64_t
    {
        parser p;
        for (char chr : input)
        {
            p.handle_mul(chr);
            p.handle_do(chr);
        }

        return p.sum;
    });
} // namespace aoc
