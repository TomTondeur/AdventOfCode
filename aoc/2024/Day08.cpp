module aoc;

import std;

namespace aoc
{
    int64_t do_find_antinodes(BinFile input, bool single_antinode_per_antenna)
    {
        using namespace std;

        auto without_cr = input| std::ranges::views::filter([](char c) {return c != '\r'; });

        std::string demo;
        demo.append_range(without_cr);

        size_t row_length = 0;
        size_t col_length = 0;
        unordered_map<char, vector<size_t>> antenna_locations;
        for (auto [idx, chr] : demo | indexed())
        {
            switch (chr)
            {
            case '\n':
                ++col_length;
                if (row_length == 0)
                {
                    row_length = idx;
                }
                break;
            case '.':
                break;
            default:
                antenna_locations[chr].emplace_back(idx);
                break;
            }
        }

        unordered_set<size_t> antinode_locations;

        auto get_coords = [=](size_t loc) { return std::tuple{ loc % (row_length + 1), loc / (row_length + 1) }; };
        for (const auto& [chr, locations] : antenna_locations)
        {
            for (size_t i = 0; i < locations.size(); ++i)
            {
                if (!single_antinode_per_antenna)
                {
                    auto [x, y] = get_coords(locations[i]);
                    antinode_locations.emplace(x | y << 16ull);
                    demo[x + y * (row_length + 1)] = '#';
                }
                for (size_t j = 0; j < locations.size(); ++j)
                {
                    if (i == j) continue;

                    auto [x1, y1] = get_coords(locations[i]);
                    const auto [x2, y2] = get_coords(locations[j]);
                    const auto dx = x2 - x1;
                    const auto dy = y2 - y1;

                    do
                    {
                        x1 -= dx;
                        y1 -= dy;

                        if (x1 < row_length && y1 < col_length)
                        {
                            antinode_locations.emplace(x1 | y1 << 16ull);
                            demo[x1 + y1 * (row_length + 1)] = '#';
                        }
                        else
                        {
                            break;
                        }
                    } while (!single_antinode_per_antenna);
                }
            }
        }

        return antinode_locations.size();
    }
    create_solver find_antinodes(2024, 8, 1, +[](BinFile input) -> int64_t
    {
        return do_find_antinodes(std::move(input), true);
    });

    create_solver pt2(2024, 8, 2, +[](BinFile input) -> int64_t
    {
        return do_find_antinodes(std::move(input), false);
    });
} // namespace aoc
