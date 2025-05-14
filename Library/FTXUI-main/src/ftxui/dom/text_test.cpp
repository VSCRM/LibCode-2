﻿// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <gtest/gtest.h>
#include <string>  // for allocator, string

#include "ftxui/dom/elements.hpp"   // for text, operator|, border, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen

// NOLINTBEGIN
namespace ftxui {

TEST(TextTest, ScreenHeightSmaller) {
  auto element = text("test");
  Screen screen(2, 0);
  Render(screen, element);

  EXPECT_EQ("", screen.ToString());
}

TEST(TextTest, ScreenSmaller) {
  auto element = text("test");
  Screen screen(2, 1);
  Render(screen, element);

  EXPECT_EQ("te", screen.ToString());
}

TEST(TextTest, ScreenFit) {
  auto element = text("test");
  Screen screen(4, 1);
  Render(screen, element);

  EXPECT_EQ("test", screen.ToString());
}

TEST(TextTest, ScreenBigger) {
  auto element = text("test");
  Screen screen(6, 1);
  Render(screen, element);

  EXPECT_EQ("test  ", screen.ToString());
}

TEST(TextTest, ScreenBigger2) {
  auto element = text("test");
  Screen screen(6, 2);
  Render(screen, element);

  EXPECT_EQ("test  \r\n      ", screen.ToString());
}

// See https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-504871456
TEST(TextTest, CJK) {
  auto element = text("жµ‹иЇ•") | border;
  Screen screen(6, 3);
  Render(screen, element);
  EXPECT_EQ(
      "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
      "в”‚жµ‹иЇ•в”‚\r\n"
      "в•°в”Ђв”Ђв”Ђв”Ђв•Ї",
      screen.ToString());
}

// See https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-504871456
TEST(TextTest, CJK_2) {
  auto element = text("жµ‹иЇ•") | border;
  Screen screen(5, 3);
  Render(screen, element);
  EXPECT_EQ(
      "в•­в”Ђв”Ђв”Ђв•®\r\n"
      "в”‚жµ‹иЇ•\r\n"
      "в•°в”Ђв”Ђв”Ђв•Ї",
      screen.ToString());
}

// See https://github.com/ArthurSonzogni/FTXUI/issues/2#issuecomment-504871456
TEST(TextTest, CJK_3) {
  auto element = text("жµ‹иЇ•") | border;
  Screen screen(4, 3);
  Render(screen, element);
  EXPECT_EQ(
      "в•­в”Ђв”Ђв•®\r\n"
      "в”‚жµ‹в”‚\r\n"
      "в•°в”Ђв”Ђв•Ї",
      screen.ToString());
}

TEST(TextTest, CombiningCharacters) {
  const std::string t =
      // Combining above:
      "aМ„aМЂaМЃaМ‚aМѓaМ„aМ…aМ†aМ‡aМ€aМ‰aМЉaМ‹aМЊaМЌaМЋaМЏaМђaМ‘aМ’aМ”aМ•aМљaМ›aМЅaМѕaМїaНЂaНЃaН‚aН†aНЉaН‹aНЊaНђ"
      "aН‘aН’aН—aНaН›aНќaНћaН aНЎaНЈaН¤aНҐaН¦aН§aНЁaН©aНЄaН«aН¬aН­aН®aНЇaб·Ђaб·Ѓaб·ѓaб·„aб·…aб·†aб·‡aб·€aб·‰aб·ѕaвѓђaвѓ‘aвѓ”"
      "aвѓ•aвѓ–aвѓ—aвѓ›aвѓњaвѓЎaвѓ©aвѓ°aпё aпёЎaпёўaпёЈ"
      // Combining middle:
      "aМґaМµaМ¶aМ·aМёaвѓ’aвѓ“aвѓaвѓ™aвѓљaвѓќaвѓћaвѓџaвѓҐaвѓ¦"
      // Combining below:
      "aМ—aМaМ™aМњaМќaМћaМџaМ aМЎaМўaМЈaМҐaМ¦aМ§aМЁaМ©aМЄaМ«aМ¬aМ­aМ®aМЇaМ°aМ±aМІaМіaМ№aМєaМ»aМјaН…aН‡aН€aН‰aНЌ"
      "aНЋaН“aН”aН•aН–aН™aНљaНњaНџaНўaб·‚aб·Љaб·їaвѓЁ";
  auto element = text(t);
  Screen screen(146, 1);
  Render(screen, element);
  EXPECT_EQ(t, screen.ToString());
}

TEST(TextTest, CombiningCharactersWithSpace) {
  const std::string t =
      // Combining above:
      "aМ„ aМЂ aМЃ aМ‚ aМѓ aМ„ aМ… aМ† aМ‡ aМ€ aМ‰ aМЉ aМ‹ aМЊ aМЌ aМЋ aМЏ aМђ aМ‘ aМ’ aМ” aМ• aМљ aМ› aМЅ aМѕ aМї aНЂ aНЃ aН‚ aН† aНЉ aН‹ aНЊ aНђ "
      "aН‘ aН’ aН— aН aН› aНќ aНћ aН  aНЎ aНЈ aН¤ aНҐ aН¦ aН§ aНЁ aН© aНЄ aН« aН¬ aН­ aН® aНЇ aб·Ђ aб·Ѓ aб·ѓ aб·„ aб·… aб·† aб·‡ aб·€ aб·‰ aб·ѕ aвѓђ aвѓ‘ aвѓ” "
      "aвѓ• aвѓ– aвѓ— aвѓ› aвѓњ aвѓЎ aвѓ© aвѓ° aпё  aпёЎ aпёў aпёЈ"
      // Combining middle:
      "aМґ aМµ aМ¶ aМ· aМё aвѓ’ aвѓ“ aвѓ aвѓ™ aвѓљ aвѓќ aвѓћ aвѓџ aвѓҐ aвѓ¦"
      // Combining below:
      "aМ— aМ aМ™ aМњ aМќ aМћ aМџ aМ  aМЎ aМў aМЈ aМҐ aМ¦ aМ§ aМЁ aМ© aМЄ aМ« aМ¬ aМ­ aМ® aМЇ aМ° aМ± aМІ aМі aМ№ aМє aМ» aМј aН… aН‡ aН€ aН‰ aНЌ "
      "aНЋ aН“ aН” aН• aН– aН™ aНљ aНњ aНџ aНў aб·‚ aб·Љ aб·ї aвѓЁ ";
  auto element = text(t);
  Screen screen(290, 1);
  Render(screen, element);
  EXPECT_EQ(t, screen.ToString());
}

}  // namespace ftxui
// NOLINTEND
