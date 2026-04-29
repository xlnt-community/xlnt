// Copyright (c) 2014-2022 Thomas Fussell
// Copyright (c) 2024-2026 xlnt-community
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

#include <xlnt/packaging/uri.hpp>

class uri_test_suite : public test_suite
{
public:
    uri_test_suite()
    {
        register_test(test_default_ctor);
        register_test(test_parse_relative_path);
        register_test(test_parse_absolute_basic);
        register_test(test_parse_with_auth_query_fragment);
        register_test(test_parse_ipv6_host);
        register_test(test_parse_empty_query_and_fragment);
        register_test(test_parse_username_only);
        register_test(test_parse_network_path_reference);
        register_test(test_parse_empty_authority_file_uri);
        register_test(test_parse_query_only);
        register_test(test_parse_fragment_only);
        register_test(test_percent_encoding_preserved);
        register_test(test_to_string_round_trip);
        register_test(test_equality);
        register_test(test_make_absolute_simple);
        register_test(test_make_absolute_parent_segments);
        register_test(test_make_absolute_root_relative);
        register_test(test_make_reference_simple);
        register_test(test_make_reference_different_authority);
        register_test(test_make_reference_same_uri);
        register_test(test_rfc3986_resolution_examples);
    }

    void test_default_ctor()
    {
        xlnt::uri u;

        xlnt_assert(u.is_relative());
        xlnt_assert(!u.is_absolute());
        xlnt_assert_equals(u.scheme(), "");
        xlnt_assert_equals(u.authority(), "");
        xlnt_assert(!u.has_authentication());
        xlnt_assert_equals(u.authentication(), "");
        xlnt_assert_equals(u.username(), "");
        xlnt_assert_equals(u.password(), "");
        xlnt_assert_equals(u.host(), "");
        xlnt_assert(!u.has_port());
        xlnt_assert_equals(u.port(), std::size_t(0));
        xlnt_assert_equals(u.path().string(), "");
        xlnt_assert(!u.has_query());
        xlnt_assert_equals(u.query(), "");
        xlnt_assert(!u.has_fragment());
        xlnt_assert_equals(u.fragment(), "");
        xlnt_assert_equals(u.to_string(), "");
    }

    void test_parse_relative_path()
    {
        xlnt::uri u("folder/file.txt");

        xlnt_assert(u.is_relative());
        xlnt_assert_equals(u.scheme(), "");
        xlnt_assert_equals(u.authority(), "");
        xlnt_assert_equals(u.path().string(), "folder/file.txt");
        xlnt_assert_equals(u.to_string(), "folder/file.txt");
        xlnt_assert(!u.has_query());
        xlnt_assert(!u.has_fragment());
    }

    void test_parse_absolute_basic()
    {
        xlnt::uri u("http://example.com/document");

        xlnt_assert(u.is_absolute());
        xlnt_assert_equals(u.scheme(), "http");
        xlnt_assert_equals(u.authority(), "example.com");
        xlnt_assert_equals(u.host(), "example.com");
        xlnt_assert(!u.has_authentication());
        xlnt_assert(!u.has_port());
        xlnt_assert_equals(u.path().string(), "/document");
        xlnt_assert_equals(u.to_string(), "http://example.com/document");
    }

    void test_parse_with_auth_query_fragment()
    {
        xlnt::uri u("http://user:pass@example.com:80/document?v=1&x=3#abc");

        xlnt_assert(u.is_absolute());
        xlnt_assert_equals(u.scheme(), "http");
        xlnt_assert(u.has_authentication());
        xlnt_assert_equals(u.authentication(), "user:pass");
        xlnt_assert_equals(u.username(), "user");
        xlnt_assert_equals(u.password(), "pass");
        xlnt_assert_equals(u.host(), "example.com");
        xlnt_assert(u.has_port());
        xlnt_assert_equals(u.port(), std::size_t(80));
        xlnt_assert_equals(u.authority(), "user:pass@example.com:80");
        xlnt_assert_equals(u.path().string(), "/document");
        xlnt_assert(u.has_query());
        xlnt_assert_equals(u.query(), "v=1&x=3");
        xlnt_assert(u.has_fragment());
        xlnt_assert_equals(u.fragment(), "abc");
        xlnt_assert_equals(u.to_string(), "http://user:pass@example.com:80/document?v=1&x=3#abc");
    }

