module aoc;

import std;

namespace aoc
{
    int count_needles(std::string_view haystack, std::string_view needle)
    {
        int count = -1;
        const char* hb = haystack.data();
        const char* he = &haystack.back() + 1;
        const auto nb = needle.begin(), ne = needle.end();
        auto next = hb;
        const char* found;
        
        do
        {
            ++count;
            found = std::search(next, he, nb, ne);
            next = found + needle.size();
        } while (found != he);

        return count;
    }

    int check(const std::deque<std::string>& lines, std::string_view needle, auto get_x)
    {
        for (size_t i = 0; i < needle.size(); ++i)
        {
            const size_t x = get_x(i);
            if (x > lines[i].size() || lines[i][x] != needle[i]) return 0;
        }

        return 1;
    }

    create_solver count_xmas(2024, 4, 1, +[](BinFile input) -> int64_t
    {
        using namespace std;

        const string_view xmas{ "XMAS"sv };
        const string_view samx{ "SAMX"sv };

        int64_t sum = 0;

        deque<string> lines;
        lines.emplace_back();

        while (getline(input.f, lines.back()))
        {
            auto& back = lines.back();
            if (back.back() == '\r') back.resize(back.size() - 1);

            // search horizontally
            sum += count_needles(back, xmas);
            sum += count_needles(back, samx);

            if (lines.size() < 4)
            {
                lines.emplace_back();
                continue;
            }

            // search across multiple lines
            for (size_t x = 0; x < back.size(); ++x)
            {
                for (std::string_view needle : {xmas, samx})
                {
                    sum += check(lines, needle, [=](size_t) { return x; }); // vertical
                    sum += check(lines, needle, [=](size_t i) { return x + i; }); // diagonal descending
                    sum += check(lines, needle, [=](size_t i) { return x + 3 - i; }); // diagonal ascending
                }
            }

            lines.emplace_back();
            swap(lines.front(), lines.back());
            lines.pop_front();
        }

        return sum;
    });

    create_solver count_cross_mas(2024, 4, 2, +[](BinFile input) -> int64_t
    {
        using namespace std;

        deque<string> lines;

        const std::string_view needle{ "MAS"sv };
        const std::string_view rev_needle{ "SAM"sv };
        const size_t ns = needle.size();

        while (lines.size() < ns)
        {
            lines.emplace_back();
            if (!getline(input.f, lines.back()))
            {
                return 0ll;
            }
        }

        int64_t sum = 0;
        do
        {
            for (size_t x = 0; x < lines[0].size() - ns; ++x)
            {
                const auto diagonal_descending = [=](size_t i) { return x + i; };
                const auto diagonal_ascending = [=](size_t i) { return x + ns - i - 1; };
                sum += (check(lines, needle, diagonal_ascending) | check(lines, rev_needle, diagonal_ascending))
                    & (check(lines, needle, diagonal_descending) | check(lines, rev_needle, diagonal_descending));
            }

            lines.emplace_back();
            swap(lines.front(), lines.back());
            lines.pop_front();
        } while (getline(input.f, lines.back()));
        return sum;
    });
} // namespace aoc
