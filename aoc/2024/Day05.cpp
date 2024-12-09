module aoc;

import std;

namespace aoc
{
    struct ordering_tester
    {
        // TODO: Use std::generator when my compiler supports it
        template<typename TCollection,  typename TFunc, typename TElement = typename TCollection::value_type>
        void half_cross_join(const TCollection& c, const TFunc& f)
        {
            auto it1 = c.begin();

            for (auto& elem : c)
            {
                for (auto it2 = ++it1; it2 != c.end(); ++it2)
                {
                    f(std::tie(c, *it2));
                }
            }
        }

        struct hasher
        {
            size_t operator()(const std::pair<int64_t, int64_t>& x) const
            {
                return get<0>(x) ^ get<1>(x);
            }
        };

        std::unordered_set<std::pair<int64_t, int64_t>, hasher> strongly_ordered;

        void add_ordered_pair(int64_t l, int64_t r)
        {
            strongly_ordered.emplace(l, r);
        }

        int64_t middle_value(const std::vector<int64_t>& nums)
        {
            return nums[nums.size() / 2];
        }

        int64_t get_score(const std::vector<int64_t>& nums)
        {
            return is_correctly_ordered(nums) ? middle_value(nums) : 0ll;
        }

        bool is_correctly_ordered(const std::vector<int64_t>& nums)
        {
            if (nums.empty()) return 0;

            auto it1 = nums.begin();
            for (auto& elem : nums)
            {
                for (auto it2 = ++it1; it2 != nums.end(); ++it2)
                {
                    if (strongly_ordered.contains(std::pair{ *it2, elem }))
                    {
                        return 0ll;
                    }
                }
            }

            return middle_value(nums);
        }

        int64_t get_score_for_corrected_ordering(std::vector<int64_t>&& nums)
        {
            if (nums.empty()) return 0;

            auto it1 = nums.begin();
            for (auto& elem : nums)
            {
                for (auto it2 = ++it1; it2 != nums.end(); ++it2)
                {
                    if (strongly_ordered.contains(std::pair{ *it2, elem }))
                    {
                        // sort nums according to ordering rules
                        std::ranges::sort(nums, [this](int64_t a, int64_t b)
                            {
                                return strongly_ordered.contains(std::pair{ a, b }) || !strongly_ordered.contains(std::pair{ b, a });
                            });

                        return middle_value(nums);
                    }
                }
            }

            return 0ll;
        }
    };

    create_solver middle_page_numbers_of_correctly_ordered_lines(2024, 5, 1, +[](BinFile input) -> int64_t
    {
        using namespace std;
        using namespace std::ranges;
        
        int64_t sum = 0;

        ordering_tester tester;

        read_lines line_reader(input);
        auto line_it = line_reader.begin();

        for (; line_it != line_reader.end() && !(*line_it).empty(); ++line_it)
        {
            auto split_on_pipe = split_str(*line_it, '|');
            auto [l, r] = split_on_pipe
                | views::transform([](string_view s) { int64_t parsed; from_chars(s.data(), s.data() + s.size(), parsed); return parsed; })
                | to_sized_tuple<2, int64_t>();
            tester.add_ordered_pair(l, r);
        }

        vector<int64_t> current_line;
        for (++line_it; line_it != line_reader.end(); ++line_it)
        {
            auto split_on_comma = split_str(*line_it, ',');

            current_line.clear();
            for (auto part : split_on_comma)
            {
                int64_t parsed;
                from_chars(part.data(), part.data() + part.size(), parsed);
                current_line.emplace_back(parsed);
            }

            sum += tester.get_score(current_line);
        }

        return sum;
    });

    create_solver middle_page_numbers_of_incorrectly_ordered_lines_after_sorting(2024, 5, 2, +[](BinFile input) -> int64_t
        {
            using namespace std;
            using namespace std::ranges;

            int64_t sum = 0;

            ordering_tester tester;

            read_lines line_reader(input);
            auto line_it = line_reader.begin();

            for (; line_it != line_reader.end() && !(*line_it).empty(); ++line_it)
            {
                auto split_on_pipe = split_str(*line_it, '|');
                auto [l, r] = split_on_pipe
                    | views::transform([](string_view s) { int64_t parsed; from_chars(s.data(), s.data() + s.size(), parsed); return parsed; })
                    | to_sized_tuple<2, int64_t>();
                tester.add_ordered_pair(l, r);
            }

            vector<int64_t> current_line;
            for (++line_it; line_it != line_reader.end(); ++line_it)
            {
                auto split_on_comma = split_str(*line_it, ',');

                current_line.clear();
                for (auto part : split_on_comma)
                {
                    int64_t parsed;
                    from_chars(part.data(), part.data() + part.size(), parsed);
                    current_line.emplace_back(parsed);
                }

                sum += tester.get_score_for_corrected_ordering(std::move(current_line));
            }

            return sum;
        });
} // namespace aoc