    void test_parse_ipv6_host()
    {
        xlnt::uri u("https://[::1]:443/index.html");

        xlnt_assert(u.is_absolute());
        xlnt_assert_equals(u.scheme(), "https");
        xlnt_assert_equals(u.host(), "::1");
        xlnt_assert(u.has_port());
        xlnt_assert_equals(u.port(), std::size_t(443));
        xlnt_assert_equals(u.authority(), "[::1]:443");
        xlnt_assert_equals(u.to_string(), "https://[::1]:443/index.html");
    }

    void test_parse_empty_query_and_fragment()
    {
        xlnt::uri u1("http://example.com/document?");
        xlnt_assert(u1.has_query());
        xlnt_assert_equals(u1.query(), "");
        xlnt_assert_equals(u1.to_string(), "http://example.com/document?");

        xlnt::uri u2("http://example.com/document#");
        xlnt_assert(u2.has_fragment());
        xlnt_assert_equals(u2.fragment(), "");
        xlnt_assert_equals(u2.to_string(), "http://example.com/document#");

        xlnt::uri u3("http://example.com/document?#");
        xlnt_assert(u3.has_query());
        xlnt_assert(u3.has_fragment());
        xlnt_assert_equals(u3.query(), "");
        xlnt_assert_equals(u3.fragment(), "");
        xlnt_assert_equals(u3.to_string(), "http://example.com/document?#");
    }

    void test_parse_username_only()
    {
        xlnt::uri u("http://user@example.com");

        xlnt_assert(u.is_absolute());
        xlnt_assert(u.has_authentication());
        xlnt_assert_equals(u.username(), "user");
        xlnt_assert_equals(u.password(), "");
        xlnt_assert_equals(u.authentication(), "user");
        xlnt_assert_equals(u.authority(), "user@example.com");
        xlnt_assert_equals(u.to_string(), "http://user@example.com");
    }

    void test_parse_network_path_reference()
    {
        xlnt::uri u("//example.com/path");

        xlnt_assert(u.is_relative());
        xlnt_assert_equals(u.scheme(), "");
        xlnt_assert_equals(u.authority(), "example.com");
        xlnt_assert_equals(u.host(), "example.com");
        xlnt_assert_equals(u.path().string(), "/path");
        xlnt_assert_equals(u.to_string(), "//example.com/path");
    }

    void test_parse_empty_authority_file_uri()
    {
        xlnt::uri u("file:///tmp/test.txt");

        xlnt_assert(u.is_absolute());
        xlnt_assert_equals(u.scheme(), "file");
        xlnt_assert_equals(u.authority(), "");
        xlnt_assert_equals(u.host(), "");
        xlnt_assert_equals(u.path().string(), "/tmp/test.txt");
        xlnt_assert_equals(u.to_string(), "file:///tmp/test.txt");
    }

    void test_parse_query_only()
    {
        xlnt::uri u("?x=1");

        xlnt_assert(u.is_relative());
        xlnt_assert_equals(u.path().string(), "");
        xlnt_assert(u.has_query());
        xlnt_assert_equals(u.query(), "x=1");
        xlnt_assert(!u.has_fragment());
        xlnt_assert_equals(u.to_string(), "?x=1");
    }

    void test_parse_fragment_only()
    {
        xlnt::uri u("#frag");

        xlnt_assert(u.is_relative());
        xlnt_assert_equals(u.path().string(), "");
        xlnt_assert(!u.has_query());
        xlnt_assert(u.has_fragment());
        xlnt_assert_equals(u.fragment(), "frag");
        xlnt_assert_equals(u.to_string(), "#frag");
    }

    void test_percent_encoding_preserved()
    {
        xlnt::uri u("http://example.com/a%20b");

        xlnt_assert_equals(u.path().string(), "/a%20b");
        xlnt_assert_equals(u.to_string(), "http://example.com/a%20b");
    }

    void test_to_string_round_trip()
    {
        const std::string input = "https://user:pw@example.org:8443/a/b/c?x=1&y=2#frag";
        xlnt::uri u(input);

        xlnt_assert_equals(u.to_string(), input);

        xlnt::uri round_trip(u.to_string());
        xlnt_assert_equals(round_trip.to_string(), input);
        xlnt_assert(round_trip == u);
    }

    void test_equality()
    {
        xlnt::uri a("http://example.com/a");
        xlnt::uri b("http://example.com/a");
        xlnt::uri c("http://example.com/b");

        xlnt_assert(a == b);
        xlnt_assert(!(a != b));
        xlnt_assert(a != c);
        xlnt_assert(!(a == c));
    }

