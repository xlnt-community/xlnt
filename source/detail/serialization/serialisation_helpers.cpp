#include <detail/serialization/serialisation_helpers.hpp>
#include <detail/locale.hpp>
#include <detail/parsers.hpp>

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

double deserialise(const std::string &s, size_t *len_converted)
{
    assert(!s.empty());
    assert(len_converted != nullptr);
    *len_converted = 0;
    double d = std::numeric_limits<double>::quiet_NaN();
    detail::parse(s, d, len_converted);
    return d;
}

double deserialise(const std::string &s)
{
    size_t ignore;
    return deserialise(s, &ignore);
}

} // namespace detail
} // namespace xlnt
