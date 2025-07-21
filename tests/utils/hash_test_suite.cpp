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

#include <helpers/test_suite.hpp>
#include <helpers/timing.hpp>

#include <xlnt/styles/color.hpp>
#include <xlnt/styles/font.hpp>
#include <xlnt/utils/hash.hpp>

#include <chrono>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

class hash_test_suite : public test_suite
{
public:
    hash_test_suite()
    {
        register_test(test_color_hash_functionality);
        register_test(test_font_hash_functionality);
        register_test(test_color_hash_performance);
        register_test(test_font_hash_performance);
        register_test(test_hash_consistency);
    }

    void test_color_hash_functionality()
    {
        // Test that the same color produces the same hash
        xlnt::color color1 = xlnt::color::red();
        xlnt::color color2 = xlnt::color::red();
        std::hash<xlnt::color> hasher;

        xlnt_assert_equals(hasher(color1), hasher(color2));

        // Test that different colors produce different hashes
        xlnt::color color3 = xlnt::color::blue();
        xlnt_assert(hasher(color1) != hasher(color3));

        // Test indexed colors
        xlnt::color indexed1(xlnt::indexed_color(1));
        xlnt::color indexed2(xlnt::indexed_color(2));
        xlnt_assert(hasher(indexed1) != hasher(indexed2));

        // Test theme colors
        xlnt::color theme1(xlnt::theme_color(1));
        xlnt::color theme2(xlnt::theme_color(1));
        xlnt_assert_equals(hasher(theme1), hasher(theme2));

        // Test tinted colors
        xlnt::color tinted = xlnt::color::red();
        tinted.tint(0.5);
        xlnt_assert(hasher(color1) != hasher(tinted));
    }

    void test_font_hash_functionality()
    {
        std::hash<xlnt::font> hasher;

        // Test default font
        xlnt::font font1;
        xlnt::font font2;
        xlnt_assert_equals(hasher(font1), hasher(font2));

        // Test fonts with different names
        xlnt::font font3;
        font3.name("Arial");
        xlnt_assert(hasher(font1) != hasher(font3));

        // Test fonts with different sizes
        xlnt::font font4;
        font4.size(14.0);
        xlnt_assert(hasher(font1) != hasher(font4));

        // Test bold fonts
        xlnt::font font5;
        font5.bold(true);
        xlnt_assert(hasher(font1) != hasher(font5));

        // Test fonts with colors
        xlnt::font font6;
        font6.color(xlnt::color::red());
        xlnt_assert(hasher(font1) != hasher(font6));
    }

    void test_color_hash_performance()
    {
        const int test_size = 10000;
        std::vector<xlnt::color> colors;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        // Generate random colors
        for (int i = 0; i < test_size; ++i)
        {
            auto r = static_cast<std::uint8_t>(dis(gen));
            auto g = static_cast<std::uint8_t>(dis(gen));
            auto b = static_cast<std::uint8_t>(dis(gen));
            colors.emplace_back(xlnt::rgb_color(r, g, b));
        }

        // Test unordered_set insertion performance
        auto start = std::chrono::high_resolution_clock::now();

        std::unordered_set<xlnt::color> unique_colors;
        for (const auto &color : colors)
        {
            unique_colors.insert(color);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // Ensure performance is reasonable (each insertion should be less than 2 microseconds)
        xlnt_assert(static_cast<double>(duration.count()) / test_size < 2.0);
    }

    void test_font_hash_performance()
    {
        const int test_size = 10000;
        std::vector<xlnt::font> fonts;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(8, 72);
        std::uniform_int_distribution<> color_dis(0, 255);

        std::vector<std::string> font_names = {"Arial", "Calibri", "Times New Roman", "Helvetica", "Verdana"};
        std::uniform_int_distribution<> name_dis(0, font_names.size() - 1);

        // Generate random fonts
        for (int i = 0; i < test_size; ++i)
        {
            xlnt::font font;
            font.name(font_names[name_dis(gen)]);
            font.size(static_cast<double>(dis(gen)));
            font.bold(gen() % 2 == 0);
            font.italic(gen() % 2 == 0);

            auto r = static_cast<std::uint8_t>(color_dis(gen));
            auto g = static_cast<std::uint8_t>(color_dis(gen));
            auto b = static_cast<std::uint8_t>(color_dis(gen));
            font.color(xlnt::color(xlnt::rgb_color(r, g, b)));

            fonts.push_back(font);
        }

        // Test unordered_set insertion performance
        auto start = std::chrono::high_resolution_clock::now();

        std::unordered_set<xlnt::font> unique_fonts;
        for (const auto &font : fonts)
        {
            unique_fonts.insert(font);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // Ensure performance is reasonable (each insertion should be less than 4 microseconds, as font is more complex than color)
        xlnt_assert(static_cast<double>(duration.count()) / test_size < 4.0);
    }

    void test_hash_consistency()
    {
        // Test hash consistency
        std::hash<xlnt::color> color_hasher;
        std::hash<xlnt::font> font_hasher;

        xlnt::color color = xlnt::color::red();
        xlnt::font font;
        font.name("Arial").size(12.0).bold(true);

        // Calculating the hash of the same object multiple times should yield the same result
        auto color_hash1 = color_hasher(color);
        auto color_hash2 = color_hasher(color);
        auto font_hash1 = font_hasher(font);
        auto font_hash2 = font_hasher(font);

        xlnt_assert_equals(color_hash1, color_hash2);
        xlnt_assert_equals(font_hash1, font_hash2);

        // Test hash equality for equivalent objects
        xlnt::color color_copy = xlnt::color::red();
        xlnt::font font_copy;
        font_copy.name("Arial").size(12.0).bold(true);

        xlnt_assert_equals(color_hasher(color), color_hasher(color_copy));
        xlnt_assert_equals(font_hasher(font), font_hasher(font_copy));
    }
};

static hash_test_suite x;
