﻿// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <gtest/gtest.h>

#include "ftxui/dom/elements.hpp"  // for LIGHT, flex, center, EMPTY, DOUBLE
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/dom/table.hpp"
#include "ftxui/screen/screen.hpp"  // for Screen

// NOLINTBEGIN
namespace ftxui {

TEST(TableTest, Empty) {
  auto table = Table();
  Screen screen(5, 5);
  Render(screen, table.Render());
  EXPECT_EQ(
      "     \r\n"
      "     \r\n"
      "     \r\n"
      "     \r\n"
      "     ",
      screen.ToString());
}

TEST(TableTest, Basic) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "abcd      \r\n"
      "efgh      \r\n"
      "ijkl      \r\n"
      "mnop      \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SeparatorVerticalEmpty) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().SeparatorVertical(EMPTY);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "a b c d   \r\n"
      "e f g h   \r\n"
      "i j k l   \r\n"
      "m n o p   \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SeparatorHorizontalEmpty) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().SeparatorHorizontal(EMPTY);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "abcd      \r\n"
      "          \r\n"
      "efgh      \r\n"
      "          \r\n"
      "ijkl      \r\n"
      "          \r\n"
      "mnop      \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SeparatorHorizontalLight) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().SeparatorHorizontal(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "abcd      \r\n"
      "в”Ђв”Ђв”Ђв”Ђ      \r\n"
      "efgh      \r\n"
      "в”Ђв”Ђв”Ђв”Ђ      \r\n"
      "ijkl      \r\n"
      "в”Ђв”Ђв”Ђв”Ђ      \r\n"
      "mnop      \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SeparatorVerticalLight) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().SeparatorVertical(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "aв”‚bв”‚cв”‚d   \r\n"
      "eв”‚fв”‚gв”‚h   \r\n"
      "iв”‚jв”‚kв”‚l   \r\n"
      "mв”‚nв”‚oв”‚p   \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SeparatorLight) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Separator(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "aв”‚bв”‚cв”‚d   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "eв”‚fв”‚gв”‚h   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "iв”‚jв”‚kв”‚l   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "mв”‚nв”‚oв”‚p   \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SeparatorVerticalHorizontalLight) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().SeparatorVertical(LIGHT);
  table.SelectAll().SeparatorHorizontal(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "aв”‚bв”‚cв”‚d   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "eв”‚fв”‚gв”‚h   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "iв”‚jв”‚kв”‚l   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "mв”‚nв”‚oв”‚p   \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SeparatorHorizontalVerticalLight) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().SeparatorHorizontal(LIGHT);
  table.SelectAll().SeparatorVertical(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "aв”‚bв”‚cв”‚d   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "eв”‚fв”‚gв”‚h   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "iв”‚jв”‚kв”‚l   \r\n"
      "в”Ђв”јв”Ђв”јв”Ђв”јв”Ђ   \r\n"
      "mв”‚nв”‚oв”‚p   \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, BorderLight) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”Ђв”Ђв”Ђв”ђ    \r\n"
      "в”‚abcdв”‚    \r\n"
      "в”‚efghв”‚    \r\n"
      "в”‚ijklв”‚    \r\n"
      "в”‚mnopв”‚    \r\n"
      "в””в”Ђв”Ђв”Ђв”Ђв”    \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, BorderSeparatorLight) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectAll().Separator(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”¬в”Ђв”¬в”Ђв”¬в”Ђв”ђ \r\n"
      "в”‚aв”‚bв”‚cв”‚dв”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚eв”‚fв”‚gв”‚hв”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚iв”‚jв”‚kв”‚lв”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚mв”‚nв”‚oв”‚pв”‚ \r\n"
      "в””в”Ђв”ґв”Ђв”ґв”Ђв”ґв”Ђв” \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectRow) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectRow(1).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      " abcd     \r\n"
      "в”Њв”Ђв”Ђв”Ђв”Ђв”ђ    \r\n"
      "в”‚efghв”‚    \r\n"
      "в””в”Ђв”Ђв”Ђв”Ђв”    \r\n"
      " ijkl     \r\n"
      " mnop     \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectRowNegative) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectRow(-2).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      " abcd     \r\n"
      " efgh     \r\n"
      "в”Њв”Ђв”Ђв”Ђв”Ђв”ђ    \r\n"
      "в”‚ijklв”‚    \r\n"
      "в””в”Ђв”Ђв”Ђв”Ђв”    \r\n"
      " mnop     \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectColumn) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectColumn(1).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      " в”Њв”Ђв”ђ      \r\n"
      "aв”‚bв”‚cd    \r\n"
      "eв”‚fв”‚gh    \r\n"
      "iв”‚jв”‚kl    \r\n"
      "mв”‚nв”‚op    \r\n"
      " в””в”Ђв”      \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectColumnNegative) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectColumn(-2).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "  в”Њв”Ђв”ђ     \r\n"
      "abв”‚cв”‚d    \r\n"
      "efв”‚gв”‚h    \r\n"
      "ijв”‚kв”‚l    \r\n"
      "mnв”‚oв”‚p    \r\n"
      "  в””в”Ђв”     \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, CrossingBorders) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectRow(1).Border(LIGHT);
  table.SelectColumn(1).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "  в”Њв”Ђв”ђ     \r\n"
      " aв”‚bв”‚cd   \r\n"
      "в”Њв”Ђв”јв”Ђв”јв”Ђв”Ђв”ђ  \r\n"
      "в”‚eв”‚fв”‚ghв”‚  \r\n"
      "в””в”Ђв”јв”Ђв”јв”Ђв”Ђв”  \r\n"
      " iв”‚jв”‚kl   \r\n"
      " mв”‚nв”‚op   \r\n"
      "  в””в”Ђв”     \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, CrossingBordersLightAndDouble) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectRow(1).Border(LIGHT);
  table.SelectColumn(1).Border(DOUBLE);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "  в•”в•ђв•—     \r\n"
      " aв•‘bв•‘cd   \r\n"
      "в”Њв”Ђв•«в”Ђв•«в”Ђв”Ђв”ђ  \r\n"
      "в”‚eв•‘fв•‘ghв”‚  \r\n"
      "в””в”Ђв•«в”Ђв•«в”Ђв”Ђв”  \r\n"
      " iв•‘jв•‘kl   \r\n"
      " mв•‘nв•‘op   \r\n"
      "  в•љв•ђв•ќ     \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectColumns) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectColumns(1, 2).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      " в”Њв”Ђв”Ђв”ђ     \r\n"
      "aв”‚bcв”‚d    \r\n"
      "eв”‚fgв”‚h    \r\n"
      "iв”‚jkв”‚l    \r\n"
      "mв”‚noв”‚p    \r\n"
      " в””в”Ђв”Ђв”     \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectRows) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectRows(1, 2).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      " abcd     \r\n"
      "в”Њв”Ђв”Ђв”Ђв”Ђв”ђ    \r\n"
      "в”‚efghв”‚    \r\n"
      "в”‚ijklв”‚    \r\n"
      "в””в”Ђв”Ђв”Ђв”Ђв”    \r\n"
      " mnop     \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectRectangle) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectRectangle(1, 2, 1, 2).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "a bc d    \r\n"
      " в”Њв”Ђв”Ђв”ђ     \r\n"
      "eв”‚fgв”‚h    \r\n"
      "iв”‚jkв”‚l    \r\n"
      " в””в”Ђв”Ђв”     \r\n"
      "m no p    \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectColumnsNegative) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectColumns(1, -1).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      " в”Њв”Ђв”Ђв”Ђв”ђ    \r\n"
      "aв”‚bcdв”‚    \r\n"
      "eв”‚fghв”‚    \r\n"
      "iв”‚jklв”‚    \r\n"
      "mв”‚nopв”‚    \r\n"
      " в””в”Ђв”Ђв”Ђв”    \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, SelectInverted) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectColumns(-1, 1).Border(LIGHT);
  Screen screen(10, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      " в”Њв”Ђв”Ђв”Ђв”ђ    \r\n"
      "aв”‚bcdв”‚    \r\n"
      "eв”‚fghв”‚    \r\n"
      "iв”‚jklв”‚    \r\n"
      "mв”‚nopв”‚    \r\n"
      " в””в”Ђв”Ђв”Ђв”    \r\n"
      "          \r\n"
      "          \r\n"
      "          \r\n"
      "          ",
      screen.ToString());
}

TEST(TableTest, ColumnFlex) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectAll().Separator(LIGHT);
  table.SelectColumn(1).Decorate(flex);
  Screen screen(20, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”¬в”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”¬в”Ђв”¬в”Ђв”ђ\r\n"
      "в”‚aв”‚b           в”‚cв”‚dв”‚\r\n"
      "в”њв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”јв”Ђв”јв”Ђв”¤\r\n"
      "в”‚eв”‚f           в”‚gв”‚hв”‚\r\n"
      "в”њв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”јв”Ђв”јв”Ђв”¤\r\n"
      "в”‚iв”‚j           в”‚kв”‚lв”‚\r\n"
      "в”њв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”јв”Ђв”јв”Ђв”¤\r\n"
      "в”‚mв”‚n           в”‚oв”‚pв”‚\r\n"
      "в””в”Ђв”ґв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”ґв”Ђв”ґв”Ђв”\r\n"
      "                    ",
      screen.ToString());
}

