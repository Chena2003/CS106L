#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>
#include <cctype>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(const std::string& source) {
  auto t = find_all(source.begin(), source.end(), ::isspace);
  Corpus tokens;
  auto func = [&source](auto it1, auto it2) {
            return Token(source, it1, it2);
  };
  std::transform(t.begin(), t.end() - 1, t.begin() + 1, std::inserter(tokens, tokens.end()), func);
  std::erase_if(tokens, [](auto t) -> bool { return t.content.empty(); } );
  
  return tokens;
}

std::set<Mispelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  namespace rv = std::ranges::views;

  auto isMispelling = [&dictionary](const Token& t) {
    return !dictionary.contains(t.content);
  };

  auto toMispelling = [&dictionary] (const Token& t) {

    auto oneMispelling = [&t](auto& b) {
      return levenshtein(t.content, b) == 1;
    };

    auto view = dictionary | rv::filter(oneMispelling);
    std::set<std::string> suggestions(view.begin(), view.end());

    return Mispelling{t, suggestions};
  };

  auto hasSuggestion = [] (const Mispelling& t) {
    return !t.suggestions.empty();
  };

  auto view = source 
    | rv::filter(isMispelling)
    | rv::transform(toMispelling);

  auto vv = view | rv::filter(hasSuggestion);

  return std::set<Mispelling>(vv.begin(), vv.end());
};

/* Helper methods */

#include "utils.cpp"