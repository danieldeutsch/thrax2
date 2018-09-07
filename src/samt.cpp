#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <vector>

#include "filters.h"
#include "label.h"
#include "phrasalrule.h"
#include "sentence.h"
#include "tree.h"

namespace {

int count = 0;
std::mutex countLock, inputLock, outputLock;

bool valid(const jhu::thrax::PhrasalRule& rule) {
  return !isNonlexicalXRule(rule)
      && !isAbstract(rule)
      && withinTokenLimit(rule)
      && !isSlashedRule(rule)
      && !isConcatRule(rule)
      && !hasAnyXNT(rule);
}

bool process() {
  std::string line;
  {
    std::lock_guard g(inputLock);
    if (!std::getline(std::cin, line)) {
      return false;
    }
  }
  try {
    auto asp = jhu::thrax::readAlignedSentencePair<false, true>(line);
    auto initial = jhu::thrax::allConsistentPairs(asp, 20);
    auto tree = jhu::thrax::readTree(jhu::thrax::fields(line)[1]);
    auto label = jhu::thrax::SAMTLabeler{std::move(tree)};
    std::ostringstream out;
    for (auto& rule : jhu::thrax::extract(label, asp, std::move(initial))) {
      if (valid(rule)) {
        out << std::move(rule) << '\n';
      }
    }
    std::lock_guard g(outputLock);
    std::cout << out.str();
  } catch (std::exception& e) {
    std::cerr << e.what() << ' ' << line << '\n';
  }
  {
    std::lock_guard g(countLock);
    count++;
    if (count % 1000 == 0) {
      std::lock_guard g(outputLock);
      std::cerr << count << std::endl;
    }
  }
  return true;
}

}  // namespace

int main(int argc, char** argv) {
  int threads = 1;
  if (argc > 1) {
    threads = std::atoi(argv[1]);
  }
  if (threads < 2) {
    while (process()) {}
    return 0;
  }
  std::vector<std::future<void>> workers(threads);
  for (auto& t : workers) {
    t = std::async([]() { while (process()) {} });
  }
}
