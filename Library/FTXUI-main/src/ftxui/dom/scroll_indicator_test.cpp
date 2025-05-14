﻿// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <gtest/gtest.h>
#include <string>   // for allocator, to_string, string
#include <utility>  // for move

#include "ftxui/dom/elements.hpp"  // for operator|, Element, operator|=, text, vbox, Elements, border, select, frame, vscroll_indicator
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for Color, Color::Red
#include "ftxui/screen/screen.hpp"  // for Screen

// NOLINTBEGIN
namespace ftxui {

namespace {
Element MakeVerticalList(int focused_index, int n) {
  Elements list;
  for (int i = 0; i < n; ++i) {
    auto element = text(std::to_string(i));
    if (i == focused_index) {
      element |= focus;
    }
    list.push_back(element);
  }
  return vbox(std::move(list)) | vscroll_indicator | frame | border;
}

Element MakeHorizontalList(int focused_index, int n) {
  Elements list;
  for (int i = 0; i < n; ++i) {
    auto element = text(std::to_string(i));
    if (i == focused_index) {
      element |= focus;
    }
    list.push_back(element);
  }
  return hbox(std::move(list)) | hscroll_indicator | frame | border;
}

std::string PrintVerticalList(int focused_index, int n) {
  auto element = MakeVerticalList(focused_index, n);
  Screen screen(6, 6);
  Render(screen, element);
  return screen.ToString();
}

std::string PrintHorizontalList(int focused_index, int n) {
  auto element = MakeHorizontalList(focused_index, n);
  Screen screen(6, 4);
  Render(screen, element);
  return screen.ToString();
}

}  // namespace

TEST(ScrollIndicator, BasicVertical) {
  EXPECT_EQ(PrintVerticalList(0, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚0  в”ѓв”‚\r\n"
            "в”‚1  в”ѓв”‚\r\n"
            "в”‚2   в”‚\r\n"
            "в”‚3   в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(1, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚0  в”ѓв”‚\r\n"
            "в”‚1  в”ѓв”‚\r\n"
            "в”‚2   в”‚\r\n"
            "в”‚3   в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(2, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚1  в”ѓв”‚\r\n"
            "в”‚2  в”ѓв”‚\r\n"
            "в”‚3   в”‚\r\n"
            "в”‚4   в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(3, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚2  в•»в”‚\r\n"
            "в”‚3  в”ѓв”‚\r\n"
            "в”‚4  в•№в”‚\r\n"
            "в”‚5   в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(4, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚3   в”‚\r\n"
            "в”‚4  в”ѓв”‚\r\n"
            "в”‚5  в”ѓв”‚\r\n"
            "в”‚6   в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(5, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚4   в”‚\r\n"
            "в”‚5  в•»в”‚\r\n"
            "в”‚6  в”ѓв”‚\r\n"
            "в”‚7  в•№в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(6, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚5   в”‚\r\n"
            "в”‚6   в”‚\r\n"
            "в”‚7  в”ѓв”‚\r\n"
            "в”‚8  в”ѓв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(7, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚6   в”‚\r\n"
            "в”‚7   в”‚\r\n"
            "в”‚8  в”ѓв”‚\r\n"
            "в”‚9  в”ѓв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(8, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚6   в”‚\r\n"
            "в”‚7   в”‚\r\n"
            "в”‚8  в”ѓв”‚\r\n"
            "в”‚9  в”ѓв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintVerticalList(9, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚6   в”‚\r\n"
            "в”‚7   в”‚\r\n"
            "в”‚8  в”ѓв”‚\r\n"
            "в”‚9  в”ѓв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
}

TEST(ScrollIndicator, VerticalColorable) {
  // The list we generate looks like this
  //           "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
  //           "в”‚0  в”ѓв”‚\r\n"
  //           "в”‚1  в”ѓв”‚\r\n"
  //           "в”‚2   в”‚\r\n"
  //           "в”‚3   в”‚\r\n"
  //           "в•°в”Ђв”Ђв”Ђв”Ђв•Ї"

  auto element = MakeVerticalList(0, 10) | color(Color::Red);
  Screen screen(6, 6);
  Render(screen, element);

  EXPECT_EQ(screen.PixelAt(4, 4).foreground_color, Color::Red);
  EXPECT_EQ(screen.PixelAt(4, 4).background_color, Color());
}

TEST(ScrollIndicator, VerticalBackgroundColorable) {
  // The list we generate looks like this
  //           "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
  //           "в”‚0  в”ѓв”‚\r\n"
  //           "в”‚1  в”ѓв”‚\r\n"
  //           "в”‚2   в”‚\r\n"
  //           "в”‚3   в”‚\r\n"
  //           "в•°в”Ђв”Ђв”Ђв”Ђв•Ї"

  auto element = MakeVerticalList(0, 10) | bgcolor(Color::Red);
  Screen screen(6, 6);
  Render(screen, element);

  EXPECT_EQ(screen.PixelAt(4, 4).foreground_color, Color());
  EXPECT_EQ(screen.PixelAt(4, 4).background_color, Color::Red);
}

TEST(ScrollIndicator, VerticalFullColorable) {
  // The list we generate looks like this
  //           "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
  //           "в”‚0  в”ѓв”‚\r\n"
  //           "в”‚1  в”ѓв”‚\r\n"
  //           "в”‚2   в”‚\r\n"
  //           "в”‚3   в”‚\r\n"
  //           "в•°в”Ђв”Ђв”Ђв”Ђв•Ї"

  auto element =
      MakeVerticalList(0, 10) | color(Color::Red) | bgcolor(Color::Red);
  Screen screen(6, 6);
  Render(screen, element);

  EXPECT_EQ(screen.PixelAt(4, 4).foreground_color, Color::Red);
  EXPECT_EQ(screen.PixelAt(4, 4).background_color, Color::Red);
}

TEST(ScrollIndicator, BasicHorizontal) {
  EXPECT_EQ(PrintHorizontalList(0, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚0123в”‚\r\n"
            "в”‚в”Ђв”Ђ  в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");

  EXPECT_EQ(PrintHorizontalList(1, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚0123в”‚\r\n"
            "в”‚в”Ђв”Ђ  в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");

  EXPECT_EQ(PrintHorizontalList(2, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚1234в”‚\r\n"
            "в”‚в”Ђв”Ђ  в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalList(3, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚2345в”‚\r\n"
            "в”‚в•¶в”Ђв•ґ в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalList(4, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚3456в”‚\r\n"
            "в”‚ в”Ђв”Ђ в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalList(5, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚4567в”‚\r\n"
            "в”‚ в•¶в”Ђв•ґв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalList(6, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚5678в”‚\r\n"
            "в”‚  в”Ђв”Ђв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalList(7, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚6789в”‚\r\n"
            "в”‚  в”Ђв”Ђв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalList(8, 10),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚6789в”‚\r\n"
            "в”‚  в”Ђв”Ђв”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
}

TEST(ScrollIndicator, HorizontalColorable) {
  // The list we generate looks like this
  //           "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
  //           "в”‚5678в”‚\r\n"
  //           "в”‚  в”Ђв”Ђв”‚\r\n"
  //           "в•°в”Ђв”Ђв”Ђв”Ђв•Ї"

  auto element = MakeHorizontalList(6, 10) | color(Color::Red);
  Screen screen(6, 4);
  Render(screen, element);

  EXPECT_EQ(screen.PixelAt(4, 2).foreground_color, Color::Red);
  EXPECT_EQ(screen.PixelAt(4, 2).background_color, Color());
}

TEST(ScrollIndicator, HorizontalBackgroundColorable) {
  // The list we generate looks like this
  //           "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
  //           "в”‚5678в”‚\r\n"
  //           "в”‚  в”Ђв”Ђв”‚\r\n"
  //           "в•°в”Ђв”Ђв”Ђв”Ђв•Ї"

  auto element = MakeHorizontalList(6, 10) | bgcolor(Color::Red);
  Screen screen(6, 4);
  Render(screen, element);

  EXPECT_EQ(screen.PixelAt(4, 2).foreground_color, Color());
  EXPECT_EQ(screen.PixelAt(4, 2).background_color, Color::Red);
}

TEST(ScrollIndicator, HorizontalFullColorable) {
  // The list we generate looks like this
  //           "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
  //           "в”‚5678в”‚\r\n"
  //           "в”‚  в”Ђв”Ђв”‚\r\n"
  //           "в•°в”Ђв”Ђв”Ђв”Ђв•Ї"

  auto element =
      MakeHorizontalList(6, 10) | color(Color::Red) | bgcolor(Color::Red);
  Screen screen(6, 4);
  Render(screen, element);

  EXPECT_EQ(screen.PixelAt(4, 2).foreground_color, Color::Red);
  EXPECT_EQ(screen.PixelAt(4, 2).background_color, Color::Red);
}

namespace {

Element MakeHorizontalFlexboxList(int n) {
  Elements list;
  for (int i = 0; i < n; ++i) {
    list.push_back(text(std::to_string(i % 10)));
  }
  return flexbox(std::move(list)) | vscroll_indicator | yframe | border;
}

std::string PrintHorizontalFlexboxList(int n) {
  auto element = MakeHorizontalFlexboxList(n);
  Screen screen(6, 6);
  Render(screen, element);
  return screen.ToString();
}

TEST(ScrollIndicator, HorizontalFlexbox) {
  EXPECT_EQ(PrintHorizontalFlexboxList(1),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚0   в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(2),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚01  в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(3),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012 в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(4),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012 в”‚\r\n"
            "в”‚3   в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(5),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012 в”‚\r\n"
            "в”‚34  в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(6),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012 в”‚\r\n"
            "в”‚345 в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в”‚    в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(11),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012 в”‚\r\n"
            "в”‚345 в”‚\r\n"
            "в”‚678 в”‚\r\n"
            "в”‚90  в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(15),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012в”ѓв”‚\r\n"
            "в”‚345в”ѓв”‚\r\n"
            "в”‚678в”ѓв”‚\r\n"
            "в”‚901в•№в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(16),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012в”ѓв”‚\r\n"
            "в”‚345в”ѓв”‚\r\n"
            "в”‚678в”ѓв”‚\r\n"
            "в”‚901 в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
  EXPECT_EQ(PrintHorizontalFlexboxList(17),
            "в•­в”Ђв”Ђв”Ђв”Ђв•®\r\n"
            "в”‚012в”ѓв”‚\r\n"
            "в”‚345в”ѓв”‚\r\n"
            "в”‚678в”ѓв”‚\r\n"
            "в”‚901 в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї");
}

}  // namespace

}  // namespace ftxui
// NOLINTEND
