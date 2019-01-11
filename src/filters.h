#pragma once

#include "phrasalrule.h"

namespace jhu::thrax {

inline bool isNonlexicalXRule(const PhrasalRule& rule) {
  return !rule.lexical() && rule.lhs.label == "X";
}

inline bool isXNT(const NT& nt) {
  return nt.label == "X";
}

inline bool hasAnyXNT(const PhrasalRule& rule) {
  if (isXNT(rule.lhs)) {
    return true;
  }
  return std::any_of(
      rule.nts.begin(),
      rule.nts.begin() + rule.nextNT,
      [](const auto& nt) {
        return isXNT(nt);
      });
}

inline bool withinTokenLimit(const PhrasalRule& rule) {
  constexpr int kLexicalLimit = 7;
  constexpr int kNonlexLimit = 7;
  bool lex = rule.lexical();
  auto src = rule.numTokens<true>();
  auto tgt = rule.numTokens<false>();
  if (lex) {
    return src < kLexicalLimit && tgt < kLexicalLimit;
  } else {
    return src < kNonlexLimit && tgt < kNonlexLimit;
  }
}

inline bool isAbstract(const PhrasalRule& rule) {
  auto nts = rule.nextNT;
  if (nts == 0) {
    return false;
  }
  return rule.numTokens<true>() == nts && rule.numTokens<false>() == nts;
}

inline bool isSlashedNT(const NT& nt) {
  return nt.label.find("/") != std::string::npos ||
         nt.label.find("\\") != std::string::npos;
}

inline bool isSlashedRule(const PhrasalRule& rule) {
  if (isSlashedNT(rule.lhs)) {
    return true;
  }
  return std::any_of(
      rule.nts.begin(),
      rule.nts.end(),
      [](const auto& nt) {
        return isSlashedNT(nt);
      });
}

inline bool isConcatNT(const NT& nt) {
  return nt.label.find("+") != std::string::npos;
}

inline bool isConcatRule(const PhrasalRule& rule) {
  if (isConcatNT(rule.lhs)) {
    return true;
  }
  return std::any_of(
      rule.nts.begin(),
      rule.nts.end(),
      [](const auto& nt) {
        return isConcatNT(nt);
      });
}

}
