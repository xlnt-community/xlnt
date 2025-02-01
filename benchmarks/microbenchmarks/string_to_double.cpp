// A core part of the xlsx parsing routine is taking strings from the xml parser and parsing these to a double
// this has a few requirements
// - expect strings in the form 1234.56 (i.e. no thousands seperator, '.' used for the decimal seperator)
// - handles atleast 15 significant figures (excel only serialises numbers up to 15sf)

#include <benchmark/benchmark.h>
#include <cstring>
#include <locale>
#include <random>
#include <sstream>
#include <detail/serialization/serialisation_helpers.hpp>
#include <xlnt/internal/features.hpp>

namespace {

// setup a large quantity of random doubles as strings
template <bool Decimal_Locale = true>
class RandomFloatStrs : public benchmark::Fixture
{
    static constexpr size_t Number_of_Elements = 1 << 20;
    static_assert(Number_of_Elements > 1'000'000, "ensure a decent set of random values is generated");

    std::vector<std::string> inputs;

    size_t index = 0;
    const char *locale_str = nullptr;

public:
    void SetUp(::benchmark::State &state)
    {
        locale_str = setlocale(LC_ALL, nullptr);
        setlocale(LC_ALL, "C");
        std::random_device rd; // obtain a seed for the random number engine
        std::mt19937 gen(rd());
        // doing full range is stupid (<double>::min/max()...), it just ends up generating very large numbers
        // uniform is probably not the best distribution to use here, but it will do for now
        std::uniform_real_distribution<double> dis(-1'000, 1'000);
        // generate a large quantity of doubles to deserialise
        inputs.reserve(Number_of_Elements);
        for (int i = 0; i < Number_of_Elements; ++i)
        {
            double d = dis(gen);
            char buf[16];
            snprintf(buf, 16, "%.15f", d);
            inputs.push_back(std::string(buf));
        }
        if (Decimal_Locale)
        {
            locale_str = setlocale(LC_ALL, "C");
        }
        else
        {
#ifdef XLNT_USE_LOCALE_COMMA_DECIMAL_SEPARATOR
            if (setlocale(LC_ALL, XLNT_LOCALE_COMMA_DECIMAL_SEPARATOR) == nullptr)
                state.SkipWithError(XLNT_LOCALE_COMMA_DECIMAL_SEPARATOR " locale not installed");
#else
            state.SkipWithError("Benchmarks that use a comma as decimal separator are disabled. Enable XLNT_USE_LOCALE_COMMA_DECIMAL_SEPARATOR if you want to run this benchmark.");
#endif
        }
    }

    void TearDown(const ::benchmark::State &state)
    {
        // restore locale
        setlocale(LC_ALL, locale_str);
        // gbench is keeping the fixtures alive somewhere, need to clear the data after use
        inputs = std::vector<std::string>{};
    }

    std::string &get_rand()
    {
        return inputs[++index & (Number_of_Elements - 1)];
    }
};

struct number_converter_production
{
    double stold(const std::string &s)
    {
        return xlnt::detail::deserialise(s);
    }
};

// method used by xlsx_consumer.cpp in commit - ba01de47a7d430764c20ec9ac9600eec0eb38bcf
// std::istringstream with the locale set to "C"
struct number_converter_stream
{
    number_converter_stream()
    {
        stream.imbue(std::locale::classic());
    }

    double stold(const std::string &s)
    {
        stream.str(s);
        stream.clear();
        stream >> result;
        return result;
    }

    std::istringstream stream;
    double result;
};


// To resolve the locale issue with strtod, a little preprocessing of the input is required.
// IMPORTANT: the string localeconv()->decimal_point might be longer than a single char
// in some locales (e.g. in the ps_AF locale which uses the arabic decimal separator).
struct number_converter_mk2
{
    explicit number_converter_mk2()
        : should_convert(strcmp(localeconv()->decimal_point, ".") != 0)
    {
    }

    double stold(std::string &s) const noexcept
    {
        assert(!s.empty());
        if (should_convert)
        {
            auto decimal_pt = std::find(s.begin(), s.end(), '.');
            if (decimal_pt != s.end())
            {
                s.replace(decimal_pt, decimal_pt + 1, localeconv()->decimal_point);
            }
        }
        return strtod(s.c_str(), nullptr);
    }

    double stold(const std::string &s) const
    {
        assert(!s.empty());
        if (!should_convert)
        {
            return strtod(s.c_str(), nullptr);
        }
        std::string copy(s);
        auto decimal_pt = std::find(copy.begin(), copy.end(), '.');
        if (decimal_pt != copy.end())
        {
            copy.replace(decimal_pt, decimal_pt + 1, localeconv()->decimal_point);
        }
        return strtod(copy.c_str(), nullptr);
    }

private:
    bool should_convert = false;
};

using RandFloatStrs = RandomFloatStrs<true>;
// german locale uses ',' as the seperator
using RandFloatCommaStrs = RandomFloatStrs<false>;
} // namespace

BENCHMARK_F(RandFloatStrs, double_from_string_sstream)
(benchmark::State &state)
{
    number_converter_stream converter;
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            converter.stold(get_rand()));
    }
}

// using strotod
// https://en.cppreference.com/w/cpp/string/byte/strtof
// this naive usage is broken in the face of locales (fails condition 1)
#include <cstdlib>
BENCHMARK_F(RandFloatStrs, double_from_string_strtod)
(benchmark::State &state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            strtod(get_rand().c_str(), nullptr));
    }
}

BENCHMARK_F(RandFloatStrs, double_from_string_strtod_fixed)
(benchmark::State &state)
{
    number_converter_mk2 converter;
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            converter.stold(get_rand()));
    }
}

BENCHMARK_F(RandFloatStrs, double_from_string_strtod_fixed_const_ref)
(benchmark::State &state)
{
    number_converter_mk2 converter;
    while (state.KeepRunning())
    {
        const std::string &inp = get_rand();
        benchmark::DoNotOptimize(
            converter.stold(inp));
    }
}

BENCHMARK_F(RandFloatStrs, double_from_string_production)
(benchmark::State &state)
{
    number_converter_production converter;
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            converter.stold(get_rand()));
    }
}

#if XLNT_HAS_FEATURE(TO_CHARS)
#include <charconv>
BENCHMARK_F(RandFloatStrs, double_from_string_std_from_chars)
(benchmark::State &state)
{
    while (state.KeepRunning())
    {
        const std::string &input = get_rand();
        double output;
        benchmark::DoNotOptimize(
            std::from_chars(input.data(), input.data() + input.size(), output));
    }
}
#endif

// not using the standard "C" locale with '.' seperator
BENCHMARK_F(RandFloatCommaStrs, double_from_string_strtod_fixed_comma_ref)
(benchmark::State &state)
{
    number_converter_mk2 converter;
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            converter.stold(get_rand()));
    }
}

BENCHMARK_F(RandFloatCommaStrs, double_from_string_strtod_fixed_comma_const_ref)
(benchmark::State &state)
{
    number_converter_mk2 converter;
    while (state.KeepRunning())
    {
        const std::string &inp = get_rand();
        benchmark::DoNotOptimize(
            converter.stold(inp));
    }
}

BENCHMARK_F(RandFloatCommaStrs, double_from_string_production_comma)
(benchmark::State &state)
{
    number_converter_production converter;
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(
            converter.stold(get_rand()));
    }
}
