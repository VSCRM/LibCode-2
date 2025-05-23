﻿﻿// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>  // for max
#include <cstddef>    // for size_t
#include <memory>  // for __shared_ptr_access, shared_ptr, make_shared, allocator_traits<>::value_type
#include <utility>  // for move
#include <vector>   // for vector, __alloc_traits<>::value_type

#include "ftxui/dom/box_helper.hpp"   // for Element, Compute
#include "ftxui/dom/elements.hpp"     // for Element, Elements, hbox
#include "ftxui/dom/node.hpp"         // for Node, Elements
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/dom/selection.hpp"    // for Selection
#include "ftxui/screen/box.hpp"       // for Box
namespace ftxui {

namespace {
class HBox : public Node {
 public:
  explicit HBox(Elements children) : Node(std::move(children)) {}

 private:
  void ComputeRequirement() override {
    requirement_ = Requirement{};

    for (auto& child : children_) {
      child->ComputeRequirement();

      // Propagate the focused requirement.
      if (requirement_.focused.Prefer(child->requirement().focused)) {
        requirement_.focused = child->requirement().focused;
        requirement_.focused.box.Shift(requirement_.min_x, 0);
      }

      // Extend the min_x and min_y to contain all the children
      requirement_.min_x += child->requirement().min_x;
      requirement_.min_y =
          std::max(requirement_.min_y, child->requirement().min_y);
    }
  }

  void SetBox(Box box) override {
    Node::SetBox(box);

    std::vector<box_helper::Element> elements(children_.size());
    for (size_t i = 0; i < children_.size(); ++i) {
      auto& element = elements[i];
      const auto& requirement = children_[i]->requirement();
      element.min_size = requirement.min_x;
      element.flex_grow = requirement.flex_grow_x;
      element.flex_shrink = requirement.flex_shrink_x;
    }
    const int target_size = box.x_max - box.x_min + 1;
    box_helper::Compute(&elements, target_size);

    int x = box.x_min;
    for (size_t i = 0; i < children_.size(); ++i) {
      box.x_min = x;
      box.x_max = x + elements[i].size - 1;
      children_[i]->SetBox(box);
      x = box.x_max + 1;
    }
  }

  void Select(Selection& selection) override {
    // If this Node box_ doesn't intersect with the selection, then no
    // selection.
    if (Box::Intersection(selection.GetBox(), box_).IsEmpty()) {
      return;
    }

    Selection selection_saturated = selection.SaturateHorizontal(box_);
    for (auto& child : children_) {
      child->Select(selection_saturated);
    }
  }
};

}  // namespace

/// @brief A container displaying elements horizontally one by one.
/// @param children The elements in the container
/// @return The container.
///
/// #### Example
///
/// ```cpp
/// hbox({
///   text("Left"),
///   text("Right"),
/// });
/// ```
Element hbox(Elements children) {
  return std::make_shared<HBox>(std::move(children));
}

}  // namespace ftxui
