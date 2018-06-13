#pragma once

#include <algorithm>
#include <cctype>
#include <functional>
#include <string_view>
#include <vector>

#include "alignment.h"

namespace jhu::thrax {

using Sentence = std::vector<std::string_view>;

constexpr auto isSpace = [](char c) {
  return std::isspace(static_cast<unsigned char>(c));
};

inline Sentence split(std::string_view line) {
  auto spaces = std::count_if(line.begin(), line.end(), isSpace);
  Sentence result;
  result.reserve(spaces + 1);  // estimate
  static const auto notSpace = std::not_fn(isSpace);
  auto s = std::find_if(line.begin(), line.end(), notSpace);
  for (auto e = s; s < line.end(); s = std::find_if(e, line.end(), notSpace)) {
    e = std::find_if(s, line.end(), isSpace);
    size_t len = std::distance(s, e);
    result.push_back({ s, len });
  }
  return result;
}

template<bool IsParsed>
Sentence tokens(std::string_view line) {
  auto sentence = split(line);
  if constexpr (!IsParsed) {
    return sentence;
  }
  auto it = std::remove_if(
      sentence.begin(),
      sentence.end(),
      [](auto t) { return t.front() == '('; });
  sentence.erase(it, sentence.end());
  for (auto& t : sentence) {
    while (!t.empty() && t.back() == ')') {
      t.remove_suffix(1);
    }
  }
  return sentence;
}

struct AlignedSentencePair {
  const Sentence src, tgt;
  const Alignment alignment;

  AlignedSentencePair(Sentence s, Sentence t, Alignment a)
      : src(std::move(s)), tgt(std::move(t)), alignment(std::move(a)) {
    if (!consistent()) {
      throw std::out_of_range("inconsistent aligned sentence pair");
    }
  }

 private:
  bool consistent() const {
    return std::all_of(
        alignment.begin(),
        alignment.end(),
        [this](auto p) {
          return p.src >= 0
              && p.tgt >= 0
              && p.src < src.size()
              && p.tgt < tgt.size();
        });
  }
};

}
