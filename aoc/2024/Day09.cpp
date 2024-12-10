module aoc;

import std;

namespace aoc
{
    create_solver d9_1(2024, 9, 1, +[](BinFile input) -> int64_t
    {
        using namespace std;

        string dbg;
        string str;
        str.append_range(input);
        while (str.back() < '0' || str.back() > '9') str.resize(str.size() - 1);
        if (str.size() % 2 == 0) str.resize(str.size() - 1);

        size_t forward_it = 0, reverse_it = str.size() - 1;

        int64_t checksum = 0;
        size_t block_id = 0;
        while (true)
        {
            // forward_it is a file
            size_t file_id = forward_it / 2;
            size_t file_len = str[forward_it];
            for (char i = '0'; i < file_len; ++i)
            {
                dbg += file_id + '0';
                checksum += file_id * block_id++;
            }

            ++forward_it;
            // forward_it is a gap
            if (forward_it > reverse_it) break;

            FillGap:
            //reverse_it is a file
            file_id = reverse_it / 2;
            char delta = str[forward_it] - str[reverse_it];
            if (delta < 0)
            {
                // file bigger than gap
                for (char i = '0'; i < str[forward_it]; ++i)
                {
                    dbg += file_id + '0';
                    checksum += file_id * block_id++;
                }
                str[reverse_it] = '0' - delta;
            }
            else
            {
                // gap at least as big as file
                for (char i = '0'; i < str[reverse_it]; ++i)
                {
                    dbg += file_id + '0';
                    checksum += file_id * block_id++;
                }
                reverse_it -= 2;

                // gap bigger than file -> continue filling with the next file
                if (delta > 0 && reverse_it > forward_it)
                {
                    str[forward_it] = '0' + delta;
                    goto FillGap;
                }
            }

            ++forward_it;
        }

    return checksum;
    });

    create_solver d9_2(2024, 9, 2, +[](BinFile input) -> int64_t
    {
        using namespace std;

        string dbg;
        string str;
        str.append_range(input);
        while (str.back() < '0' || str.back() > '9') str.resize(str.size() - 1);
        if (str.size() % 2 == 0) str.resize(str.size() - 1);

        size_t forward_it = 0, reverse_it = str.size() - 1;

        int64_t checksum = 0;
        size_t block_id = 0;
        while (true)
        {
            // forward_it is a file
            size_t file_id = forward_it / 2;
            char file_len = str[forward_it];
            if (file_len < 0)
            {
                for (char i = '0'; i < file_len; ++i)
                {
                    dbg += '.';
                }
                block_id -= file_len;
            }
            for (char i = '0'; i < file_len; ++i)
            {
                dbg += file_id + '0';
                checksum += file_id * block_id++;
            }

            ++forward_it;
            // forward_it is a gap
            if (forward_it > reverse_it) break;

        FillGap:
            //reverse_it is a file
            file_id = reverse_it / 2;
            char delta = str[forward_it] - str[reverse_it];
            if (delta < 0)
            {
                // file bigger than gap
                for (size_t reverse_it2 = reverse_it - 2; reverse_it2 > forward_it; reverse_it2 -= 2)
                {
                    delta = str[forward_it] - str[reverse_it2];
                    if (str[reverse_it2] > '0' && delta >= 0)
                    {
                        file_id = reverse_it2 / 2;
                        for (char i = '0'; i < str[reverse_it2]; ++i)
                        {
                            dbg += file_id + '0';
                            checksum += file_id * block_id++;
                        }
                        str[reverse_it2] = -str[reverse_it2]; // flip this one, so we know it's a gap, even though it's where we would normally expect a file
                        str[forward_it] = '0' + delta;
                        if (delta == 0)
                        {
                            // done with this gap
                            goto GapFullyFilled;
                        }
                    }
                }

                // gap not fully filled: increment block id
                block_id += str[forward_it] - '0';
                for (char i = '0'; i < str[forward_it]; ++i) dbg += '.';
            }
            else
            {
                // gap at least as big as file
                for (char i = '0'; i < str[reverse_it]; ++i)
                {
                    dbg += file_id + '0';
                    checksum += file_id * block_id++;
                }
                reverse_it -= 2;

                // gap bigger than file -> continue filling with the next file
                if (delta > 0 && reverse_it > forward_it)
                {
                    str[forward_it] = '0' + delta;
                    goto FillGap;
                }
            }


        GapFullyFilled:
            ++forward_it;
        }

        return checksum;
    });
} // namespace aoc
