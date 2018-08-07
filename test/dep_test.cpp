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
  auto tree = readDepTree("(the,DT,0,det)");
  EXPECT_EQ(1, tree.size());
  expectNode("the", "DT", "det", 0, tree[0]);

  tree = readDepTree("(the,DT,2,det),(president,NNP,0,root)");
  EXPECT_EQ(2, tree.size());
  expectNode("the", "DT", "det", 2, tree[0]);
  expectNode("president", "NNP", "root", 0, tree[1]);

  tree = readDepTree("(,,,,0,punct)");
  EXPECT_EQ(1, tree.size());
  expectNode(",", ",", "punct", 0, tree[0]);
}

TEST(DepTreeTests, ParseDepTokens) {
  auto tokens = parseDepTokens("(the,DT,2,det),(president,NNP,0,root)");
  EXPECT_EQ(2, tokens.size());
  EXPECT_EQ("the", tokens[0]);
  EXPECT_EQ("president", tokens[1]);
}

TEST(DepTreeTests, AddSpans) {
  auto tree = readDepTree("(the,DT,2,det),(president,NNP,0,root)");
  EXPECT_EQ(2, tree.size());
  EXPECT_EQ(0, tree[0].span.start);
  EXPECT_EQ(1, tree[0].span.end);
  EXPECT_EQ(0, tree[1].span.start);
  EXPECT_EQ(2, tree[1].span.end);

  tree = readDepTree("(her,NNP,2,compound),(majesty,NNP,0,root),(the,DT,4,det),(queen,NNP,2,dep),(of,IN,6,case),(denmark,NNP,4,nmod),(,,,,6,punct)");
  EXPECT_EQ(7, tree.size());
  EXPECT_EQ(0, tree[0].span.start);
  EXPECT_EQ(1, tree[0].span.end);
  EXPECT_EQ(0, tree[1].span.start);
  EXPECT_EQ(7, tree[1].span.end);
  EXPECT_EQ(2, tree[2].span.start);
  EXPECT_EQ(3, tree[2].span.end);
  EXPECT_EQ(2, tree[3].span.start);
  EXPECT_EQ(7, tree[3].span.end);
  EXPECT_EQ(4, tree[4].span.start);
  EXPECT_EQ(5, tree[4].span.end);
  EXPECT_EQ(4, tree[5].span.start);
  EXPECT_EQ(7, tree[5].span.end);
  EXPECT_EQ(6, tree[6].span.start);
  EXPECT_EQ(7, tree[6].span.end);
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
