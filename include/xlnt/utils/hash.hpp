// Copyright (c) 2024-2025 xlnt-community
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file

#pragma once

#include <functional>
#include <xlnt/styles/font.hpp>

// A standard, robust way to combine hash values.
// Often credited to Boost's hash_combine.
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {

template<>
struct hash<xlnt::color>
{
    size_t operator()(const xlnt::color& c) const
    {
        size_t seed = 0;
        // Start by hashing the type to prevent collisions between different color types
        // that might share an underlying value (e.g., theme(1) vs indexed(1)).
        hash_combine(seed, static_cast<int>(c.type()));

        // Hash auto color flag
        hash_combine(seed, c.auto_());

        // Hash tint if present
        if (c.has_tint())
        {
            hash_combine(seed, c.tint());
        }

        switch (c.type())
        {
            case xlnt::color_type::indexed:
                hash_combine(seed, c.indexed().index());
                break;
            case xlnt::color_type::theme:
                hash_combine(seed, c.theme().index());
                break;
            case xlnt::color_type::rgb:
            {
                const auto& rgb = c.rgb();
                hash_combine(seed, rgb.red());
                hash_combine(seed, rgb.green());
                hash_combine(seed, rgb.blue());
                hash_combine(seed, rgb.alpha());
                break;
            }
        }
        return seed;
    }
};

template<>
struct hash<xlnt::font>
{
    size_t operator()(const xlnt::font& f) const
    {
        size_t seed = 0;
        
        // Hash name
        if (f.has_name())
        {
            hash_combine(seed, f.name());
        }
        
        // Hash size
        if (f.has_size())
        {
            hash_combine(seed, f.size());
        }
        
        // Hash all boolean properties
        hash_combine(seed, f.bold());
        hash_combine(seed, f.italic());
        hash_combine(seed, f.superscript());
        hash_combine(seed, f.subscript());
        hash_combine(seed, f.strikethrough());
        hash_combine(seed, static_cast<int>(f.underline()));
        hash_combine(seed, f.outline());
        hash_combine(seed, f.shadow());
        
        // Hash scheme
        if (f.has_scheme())
        {
            hash_combine(seed, f.scheme());
        }
        
        // Hash color (importantly, this re-uses the std::hash<xlnt::color> we just defined)
        if (f.has_color())
        {
            hash_combine(seed, f.color());
        }
        
        // Hash family
        if (f.has_family())
        {
            hash_combine(seed, f.family());
        }
        
        // Hash charset
        if (f.has_charset())
        {
            hash_combine(seed, f.charset());
        }

        return seed;
    }
};

} // namespace std
