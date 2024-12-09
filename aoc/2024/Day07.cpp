module aoc;

import std;

namespace aoc
{
    namespace
    {
        int64_t svtoll(std::string_view sv)
        {
            int64_t parsed;
            std::from_chars(sv.data(), sv.data() + sv.size(), parsed);
            return parsed;
        }

        std::vector<int64_t> numbers;

        template<int64_t num_operators>
        int64_t get_equation_score(std::string_view line)
        {
            using namespace std;
            using namespace std::ranges;

            auto [result_str, numbers_str] = split_str(line, ':') | to_sized_tuple<2, string_view>();

            int64_t result = svtoll(result_str);

            auto numbers_range = split_str(numbers_str, ' ') | views::transform(svtoll);
            numbers.clear();
            numbers.insert(numbers.end(), numbers_range.begin(), numbers_range.end());
            if (numbers.empty())
            {
                return 0ll;
            }

            const size_t num_possible_combinations = static_cast<size_t>(pow(num_operators, numbers.size()));
            for (size_t combination = 0; combination < num_possible_combinations; ++combination)
            {
                size_t op = combination;
                int64_t found = std::accumulate(numbers.begin() + 1, numbers.end(), numbers.front(), [&](int64_t agg, int64_t cur)
                    {
                        auto op_to_use = op % num_operators;
                        op /= num_operators;
                        switch (op_to_use)
                        {
                        case 0:
                            return agg + cur;
                        case 1:
                            return agg * cur;
                        case 2:
                            return static_cast<int64_t>(agg * pow(10, floor(log10(cur) + 1))) + cur;
                        }
                    });
                if (found == result)
                {
                    return result;
                }
            }
            return 0ll;
        }
    }

    create_solver day7_1(2024, 7, 1, +[](BinFile input) -> int64_t
    {
        using namespace std;
        using namespace std::ranges;

        auto scores = read_lines(input) | views::transform(get_equation_score<2>);
        return std::reduce(scores.begin(), scores.end());
    });

    create_solver day7_2(2024, 7, 2, +[](BinFile input) -> int64_t
    {
        using namespace std;
        using namespace std::ranges;

        auto scores = read_lines(input) | views::transform(get_equation_score<3>);
        return std::reduce(scores.begin(), scores.end());
    });
} // namespace aoc
