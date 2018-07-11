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
  auto result = splitIntoGroups("(the,DT,2,det)");
  EXPECT_EQ(1, result.size());
  EXPECT_EQ("the,DT,2,det", result[0]);

  result = splitIntoGroups("(the,DT,2,det),(president,NNP,0,root)");
  EXPECT_EQ(2, result.size());
  EXPECT_EQ("the,DT,2,det", result[0]);
  EXPECT_EQ("president,NNP,0,root", result[1]);
}

TEST(DepTreeTests, ParseNodes) {
  auto groups = splitIntoGroups("(the,DT,2,det)");
  auto tree = parseNodes(groups);
  EXPECT_EQ(1, tree.size());
  expectNode("the", "DT", "det", 2, tree[0]);

  groups = splitIntoGroups("(the,DT,2,det),(president,NNP,0,root)");
  tree = parseNodes(groups);
  EXPECT_EQ(2, tree.size());
  expectNode("the", "DT", "det", 2, tree[0]);
  expectNode("president", "NNP", "root", 0, tree[1]);

  groups = splitIntoGroups("(,,,,5,punct)");
  tree = parseNodes(groups);
  EXPECT_EQ(1, tree.size());
  expectNode(",", ",", "punct", 5, tree[0]);
}

TEST(DepTreeTests, ParseDepTokens) {
  auto tokens = parseDepTokens("(the,DT,2,det),(president,NNP,0,root)");
  EXPECT_EQ(2, tokens.size());
  EXPECT_EQ("the", tokens[0]);
  EXPECT_EQ("president", tokens[1]);
}

TEST(DepTreeTests, ReadAlignedDepSentencePair) {
  // The example was maniuplated for the unit test so the source and target
  // would have a different number of tokens and the alignments would not
  // be i-i.
  auto result = readAlignedDepSentencePair("el presidente de irlanda , a\t(the,DT,2,det),(president,NNP,0,root),(of,IN,4,case),(ireland,NNP,2,nmod),(,,,,4,punct)\t0-2 1-4 3-3 2-0 1-1");
  EXPECT_EQ(6, result.src.size());
  EXPECT_EQ("el", result.src[0]);
  EXPECT_EQ("presidente", result.src[1]);
  EXPECT_EQ("de", result.src[2]);
  EXPECT_EQ("irlanda", result.src[3]);
  EXPECT_EQ(",", result.src[4]);
  EXPECT_EQ("a", result.src[5]);

  EXPECT_EQ(5, result.tgt.size());
  EXPECT_EQ("the", result.tgt[0]);
  EXPECT_EQ("president", result.tgt[1]);
  EXPECT_EQ("of", result.tgt[2]);
  EXPECT_EQ("ireland", result.tgt[3]);
  EXPECT_EQ(",", result.tgt[4]);

  EXPECT_EQ(5, result.alignment.size());
}

}