    void test_make_absolute_simple()
    {
        xlnt::uri base("http://example.com/dir/index.html");
        xlnt::uri relative("child.txt");

        xlnt::uri abs = relative.make_absolute(base);

        xlnt_assert(abs.is_absolute());
        xlnt_assert_equals(abs.scheme(), "http");
        xlnt_assert_equals(abs.authority(), "example.com");
        xlnt_assert_equals(abs.path().string(), "/dir/child.txt");
        xlnt_assert_equals(abs.to_string(), "http://example.com/dir/child.txt");
    }

    void test_make_absolute_parent_segments()
    {
        xlnt::uri base("http://example.com/a/b/c/");
        xlnt::uri relative("../d/e.txt");

        xlnt::uri abs = relative.make_absolute(base);

        xlnt_assert(abs.is_absolute());
        xlnt_assert_equals(abs.path().string(), "/a/b/d/e.txt");
        xlnt_assert_equals(abs.to_string(), "http://example.com/a/b/d/e.txt");
    }

    void test_make_absolute_root_relative()
    {
        xlnt::uri base("http://example.com/a/b/c");
        xlnt::uri relative("/root/file.txt");

        xlnt::uri abs = relative.make_absolute(base);

        xlnt_assert(abs.is_absolute());
        xlnt_assert_equals(abs.path().string(), "/root/file.txt");
        xlnt_assert_equals(abs.to_string(), "http://example.com/root/file.txt");
    }

    void test_make_reference_simple()
    {
        xlnt::uri base("http://example.com/a/b/");
        xlnt::uri abs("http://example.com/a/b/c/d.txt");

        xlnt::uri ref = abs.make_reference(base);

        xlnt_assert(ref.is_relative());
        xlnt_assert_equals(ref.scheme(), "");
        xlnt_assert_equals(ref.authority(), "");
        xlnt_assert_equals(ref.path().string(), "c/d.txt");
        xlnt_assert_equals(ref.to_string(), "c/d.txt");
    }

    void test_make_reference_different_authority()
    {
        xlnt::uri base("http://example.com/a/b/");
        xlnt::uri abs("http://other.example.com/a/b/c.txt");

        xlnt::uri ref = abs.make_reference(base);

        xlnt_assert(ref.is_absolute());
        xlnt_assert_equals(ref.to_string(), "http://other.example.com/a/b/c.txt");
    }

    void test_make_reference_same_uri()
    {
        xlnt::uri base("http://example.com/a/b/");
        xlnt::uri abs("http://example.com/a/b/");

        xlnt::uri ref = abs.make_reference(base);

        xlnt_assert(ref.is_relative());
        xlnt_assert_equals(ref.path().string(), "");
        xlnt_assert_equals(ref.to_string(), "");
    }

    void test_rfc3986_resolution_examples()
    {
        xlnt::uri base("http://a/b/c/d;p?q");

        struct test_case
        {
            const char *relative;
            const char *expected;
        };

        const test_case cases[] =
        {
            {"g:h", "g:h"},
            {"g", "http://a/b/c/g"},
            {"./g", "http://a/b/c/g"},
            {"g/", "http://a/b/c/g/"},
            {"/g", "http://a/g"},
            {"//g", "http://g"},
            {"?y", "http://a/b/c/d;p?y"},
            {"g?y", "http://a/b/c/g?y"},
            {"#s", "http://a/b/c/d;p?q#s"},
            {"g#s", "http://a/b/c/g#s"},
            {"g?y#s", "http://a/b/c/g?y#s"},
            {";x", "http://a/b/c/;x"},
            {"g;x", "http://a/b/c/g;x"},
            {"", "http://a/b/c/d;p?q"},
            {".", "http://a/b/c/"},
            {"./", "http://a/b/c/"},
            {"..", "http://a/b/"},
            {"../", "http://a/b/"},
            {"../g", "http://a/b/g"},
            {"../..", "http://a/"},
            {"../../g", "http://a/g"}
        };

        for (const auto &test : cases)
        {
            xlnt::uri relative(test.relative);
            xlnt::uri resolved = relative.make_absolute(base);
            xlnt_assert_equals(resolved.to_string(), std::string(test.expected));
        }
    }
};

static uri_test_suite x;
