﻿// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <gtest/gtest.h>
#include <string>  // for allocator

#include "ftxui/dom/elements.hpp"  // for filler, operator|, text, border, dbox, hbox, vbox, Element
#include "ftxui/dom/node.hpp"       // for Render
#include "ftxui/screen/screen.hpp"  // for Screen

// NOLINTBEGIN
namespace ftxui {

TEST(DBoxTest, Basic) {
  auto root = dbox({
      hbox({
          text("test") | border,
          filler(),
      }),
      vbox({
          text("test") | border,
          filler(),
      }),
  });

  Screen screen(8, 4);
  Render(screen, root);
  EXPECT_EQ(screen.ToString(),
            "в•­в”Ђв”Ђв”Ђв”Ђв”¬в”Ђв•®\r\n"
            "в”‚testв”‚ в”‚\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв”ґв”Ђв•Ї\r\n"
            "в•°в”Ђв”Ђв”Ђв”Ђв•Ї  ");
}

}  // namespace ftxui
// NOLINTEND
