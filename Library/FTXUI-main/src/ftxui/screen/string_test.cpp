﻿// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/screen/string.hpp"
#include <gtest/gtest.h>
#include <string>  // for allocator, string
#include "ftxui/screen/string_internal.hpp"

namespace ftxui {

TEST(StringTest, StringWidth) {
  // Basic:
  EXPECT_EQ(0, string_width(""));
  EXPECT_EQ(1, string_width("a"));
  EXPECT_EQ(2, string_width("ab"));
  EXPECT_EQ(1, string_width("в¬¤"));

  // Fullwidth glyphs:
  EXPECT_EQ(2, string_width("жµ‹"));
  EXPECT_EQ(4, string_width("жµ‹иЇ•"));
  EXPECT_EQ(2, string_width("вљ«"));
  EXPECT_EQ(2, string_width("рџЄђ"));

  // Combining characters:
  EXPECT_EQ(1, string_width("aМ„"));
  EXPECT_EQ(1, string_width("aвѓ’"));
  EXPECT_EQ(1, string_width("aМ—"));
  // Control characters:
  EXPECT_EQ(0, string_width("\1"));
  EXPECT_EQ(2, string_width("a\1a"));
}

TEST(StringTest, Utf8ToGlyphs) {
  using T = std::vector<std::string>;
  // Basic:
  EXPECT_EQ(Utf8ToGlyphs(""), T({}));
  EXPECT_EQ(Utf8ToGlyphs("a"), T({"a"}));
  EXPECT_EQ(Utf8ToGlyphs("ab"), T({"a", "b"}));
  // Fullwidth glyphs:
  EXPECT_EQ(Utf8ToGlyphs("жµ‹"), T({"жµ‹", ""}));
  EXPECT_EQ(Utf8ToGlyphs("жµ‹иЇ•"), T({"жµ‹", "", "иЇ•", ""}));
  // Combining characters:
  EXPECT_EQ(Utf8ToGlyphs("aМ„"), T({"aМ„"}));
  EXPECT_EQ(Utf8ToGlyphs("aвѓ’"), T({"aвѓ’"}));
  EXPECT_EQ(Utf8ToGlyphs("aМ—"), T({"aМ—"}));
  // Control characters:
  EXPECT_EQ(Utf8ToGlyphs("\1"), T({}));
  EXPECT_EQ(Utf8ToGlyphs("a\1a"), T({"a", "a"}));
}

TEST(StringTest, GlyphCount) {
  // Basic:
  EXPECT_EQ(GlyphCount(""), 0);
  EXPECT_EQ(GlyphCount("a"), 1);
  EXPECT_EQ(GlyphCount("ab"), 2);
  // Fullwidth glyphs:
  EXPECT_EQ(GlyphCount("жµ‹"), 1);
  EXPECT_EQ(GlyphCount("жµ‹иЇ•"), 2);
  // Combining characters:
  EXPECT_EQ(GlyphCount("aМ„"), 1);
  EXPECT_EQ(GlyphCount("aвѓ’"), 1);
  EXPECT_EQ(GlyphCount("aМ—"), 1);
  // Control characters:
  EXPECT_EQ(GlyphCount("\1"), 0);
  EXPECT_EQ(GlyphCount("a\1a"), 2);
}

TEST(StringTest, GlyphIterate) {
  // Basic:
  EXPECT_EQ(GlyphIterate("", -1), 0);
  EXPECT_EQ(GlyphIterate("", 0), 0);
  EXPECT_EQ(GlyphIterate("", 1), 0);
  EXPECT_EQ(GlyphIterate("a", 0), 0);
  EXPECT_EQ(GlyphIterate("a", 1), 1);
  EXPECT_EQ(GlyphIterate("ab", 0), 0);
  EXPECT_EQ(GlyphIterate("ab", 1), 1);
  EXPECT_EQ(GlyphIterate("ab", 2), 2);
  EXPECT_EQ(GlyphIterate("abc", 0), 0);
  EXPECT_EQ(GlyphIterate("abc", 1), 1);
  EXPECT_EQ(GlyphIterate("abc", 2), 2);
  EXPECT_EQ(GlyphIterate("abc", 3), 3);
  // Fullwidth glyphs:
  EXPECT_EQ(GlyphIterate("жµ‹", 0), 0);
  EXPECT_EQ(GlyphIterate("жµ‹", 1), 3);
  EXPECT_EQ(GlyphIterate("жµ‹иЇ•", 0), 0);
  EXPECT_EQ(GlyphIterate("жµ‹иЇ•", 1), 3);
  EXPECT_EQ(GlyphIterate("жµ‹иЇ•", 2), 6);
  EXPECT_EQ(GlyphIterate("жµ‹иЇ•", 1, 3), 6);
  EXPECT_EQ(GlyphIterate("жµ‹иЇ•", 1, 0), 3);
  // Combining characters:
  EXPECT_EQ(GlyphIterate("aМ„", 0), 0);
  EXPECT_EQ(GlyphIterate("aМ„", 1), 3);
  EXPECT_EQ(GlyphIterate("aвѓ’aМ—aМ„", 0), 0);
  EXPECT_EQ(GlyphIterate("aвѓ’aМ—aМ„", 1), 4);
  EXPECT_EQ(GlyphIterate("aвѓ’aМ—aМ„", 2), 7);
  EXPECT_EQ(GlyphIterate("aвѓ’aМ—aМ„", 3), 10);
  // Control characters:
  EXPECT_EQ(GlyphIterate("\1", 0), 0);
  EXPECT_EQ(GlyphIterate("\1", 1), 1);
  EXPECT_EQ(GlyphIterate("a\1a", 0), 0);
  EXPECT_EQ(GlyphIterate("a\1a", 1), 2);
  EXPECT_EQ(GlyphIterate("a\1a", 2), 3);
}

TEST(StringTest, CellToGlyphIndex) {
  // Basic:
  auto basic = CellToGlyphIndex("abc");
  ASSERT_EQ(basic.size(), 3);
  EXPECT_EQ(basic[0], 0);
  EXPECT_EQ(basic[1], 1);
  EXPECT_EQ(basic[2], 2);

  // Fullwidth glyphs:
  auto fullwidth = CellToGlyphIndex("жµ‹иЇ•");
  ASSERT_EQ(fullwidth.size(), 4);
  EXPECT_EQ(fullwidth[0], 0);
  EXPECT_EQ(fullwidth[1], 0);
  EXPECT_EQ(fullwidth[2], 1);
  EXPECT_EQ(fullwidth[3], 1);

  // Combining characters:
  auto combining = CellToGlyphIndex("aвѓ’aМ—aМ„");
  ASSERT_EQ(combining.size(), 3);
  EXPECT_EQ(combining[0], 0);
  EXPECT_EQ(combining[1], 1);
  EXPECT_EQ(combining[2], 2);
}

TEST(StringTest, Utf8ToWordBreakProperty) {
  using T = std::vector<WordBreakProperty>;
  using P = WordBreakProperty;
  EXPECT_EQ(Utf8ToWordBreakProperty("a"), T({P::ALetter}));
  EXPECT_EQ(Utf8ToWordBreakProperty("0"), T({P::Numeric}));
  EXPECT_EQ(Utf8ToWordBreakProperty("Чђ"), T({P::Hebrew_Letter}));
  EXPECT_EQ(Utf8ToWordBreakProperty("г‡°"), T({P::Katakana}));
  EXPECT_EQ(Utf8ToWordBreakProperty(" "), T({P::WSegSpace}));
  EXPECT_EQ(Utf8ToWordBreakProperty("\""), T({P::Double_Quote}));
  EXPECT_EQ(Utf8ToWordBreakProperty("'"), T({P::Single_Quote}));
  EXPECT_EQ(Utf8ToWordBreakProperty(":"), T({P::MidLetter}));
  EXPECT_EQ(Utf8ToWordBreakProperty("."), T({P::MidNumLet}));
  EXPECT_EQ(Utf8ToWordBreakProperty("\r"), T({}));  // FIXME
  EXPECT_EQ(Utf8ToWordBreakProperty("\n"), T({P::LF}));
}

TEST(StringTest, to_string) {
  EXPECT_EQ(to_string(L"hello"), "hello");
  EXPECT_EQ(to_string(L"в‚¬"), "в‚¬");
  EXPECT_EQ(to_string(L"Гї"), "Гї");
  EXPECT_EQ(to_string(L"Яї"), "Яї");
  EXPECT_EQ(to_string(L"Й°Й±"), "Й°Й±");
  EXPECT_EQ(to_string(L"В«В»"), "В«В»");
  EXPECT_EQ(to_string(L"еµ°еµІеµ«"), "еµ°еµІеµ«");
  EXPECT_EQ(to_string(L"рџЋ…рџЋ„"), "рџЋ…рџЋ„");
}

TEST(StringTest, to_wstring) {
  EXPECT_EQ(to_wstring(std::string("hello")), L"hello");
  EXPECT_EQ(to_wstring(std::string("в‚¬")), L"в‚¬");
  EXPECT_EQ(to_wstring(std::string("Гї")), L"Гї");
  EXPECT_EQ(to_wstring(std::string("Яї")), L"Яї");
  EXPECT_EQ(to_wstring(std::string("Й°Й±")), L"Й°Й±");
  EXPECT_EQ(to_wstring(std::string("В«В»")), L"В«В»");
  EXPECT_EQ(to_wstring(std::string("еµ°еµІеµ«")), L"еµ°еµІеµ«");
  EXPECT_EQ(to_wstring(std::string("рџЋ…рџЋ„")), L"рџЋ…рџЋ„");
}

}  // namespace ftxui