TEST(TableTest, ColumnFlexCenter) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectAll().Separator(LIGHT);
  table.SelectColumn(1).Decorate(flex);
  table.SelectColumn(1).DecorateCells(center);
  Screen screen(20, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”¬в”Ђв”¬в”Ђв”¬в”Ђв”ђ           \r\n"
      "в”‚aв”‚bв”‚cв”‚dв”‚           \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤           \r\n"
      "в”‚eв”‚fв”‚gв”‚hв”‚           \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤           \r\n"
      "в”‚iв”‚jв”‚kв”‚lв”‚           \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤           \r\n"
      "в”‚mв”‚nв”‚oв”‚pв”‚           \r\n"
      "в””в”Ђв”ґв”Ђв”ґв”Ђв”ґв”Ђв”           \r\n"
      "                    ",
      screen.ToString());
}

TEST(TableTest, ColumnCenter) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectAll().Separator(LIGHT);
  table.SelectColumn(1).DecorateCells(center);
  Screen screen(20, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”¬в”Ђв”¬в”Ђв”¬в”Ђв”ђ           \r\n"
      "в”‚aв”‚bв”‚cв”‚dв”‚           \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤           \r\n"
      "в”‚eв”‚fв”‚gв”‚hв”‚           \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤           \r\n"
      "в”‚iв”‚jв”‚kв”‚lв”‚           \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤           \r\n"
      "в”‚mв”‚nв”‚oв”‚pв”‚           \r\n"
      "в””в”Ђв”ґв”Ђв”ґв”Ђв”ґв”Ђв”           \r\n"
      "                    ",
      screen.ToString());
}

