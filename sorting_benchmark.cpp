#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "sorting.cpp"

namespace {

std::vector<std::string> GenerateWords(size_t count) {
  std::mt19937 rng(42);
  std::uniform_int_distribution<int> length_dist(3, 8);
  std::uniform_int_distribution<int> letter_dist(0, 25);
  std::vector<std::string> words;
  words.reserve(count);
  for (size_t i = 0; i < count; i++) {
    std::string word(static_cast<size_t>(length_dist(rng)), 'a');
    for (char &c : word) {
      c = static_cast<char>('a' + letter_dist(rng));
    }
    words.push_back(std::move(word));
  }
  return words;
}

void RunBenchmark(const char *label, size_t list_size) {
  using Sorting = uci_swe241_assignment3::Sorting;
  const std::vector<std::string> words = GenerateWords(list_size);
  std::cout << label << " (list size " << list_size << "):\n";
  const std::pair<const char *, Sorting::Method> methods[] = {
      {"heap sort", Sorting::Method::kHeapSort},
      {"radix sort", Sorting::Method::kRadixSort},
      {"bucket sort", Sorting::Method::kBucketSort},
  };
  for (const auto &[name, method] : methods) {
    Sorting sorting(method);
    const auto start = std::chrono::high_resolution_clock::now();
    const auto groups = sorting.GroupAnagram(words);
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration<double, std::micro>(end - start);
    std::cout << "  " << name << ": " << elapsed.count() << " us ("
              << groups.size() << " groups)\n";
  }
}

}

void SmallTest() { RunBenchmark("small", 100); }
void MediumTest() { RunBenchmark("medium", 1000); }
void LargeTest() { RunBenchmark("large", 10000); }

int main() {
  SmallTest();
  MediumTest();
  LargeTest();
  return 0;
}
