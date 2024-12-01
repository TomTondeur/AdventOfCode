export module aoc;

import std;

import <cstdint>;

namespace aoc
{
    // Utility to easily treat a file as an input range.
    struct BinFile
    {
        BinFile(std::ifstream f) : f(std::move(f))
        {
            if (!f.is_open() || !f.good())
            {
                throw std::runtime_error("File is not ready for reading.");
            }
        }

        auto begin()
        {
            return std::istreambuf_iterator<char>(f);
        }

        auto end()
        {
            return std::istreambuf_iterator<char>();
        }

        std::ifstream f;
    };

    // Function that takes an input file and produces a solution
    using solver = int64_t(*)(BinFile);

    // Find a solver for a given problem, this works a bit like std::unordere_map::operator[],
    // in the sense that it will create a slot to store the solver if necessary, and return a mutable reference.
    solver& get_solver(int year, int day, int subproblem)
    {
        static std::vector<solver> solvers;
        const int idx = ((year - 2023)*25 + (day - 1))*2 + (subproblem - 1);
        const int req_size = idx + 1;
        if (solvers.size() < req_size)
        {
            solvers.resize(req_size);
        }
        return solvers[idx];
    }

    // Run a previously registered solver
    export int64_t solve(int year, int day, int subproblem, int input)
    {
        const std::string path = std::format("input/aoc{}/day{:02d}_{}.txt", year - 2000, day, input);
        BinFile file{ std::ifstream{ std::move(path), std::ios::binary } };
        return get_solver(year, day, subproblem)(std::move(file));
    }

    // Create an object of this type to automagically register a function to solve a specific problem.
    struct create_solver
    {
        explicit create_solver(int year, int day, solver s)
        {
            get_solver(year, day, 1) = s;
            get_solver(year, day, 2) = s;
        }

        explicit create_solver(int year, int day, int subproblem, solver s)
        {
            get_solver(year, day, subproblem) = s;
        }
    };

    // Utility to lazily split a string that is easier to use than views::split.
    // Sequential separators are automatically skipped, empty entries are trimmed.
    struct split_str
    {
        struct Iter
        {
            using difference_type = std::ptrdiff_t;
            using value_type = std::string_view;

            Iter() = default;

            explicit Iter(std::string_view underlyingp, size_t offsetp, char split_charp)
                : underlying(underlyingp)
                , offset(offsetp)
                , next(underlyingp.find_first_of(split_charp, offsetp))
                , split_char(split_charp)
            {
                if (next == std::string::npos)
                {
                    next = underlying.size();
                }
                else if (offset == next)
                {
                    ++*this;
                }

                if (offset > next)
                {
                    offset = next;
                }
            }

            Iter& operator++()
            {
                *this = Iter(underlying, next + 1, split_char);
                return *this;
            }

            Iter operator++(int)
            {
                auto tmp = *this;
                ++*this;
                return tmp;
            }

            friend bool operator==(const Iter& a, const Iter& b)
            {
                return a.underlying == b.underlying
                    && a.offset == b.offset;
            }

            std::string_view operator*() const
            {
                return underlying.substr(offset, next - offset);
            }

            std::string_view underlying;
            size_t offset;
            size_t next;
            char split_char;
        };
        static_assert(std::input_iterator<Iter>);

        Iter begin() const
        {
            return Iter(underlying, 0ul, split_char);
        }

        Iter end() const
        {
            return Iter(underlying, underlying.size(), split_char);
        }

        split_str(std::string_view underlying, char split_char)
            : underlying(underlying)
            , split_char(split_char)
        {}

        std::string_view underlying;
        char split_char; // can't be const, otherwise we don't satisfy viewable_range
    };

    // Takes an input file and presents a range of lines in the text file. Lazily evaluated.
    // Uses std::getline under the hood to read a text file line-by-line.
    // The same buffer is re-used for each iteration, so you can't refer back to previous string_views later, you need to parse (or manually copy and buffer) each line as you receive it.
    // Handles both CRLF and LF-style newlines.
    struct read_lines
    {
        struct sentinel_type {};

