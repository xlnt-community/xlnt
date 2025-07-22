// Copyright (c) 2025 xlnt-community
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

#include <xlnt/styles/color.hpp>
#include <xlnt/styles/font.hpp>

#include <unordered_set>
#include <vector>

class hash_test_suite : public test_suite
{
public:
    hash_test_suite()
    {
        register_test(test_color_hash_functionality);
        register_test(test_font_hash_functionality);
        register_test(test_hash_uniqueness);
        register_test(test_hash_consistency);
        register_test(test_font_special_attributes);
        register_test(test_color_special_attributes);
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

    void test_hash_uniqueness()
    {
        std::hash<xlnt::color> color_hasher;
        std::hash<xlnt::font> font_hasher;
        std::unordered_set<std::size_t> used_hashes;

        // Test color hash uniqueness with fixed values
        std::vector<xlnt::color> test_colors;
        
        // RGB colors with step of 20 to ensure diversity
        for (int r = 0; r <= 255; r += 20) {
            for (int g = 0; g <= 255; g += 20) {
                for (int b = 0; b <= 255; b += 20) {
                    test_colors.emplace_back(xlnt::rgb_color(static_cast<std::uint8_t>(r), 
                                                             static_cast<std::uint8_t>(g), 
                                                             static_cast<std::uint8_t>(b)));
                }
            }
        }
        
        // Indexed colors
        for (int i = 0; i <= 10; ++i) {
            test_colors.emplace_back(xlnt::indexed_color(i));
        }
        
        // Theme colors
        for (int i = 0; i <= 10; ++i) {
            test_colors.emplace_back(xlnt::theme_color(i));
        }

        // Check that all colors have unique hashes
        for (const auto& color : test_colors) {
            auto hash_val = color_hasher(color);
            xlnt_assert(used_hashes.find(hash_val) == used_hashes.end());
            used_hashes.insert(hash_val);
        }

        // Test font hash uniqueness
        std::vector<xlnt::font> test_fonts;
        std::vector<std::string> font_names = {"Arial", "Calibri", "Times New Roman", "Helvetica"};
        std::vector<double> font_sizes = {8.0, 10.0, 12.0, 14.0, 16.0, 18.0};
        
        for (const auto& name : font_names) {
            for (const auto& size : font_sizes) {
                for (int bold = 0; bold <= 1; ++bold) {
                    for (int italic = 0; italic <= 1; ++italic) {
                        xlnt::font font;
                        font.name(name).size(size).bold(bold == 1).italic(italic == 1);
                        test_fonts.push_back(font);
                    }
                }
            }
        }

        used_hashes.clear();
        for (const auto& font : test_fonts) {
            auto hash_val = font_hasher(font);
            xlnt_assert(used_hashes.find(hash_val) == used_hashes.end());
            used_hashes.insert(hash_val);
        }
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

    void test_font_special_attributes()
    {
        std::hash<xlnt::font> hasher;

        // Test fonts with different special attributes
        xlnt::font font1;
        
        xlnt::font font2;
        font2.superscript(true);
        xlnt_assert(hasher(font1) != hasher(font2));
        
        xlnt::font font3;
        font3.subscript(true);
        xlnt_assert(hasher(font1) != hasher(font3));
        xlnt_assert(hasher(font2) != hasher(font3));
        
        xlnt::font font4;
        font4.strikethrough(true);
        xlnt_assert(hasher(font1) != hasher(font4));
        
        xlnt::font font5;
        font5.underline(xlnt::font::underline_style::single);
        xlnt_assert(hasher(font1) != hasher(font5));
        
        xlnt::font font6;
        font6.outline(true);
        xlnt_assert(hasher(font1) != hasher(font6));
        
        xlnt::font font7;
        font7.shadow(true);
        xlnt_assert(hasher(font1) != hasher(font7));
        
        // Test font with family
        xlnt::font font8;
        font8.family(2);
        xlnt_assert(hasher(font1) != hasher(font8));
        
        // Test font with charset
        xlnt::font font9;
        font9.charset(1);
        xlnt_assert(hasher(font1) != hasher(font9));
        
        // Test font with scheme
        xlnt::font font10;
        font10.scheme("major");
        xlnt_assert(hasher(font1) != hasher(font10));
        
        // Test that same attributes produce same hash
        xlnt::font font11;
        font11.family(2);
        xlnt_assert_equals(hasher(font8), hasher(font11));
    }
    
    void test_color_special_attributes()
    {
        std::hash<xlnt::color> hasher;
        
        // Test auto colors
        xlnt::color auto_color1 = xlnt::color::red();
        auto_color1.auto_(true);
        
        xlnt::color auto_color2 = xlnt::color::red();
        auto_color2.auto_(false);
        
        xlnt_assert(hasher(auto_color1) != hasher(auto_color2));
        
        // Test colors with and without tint
        xlnt::color color1 = xlnt::color::blue();
        xlnt::color color2 = xlnt::color::blue();
        color2.tint(0.5);
        
        xlnt_assert(hasher(color1) != hasher(color2));
        
        // Test same tinted colors
        xlnt::color color3 = xlnt::color::blue();
        color3.tint(0.5);
        
        xlnt_assert_equals(hasher(color2), hasher(color3));
    }
};

static hash_test_suite x;
