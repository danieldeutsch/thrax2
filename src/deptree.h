#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace jhu::thrax {

struct DepNode {
  std::string_view token;
  std::string_view pos;
  std::string_view label;
  int head;
};

using DepTree = std::vector<DepNode>;


// "(the,DT,2,det),(president,NNP,0,root)" ->
// ["the,DT,2,det", "president,NNP,0,root"]
inline std::vector<std::string_view> split_into_groups(std::string_view line) {
  // Remove the first '(' and last ')'
  line.remove_prefix(1);
  line.remove_suffix(1);
  // Split the string by "),("
  std::vector<std::string_view> result;
  while (true) {
    auto idx = line.find("),(");
    if (idx == std::string::npos) {
      result.push_back(line);
      break;
    }
    result.push_back(line.substr(0, idx));
    line.remove_prefix(idx + 3);
  }
  return result;
}

// "the,DT,2,det" -> DepNode("the", "DT", 2, "det")
inline DepTree parse_nodes(std::vector<std::string_view> groups) {
  DepTree tree;
  for (auto group : groups) {
    // Parse the group in reverse to avoid some complication with commas
    auto idx = group.rfind(',');
    auto label = group.substr(idx + 1);
    group.remove_suffix(label.size() + 1);

    idx = group.rfind(',');
    auto head_str = group.substr(idx + 1);
    auto head = std::stoi(std::string(head_str));
    group.remove_suffix(head_str.size() + 1);

    idx = group.rfind(',', group.size() - 2);
    auto pos = group.substr(idx + 1);
    group.remove_suffix(pos.size() + 1);

    auto token = group;
    tree.push_back(DepNode{token, pos, label, head});
  }
  return tree;
}

}
