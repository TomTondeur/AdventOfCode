module aoc;

import std;

namespace aoc
{
    create_solver count_unique_visited_locations(2024, 6, 1, +[](BinFile input) -> int64_t
    {
        auto without_cr = input | std::ranges::views::filter([](char c) {return c != '\r'; });
        std::string full_map;
        full_map.append_range(without_cr);

        const int64_t vertical_step = full_map.find('\n') + 1;

        int64_t num_visited = 0ll;

        std::array<int64_t, 4> steps{-vertical_step, 1, vertical_step, -1};
        size_t direction_of_travel = 0;
        size_t current_position = full_map.find('^');
        
        do
        {
            char tmp = 'X';
            std::swap(full_map[current_position], tmp);
            switch (tmp)
            {
            case '^':
            case '.':
                ++num_visited;
                break;
            case 'X':
                break;
            case '#':
                // go back one square and turn right
                full_map[current_position] = '#';
                current_position -= steps[direction_of_travel % 4];
                ++direction_of_travel;
                break;
            default:
                throw std::runtime_error("Went out of bounds");
            }

            // advance to next square
            current_position += steps[direction_of_travel % 4];
        } while (current_position < full_map.size() && full_map[current_position] != '\n');

        return num_visited;
    });

    struct guard
    {
        std::unordered_set<size_t> visited;
        size_t current_position = 0ull;
        size_t next_position = 0ull;
        size_t current_position_hash = 0ull;
        size_t direction_of_travel = 0ull;

        template<typename VisitFunc>
        void walk_map(std::string& full_map, const VisitFunc& visit)
        {
            const int64_t vertical_step = full_map.find('\n') + 1;

            std::array<int64_t, 4> steps{ -vertical_step, 1, vertical_step, -1 };
            const size_t direction_mask = 1ull << 28ull;

            while (true)
            {
                // figure out where we're going next
                next_position = current_position + steps[direction_of_travel];
                if (next_position >= full_map.size() || full_map[next_position] == '\n')
                {
                    // we're about to run out of bounds: we're done
                    break;
                }

                current_position_hash = current_position | (direction_mask << direction_of_travel);
                if (!visit())
                {
                    return;
                }
                
                // record our movement
                visited.emplace(current_position_hash);

                if (full_map[next_position] == '#')
                {
                    // make a right turn
                    direction_of_travel = (direction_of_travel + 1) % 4;
                    continue;
                }

                current_position = next_position;
            }
        }
    };

    create_solver count_locations_where_obstacle_would_cause_infinite_loop(2024, 6, 2, +[](BinFile input) -> int64_t
    {
        auto without_cr = input | std::ranges::views::filter([](char c) {return c != '\r'; });
        std::string full_map;
        full_map.append_range(without_cr);
        std::string fake_obstacle_map(full_map);

        std::string original_map(full_map);

        int64_t num_found_locations = 0ll;
        const int64_t vertical_step = full_map.find('\n') + 1;

        guard real_guard, fake_guard;
        real_guard.current_position = full_map.find('^');
        real_guard.walk_map(full_map, [&]()
        {
            full_map[real_guard.current_position] = 'X';

            if (full_map[real_guard.next_position] == '.' && fake_obstacle_map[real_guard.next_position] != 'O')
            {
                full_map[real_guard.next_position] = '#';
                fake_guard.visited.clear();
                fake_guard.current_position = real_guard.current_position;
                fake_guard.direction_of_travel = real_guard.direction_of_travel;
                fake_guard.next_position = real_guard.next_position;
                fake_guard.walk_map(full_map, [&]()
                {
                    if (real_guard.visited.contains(fake_guard.current_position_hash) || fake_guard.visited.contains(fake_guard.current_position_hash))
                    {
                        fake_obstacle_map[real_guard.next_position] = 'O';
                        ++num_found_locations;
                        return false;
                    }

                    return true;
                });

                full_map[real_guard.next_position] = '.';
            }

            return true;
        });

        return num_found_locations;
    });
} // namespace aoc
