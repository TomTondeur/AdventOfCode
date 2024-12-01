module aoc;

import std;

namespace aoc
{
    create_solver sum_deltas(2024, 1, 1, +[](BinFile input) -> int64_t
    {
        using namespace std;
        using namespace std::ranges;

        // Read both columns into a std::vector

        vector<int64_t> series_1;
        vector<int64_t> series_2;

        for (string_view line : read_lines(input))
        {
            if (line.empty()) continue;

            auto numbers = split_str(line, ' ')
                | views::transform([](string_view num) {int64_t parsed; from_chars(num.data(), num.data() + num.size(), parsed); return parsed; })
                | to_sized_tuple<2, int64_t>();
            series_1.emplace_back(get<0>(numbers));
            series_2.emplace_back(get<1>(numbers));
        }

        // Sort each vector so that the closest items in each series are on the same index
        std::sort(series_1.begin(), series_1.end());
        std::sort(series_2.begin(), series_2.end());

        // Calculate the delta between each item, and sum up the deltas
        auto deltas = views::zip_transform([](int64_t a, int64_t b) { return abs(b - a); }, series_1, series_2);
        return std::reduce(deltas.begin(), deltas.end());
    });

    create_solver sum_occurrences(2024, 1, 2, +[](BinFile input) -> int64_t
    {
        using namespace std;
        using namespace std::ranges;

        // For the second part of the challenge, we need to keep track of how often each number occurs in each column.
        // The key in this map represents the number, the pair of ints in the value represent the number of occurrences in the left and right column, respectively.
        std::unordered_map<int64_t, std::tuple<int64_t, int64_t>> occurrence_counters;

        for (string_view line : read_lines(input))
        {
            if (line.empty()) continue;

            auto [left, right] = split_str(line, ' ')
                | views::transform([](string_view num) {int64_t parsed; std::from_chars(num.data(), num.data() + num.size(), parsed); return parsed; })
                | to_sized_tuple<2, int64_t>();

            ++std::get<0>(occurrence_counters[left]);
            ++std::get<1>(occurrence_counters[right]);
        }

        // We can find the similarity score for a number by simply multiplying its value by the number of occurrences in both columns.
        // Sum up similarity scores at the end for our final answer.
        auto occurrences = occurrence_counters | views::transform([](auto kvp) { return kvp.first * std::get<0>(kvp.second) * std::get<1>(kvp.second); });
        return std::reduce(occurrences.begin(), occurrences.end());
    });
} // namespace aoc
