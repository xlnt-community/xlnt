#include <detail/serialization/serialisation_helpers.hpp>
#include <detail/serialization/parsers.hpp>

#include <limits>
#include <cassert>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

namespace xlnt {
namespace detail {

std::string serialise(double d)
{
    return fmt::format("{}", d);
}

std::string serialise(float f)
{
    return fmt::format("{}", f);
}

template <typename T>
T deserialise(const std::string &s, size_t *num_characters_parsed)
{
    assert(!s.empty());
    T d = std::numeric_limits<T>::quiet_NaN();
    detail::parse(s, d, num_characters_parsed);
    return d;
}

template <typename T>
T deserialise(const char *s, const char **end)
{
    assert(s != nullptr);
    T d = std::numeric_limits<T>::quiet_NaN();
    detail::parse(s, d, end);
    return d;
}

} // namespace detail
} // namespace xlnt

template XLNT_API_INTERNAL double xlnt::detail::deserialise<double>(const std::string &s, size_t *num_characters_parsed);

template XLNT_API_INTERNAL float xlnt::detail::deserialise<float>(const std::string &s, size_t *num_characters_parsed);

template XLNT_API_INTERNAL double xlnt::detail::deserialise<double>(const char *s, const char **end);

template XLNT_API_INTERNAL float xlnt::detail::deserialise<float>(const char *s, const char **end);
