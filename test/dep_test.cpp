#include "deptree.h"
#include "gtest/gtest.h"

namespace jhu::thrax {

namespace {

void expectNode(
    std::string_view token, std::string_view pos, std::string_view label,
    int head, DepNode actual) {
  EXPECT_EQ(token, actual.token);
  EXPECT_EQ(pos, actual.pos);
  EXPECT_EQ(label, actual.label);
  EXPECT_EQ(head, actual.head);
}

}

TEST(DepTreeTests, SplitIntoGroups) {
  auto result = split_into_groups("(the,DT,2,det)");
  EXPECT_EQ(1, result.size());
  EXPECT_EQ("the,DT,2,det", result[0]);

  result = split_into_groups("(the,DT,2,det),(president,NNP,0,root)");
  EXPECT_EQ(2, result.size());
  EXPECT_EQ("the,DT,2,det", result[0]);
  EXPECT_EQ("president,NNP,0,root", result[1]);
}

TEST(DepTreeTests, ParseNodes) {
  auto groups = split_into_groups("(the,DT,2,det)");
  auto tree = parse_nodes(groups);
  EXPECT_EQ(1, tree.size());
  expectNode("the", "DT", "det", 2, tree[0]);

  groups = split_into_groups("(the,DT,2,det),(president,NNP,0,root)");
  tree = parse_nodes(groups);
  EXPECT_EQ(2, tree.size());
  expectNode("the", "DT", "det", 2, tree[0]);
  expectNode("president", "NNP", "root", 0, tree[1]);

  groups = split_into_groups("(,,,,5,punct)");
  tree = parse_nodes(groups);
  EXPECT_EQ(1, tree.size());
  expectNode(",", ",", "punct", 5, tree[0]);
}

}