        struct iterator_type
        {
            using difference_type = std::ptrdiff_t;
            using value_type = std::string;

            iterator_type() = default;
            iterator_type(const read_lines* parent, size_t pos = 0) : parent(parent), pos(pos) {}

            iterator_type& operator++()
            {
                if (parent && parent->read_one())
                {
                    ++pos;
                }
                else
                {
                    parent = nullptr;
                    pos = 0;
                }

                return *this;
            }

            // Pre-increment returns void - we are an input iterator, NOT a forward iterator!
            void operator++(int)
            {
                ++*this;
            }

            value_type& operator*() const
            {
                return parent->line_storage;
            }

            bool operator==(const iterator_type& b) const
            {
                const iterator_type& a = *this;
                return a.parent == b.parent && a.pos == b.pos;
            }

            const read_lines* parent = nullptr;
            size_t pos;
        };
        static_assert(std::input_iterator<iterator_type>);

        iterator_type begin() const
        {
            return iterator_type(this);
        }

        iterator_type end() const
        {
            return iterator_type(nullptr);
        }

        explicit read_lines(BinFile& input) : input(&input)
        {
            read_one();
        }

    private:

        bool read_one() const
        {
            if (input->f.eof())
            {
                return false;
            }

            std::getline(input->f, line_storage);
            if (!line_storage.empty() && line_storage.back() == '\r')
            {
                line_storage.resize(line_storage.size() - 1);
            }

            return true;
        }

        BinFile* input;
        mutable std::string line_storage; // Mama says it's just a little white lie so it wouldn't hurt nobody.
    };
    static_assert(std::ranges::input_range<read_lines>);

    // Utility to "duplicate" types in a parameter pack. E.g. Pack<3, int>::type = Pack<1, int, int, int>
    // Useful to then build something like "give me a tuple of 5 doubles".
    template<size_t N, typename T1, typename... Ts>
    struct Pack
    {
        using type = Pack<N - 1, T1, T1, Ts...>::type;
    };

    template<typename T1, typename... Ts>
    struct Pack<1, T1, Ts...>
    {
        using type = Pack<1, T1, Ts...>;
    };

    template<typename PackT>
    struct to_tuple_view;

    // Use partial specialization to extract the list of type parameters from a Pack
    template<typename... Ts>
    struct to_tuple_view<Pack<1, Ts...>>
    {
        template<std::ranges::viewable_range Rng>
        friend auto operator|(Rng&& rng, const to_tuple_view&)
        {
            auto b = rng.begin();
            auto e = rng.end();

            // list-initialization of std::tuple should guarantee left-to-right evaluation
            return std::tuple{ to_tuple_view::get_elem<Ts>(b++, e)... };
        }

        template<typename T, typename It, typename EndIt>
        static T get_elem(const It& it, const EndIt& end_it)
        {
            return it == end_it ? T{} : T{ *it };
        }
    };

    // Range adaptor that iterates through a viewable range and produces a tuple of the desired type.
    // This can make extracting elements from a range very compact (assuming your input is already validated).
    // Use like: auto [a, b, c] = split_str(my_line, ' ') | transform(parse) | to_tuple<int, int, int>()
    template<typename... Ts>
    using to_tuple = to_tuple_view<typename Pack<0, Ts...>::type>;

    // Range adaptor that iterates through a viewable range and produces a tuple of the desired type.
    // This can make extracting elements from a range very compact (assuming your input is already validated).
    // Use like: auto [a, b, c] = split_str(my_line, ' ') | transform(parse) | to_tuple<3, int>()
    template<size_t N, typename T>
    using to_sized_tuple = to_tuple_view<typename Pack<N, T>::type>;

    // Range adaptor that takes a range of T, and returns a range of pair<size_t, T>,
    // with the first item of each pair representing the 0-based index of the element.
    struct indexed : std::ranges::range_adaptor_closure<indexed>
    {
        template<std::ranges::viewable_range Rng>
        auto operator()(Rng&& rng)
        {
            return std::ranges::views::enumerate(std::forward<Rng>(rng));
        }
    };

}
