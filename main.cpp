#include <iostream>
#include <vector>
#include "debruijn.h"

/// Generates a de Brujin sequence for an input alphabet
class derujin_seq_generator2
{
public:
    derujin_seq_generator2(std::vector<unsigned char> alphabet, size_t k)
        : _alphabet{ std::move(alphabet) }, _a{ std::vector<unsigned char>(k + 1, 0) }, _k{ k }
    {}

    std::vector<unsigned char> generate()
    {
        return _generate(1, 1);
    }

private:
    std::vector<unsigned char> _generate(size_t t, size_t p)
    {
        if (t > _k)
        {
            return std::vector<unsigned char>(&_a[1], &_a[p + 1]);
        }
        else
        {
            _a[t] = _a[t - p];
            auto out = _generate(t + 1, p);

            for (size_t j = _a[t - p] + 1; j < _alphabet.size(); ++j)
            {
                _a[t] = j;
                const auto temp = _generate(t + 1, t);
                std::copy(temp.begin(), temp.end(), std::back_inserter(out));
            }
            return out;
        }
    }

    std::vector<unsigned char> _alphabet;
    std::vector<unsigned char> _a;
    size_t _k;
};

class debrujin_seq_iterator
{
public:
    using value_type = fxt::ulong;

    /// WARNING: this constructor is supposed to be used only for
    /// the end() iterators
    debrujin_seq_iterator(fxt::ulong value)
        : _fxt_generator{0, 0}, _current_digit{ value }
    {}

    debrujin_seq_iterator(size_t n, size_t k)
        : _fxt_generator{n, k}, _n{ n }, _k{ k }
    {
        _current_digit = _fxt_generator.first_digit();
    }

    debrujin_seq_iterator(debrujin_seq_iterator&&) = default;
    ~debrujin_seq_iterator() noexcept = default;

    /// WARNING!
    /// this operator is supposed to be used only to compare against
    /// the end() iterator
    bool operator==(const debrujin_seq_iterator& rhs) const noexcept
    {
        return _current_digit == rhs._current_digit;
    }

    bool operator!=(const debrujin_seq_iterator& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    debrujin_seq_iterator& operator++()
    {
        _current_digit = _fxt_generator.next_digit();
        return *this;
    }

    value_type operator*() const noexcept
    {
        return _current_digit;
    }

private:
    fxt::debruijn _fxt_generator;
    fxt::ulong _current_digit;
    size_t _n;
    size_t _k;
};

/// Usage: for (auto x : debrujin_seq_generator(n, k)) { ... }
class debrujin_seq_generator
{
public:
    using iterator = debrujin_seq_iterator;

    debrujin_seq_generator(size_t n, size_t k)
        : _n{ n }, _k{ k }
    {}

    iterator begin()
    {
        return { _n, _k };
    }

    iterator end() const
    {
        return { _n };
    }

private:
    size_t _n;
    size_t _k;
};

int main()
{
    /*
    const std::vector<unsigned char> alphabet = {'A', 'C', 'G', 'T'};
    derujin_seq_generator dsg(alphabet, 4);
    const auto seq = dsg.generate();
    for (auto x : seq)
    {
        std::cout << alphabet[x];
    }
    std::cout << std::endl;
     */

    const size_t alphabet_size = 4;

    for (auto x : debrujin_seq_generator(alphabet_size, 10))
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}