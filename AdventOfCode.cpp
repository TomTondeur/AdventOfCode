import aoc;

import std;

int main()
{
    for (int year = 2024; year < 2025; ++year)
    for (int day = 8; day < 26; ++day)
    for (int subproblem : {1, 2})
    for (int input : {1, 2})
        std::print("{}:{}:{}:{} = {}\n", year, day, subproblem, input, aoc::solve(year, day, subproblem, input));

    std::cout << std::endl;
    return 0;
}
