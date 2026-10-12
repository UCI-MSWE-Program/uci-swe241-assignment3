#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "sorting.cpp"

namespace {

int tests_run = 0;
int tests_failed = 0;

void ReportFailure(const char *file, int line, const std::string &message) {
  std::cerr << file << ":" << line << ": FAIL: " << message << '\n';
  tests_failed++;
}

#define ASSERT_TRUE(condition)                                              \
  do {                                                                      \
    if (!(condition)) {                                                     \
      ReportFailure(__FILE__, __LINE__, "expected true: " #condition);      \
    }                                                                       \
  } while (false)

std::string ToString(const std::string &value) { return value; }

template <typename T>
std::string ToString(const std::vector<std::vector<T>> &groups) {
  std::string result = "{";
  for (size_t i = 0; i < groups.size(); i++) {
    if (i != 0) {
      result += ", ";
    }
    result += "{";
    for (size_t j = 0; j < groups[i].size(); j++) {
      if (j != 0) {
        result += ", ";
      }
      result += ToString(groups[i][j]);
    }
    result += "}";
  }
  result += "}";
  return result;
}

using Groups = std::vector<std::vector<std::string>>;

// GroupAnagram returns groups in an unspecified order (unordered_map), so
// canonicalize both the outer and inner orderings before comparing.
Groups Canonicalize(Groups groups) {
  for (std::vector<std::string> &group : groups) {
    std::sort(group.begin(), group.end());
  }
  std::sort(groups.begin(), groups.end());
  return groups;
}

void AssertSameGroups(const Groups &expected, const Groups &actual,
                      const char *file, int line) {
  const Groups canonical_expected = Canonicalize(expected);
  const Groups canonical_actual = Canonicalize(actual);
  if (canonical_expected != canonical_actual) {
    ReportFailure(file, line,
        std::string("expected: ") + ToString(canonical_expected) +
            std::string(", actual: ") + ToString(canonical_actual));
  }
}

}

void TestEmptyInput() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {};
  AssertSameGroups(expected, sorting.GroupAnagram({}), __FILE__, __LINE__);
}

void TestSingleWord() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {{"hello"}};
  AssertSameGroups(expected, sorting.GroupAnagram({"hello"}), __FILE__,
      __LINE__);
}

void TestTwoAnagramsGroupedTogether() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {{"listen", "silent"}};
  AssertSameGroups(expected, sorting.GroupAnagram({"listen", "silent"}),
      __FILE__, __LINE__);
}

void TestMultipleGroupsWithMultipleAnagrams() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {
      {"eat", "tea", "ate"},
      {"tan", "nat"},
      {"bat"},
  };
  AssertSameGroups(expected,
      sorting.GroupAnagram({"eat", "tea", "tan", "ate", "nat", "bat"}),
      __FILE__, __LINE__);
}

void TestGroupingIsCaseInsensitive() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {{"EAT", "eat", "TEA"}};
  AssertSameGroups(expected,
      sorting.GroupAnagram({"EAT", "eat", "TEA"}),
      __FILE__, __LINE__);
}

void TestInputOrderPreservedWithinGroup() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups groups = sorting.GroupAnagram({"tea", "eat", "ate"});
  ASSERT_TRUE(groups.size() == 1);
  if (groups.size() != 1) {
    return;
  }
  const std::vector<std::string> expected = {"tea", "eat", "ate"};
  if (groups[0] != expected) {
    ReportFailure(__FILE__, __LINE__,
        std::string("expected group: {tea, eat, ate}, actual group: ") +
            ToString(Groups{groups[0]}));
  }
}

void TestNonAnagramWordsStaySeparate() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {{"cat"}, {"dog"}, {"bird"}};
  AssertSameGroups(expected,
      sorting.GroupAnagram({"cat", "dog", "bird"}), __FILE__, __LINE__);
}

void TestDuplicateWordsGroupTogether() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {{"solo", "solo", "loos"}};
  AssertSameGroups(expected,
      sorting.GroupAnagram({"solo", "loos", "solo"}), __FILE__, __LINE__);
}

void TestSingleCharacterWords() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {{"a"}, {"b"}, {"c"}};
  AssertSameGroups(expected, sorting.GroupAnagram({"a", "b", "c"}), __FILE__,
      __LINE__);
}

void TestEmptyStringWordsGroupTogether() {
  uci_swe241_assignment3::Sorting sorting;
  const Groups expected = {{"", ""}};
  AssertSameGroups(expected, sorting.GroupAnagram({"", ""}), __FILE__,
      __LINE__);
}

void TestAllSortMethodsProduceSameGroups() {
  const std::vector<std::string> words = {
      "care", "race", "Acre", "acre", "stressed", "desserts", "heart", "earth"};
  const Groups expected = {
      {"care", "race", "Acre", "acre"},
      {"stressed", "desserts"},
      {"heart", "earth"},
  };
  AssertSameGroups(expected,
      uci_swe241_assignment3::Sorting(
          uci_swe241_assignment3::Sorting::Method::kHeapSort)
          .GroupAnagram(words),
      __FILE__, __LINE__);
  AssertSameGroups(expected,
      uci_swe241_assignment3::Sorting(
          uci_swe241_assignment3::Sorting::Method::kRadixSort)
          .GroupAnagram(words),
      __FILE__, __LINE__);
  AssertSameGroups(expected,
      uci_swe241_assignment3::Sorting(
          uci_swe241_assignment3::Sorting::Method::kBucketSort)
          .GroupAnagram(words),
      __FILE__, __LINE__);
}

#define RUN_TEST(test)        \
  do {                        \
    test();                   \
    tests_run++;              \
  } while (false)

int main() {
  RUN_TEST(TestEmptyInput);
  RUN_TEST(TestSingleWord);
  RUN_TEST(TestTwoAnagramsGroupedTogether);
  RUN_TEST(TestMultipleGroupsWithMultipleAnagrams);
  RUN_TEST(TestGroupingIsCaseInsensitive);
  RUN_TEST(TestInputOrderPreservedWithinGroup);
  RUN_TEST(TestNonAnagramWordsStaySeparate);
  RUN_TEST(TestDuplicateWordsGroupTogether);
  RUN_TEST(TestSingleCharacterWords);
  RUN_TEST(TestEmptyStringWordsGroupTogether);
  RUN_TEST(TestAllSortMethodsProduceSameGroups);

  std::cout << tests_run << " tests ran, " << tests_failed << " failed\n";
  return tests_failed == 0 ? 0 : 1;
}
