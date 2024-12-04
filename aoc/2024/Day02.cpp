module aoc;

import std;

namespace aoc
{
    bool is_valid_increment(int64_t delta) { return delta >= 1 && delta <= 3; }
    
    bool is_valid_delta(int64_t delta, bool incrementing) { return incrementing ? is_valid_increment(delta) : is_valid_increment(-delta); }

    bool is_valid_report(auto it, auto end_it, bool incrementing)
    {
        int64_t prev_level = *it;
        while (++it != end_it)
        {
            int64_t next_level = *it;
            if (!is_valid_delta(next_level - prev_level, incrementing))
            {
                return false;
            }
            prev_level = next_level;
        }

        return true;
    }

    bool is_safe_report(std::string_view report)
    {
        using namespace std;
        using namespace std::ranges;

        if (report.empty())
        {
            return false;
        }

        // A report is a series of space-separated numbers. Parse them out.
        auto levels = split_str(report, ' ')
            | views::transform([](std::string_view level) { int64_t parsed; std::from_chars(level.data(), level.data() + level.size(), parsed); return parsed; });

        // Look at the first two levels to determine if the sequence is incrementing or decrementing
        auto it = levels.begin();
        const int64_t prev_level = *it;
        const int64_t next_level = *(++it);
        const int64_t delta = next_level - prev_level;

        // Reports are only valid if all levels are strictly incrementing or strictly decrementing, and the delta between each level is no more than 3.
        bool incrementing = true;
        if (!is_valid_delta(delta, incrementing))
        {
            incrementing = false;
            if (!is_valid_delta(delta, incrementing))
            {
                return false;
            }
        }
        return is_valid_report(it, levels.end(), incrementing);
    }
    create_solver count_safe_reports(2024, 2, 1, +[](BinFile input) -> int64_t
    {
        auto reports = read_lines(input);
        return std::count_if(reports.begin(), reports.end(), &is_safe_report);
    });

    bool simple_at_most_one_away_from_safe_report(std::string_view report)
    {
        using namespace std;
        using namespace std::ranges;

        if (report.empty())
        {
            return false;
        }

        // A report is a series of space-separated numbers. Parse them out.
        auto levels = split_str(report, ' ')
            | views::transform([](std::string_view level) { int64_t parsed; std::from_chars(level.data(), level.data() + level.size(), parsed); return parsed; })
            |  to<std::vector>();

        for (auto to_skip = levels.begin(); to_skip != levels.end(); ++to_skip)
        {
            std::vector<int64_t> filtered{ levels };
            filtered.erase(filtered.begin() + std::distance(levels.begin(), to_skip));
            if (is_valid_report(filtered.begin(), filtered.end(), true)
                || is_valid_report(filtered.begin(), filtered.end(), false))
            {
                return true;
            }
        }
        return false;

    }

    create_solver count_almost_safe_reports(2024, 2, 2, +[](BinFile input) -> int64_t
    {
        auto reports = read_lines(input);
        return std::count_if(reports.begin(), reports.end(), &simple_at_most_one_away_from_safe_report);
    });
} // namespace aoc