TEST(TableTest, ColumnFlexTwo) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectAll().Separator(LIGHT);
  table.SelectColumn(1).Decorate(flex);
  table.SelectColumn(3).Decorate(flex);
  Screen screen(20, 10);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”¬в”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”¬в”Ђв”¬в”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”ђ\r\n"
      "в”‚aв”‚b     в”‚cв”‚d      в”‚\r\n"
      "в”њв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”јв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”¤\r\n"
      "в”‚eв”‚f     в”‚gв”‚h      в”‚\r\n"
      "в”њв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”јв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”¤\r\n"
      "в”‚iв”‚j     в”‚kв”‚l      в”‚\r\n"
      "в”њв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”јв”Ђв”јв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”¤\r\n"
      "в”‚mв”‚n     в”‚oв”‚p      в”‚\r\n"
      "в””в”Ђв”ґв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”ґв”Ђв”ґв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”\r\n"
      "                    ",
      screen.ToString());
}

TEST(TableTest, RowFlex) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectAll().Separator(LIGHT);
  table.SelectRow(1).Decorate(flex);
  Screen screen(10, 20);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”¬в”Ђв”¬в”Ђв”¬в”Ђв”ђ \r\n"
      "в”‚aв”‚bв”‚cв”‚dв”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚eв”‚fв”‚gв”‚hв”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚iв”‚jв”‚kв”‚lв”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚mв”‚nв”‚oв”‚pв”‚ \r\n"
      "в””в”Ђв”ґв”Ђв”ґв”Ђв”ґв”Ђв” ",
      screen.ToString());
}

TEST(TableTest, RowFlexTwo) {
  auto table = Table({
      {"a", "b", "c", "d"},
      {"e", "f", "g", "h"},
      {"i", "j", "k", "l"},
      {"m", "n", "o", "p"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectAll().Separator(LIGHT);
  table.SelectRow(1).Decorate(flex);
  table.SelectRow(3).Decorate(flex);
  Screen screen(10, 20);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”¬в”Ђв”¬в”Ђв”¬в”Ђв”ђ \r\n"
      "в”‚aв”‚bв”‚cв”‚dв”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚eв”‚fв”‚gв”‚hв”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚iв”‚jв”‚kв”‚lв”‚ \r\n"
      "в”њв”Ђв”јв”Ђв”јв”Ђв”јв”Ђв”¤ \r\n"
      "в”‚mв”‚nв”‚oв”‚pв”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в”‚ в”‚ в”‚ в”‚ в”‚ \r\n"
      "в””в”Ђв”ґв”Ђв”ґв”Ђв”ґв”Ђв” ",
      screen.ToString());
}

TEST(TableTest, Merge) {
  auto table = Table({
      {"a", "b", "c"},
      {"d", "e", "f"},
      {"g", "h", "i"},
  });
  table.SelectAll().Border(LIGHT);
  table.SelectColumn(1).Border(HEAVY);
  table.SelectRow(1).Border(HEAVY);
  Screen screen(7, 7);
  Render(screen, table.Render());
  EXPECT_EQ(
      "в”Њв”Ђв”Ів”Ѓв”±в”Ђв”ђ\r\n"
      "в”‚aв”ѓbв”ѓcв”‚\r\n"
      "в”ўв”Ѓв•‹в”Ѓв•‹в”Ѓв”Є\r\n"
      "в”ѓdв”ѓeв”ѓfв”ѓ\r\n"
      "в”Ўв”Ѓв•‹в”Ѓв•‹в”Ѓв”©\r\n"
      "в”‚gв”ѓhв”ѓiв”‚\r\n"
      "в””в”Ђв”єв”Ѓв”№в”Ђв”",
      screen.ToString());
}

TEST(TableTest, Issue912) {
  Table({
      {"a"},
  });
  Table({
      {"a", "b"},
  });
  Table({
      {"a", "b", "c"},
  });
}

}  // namespace ftxui
// NOLINTEND
