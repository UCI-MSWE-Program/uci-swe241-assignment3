#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace uci_swe241_assignment3 {

/**
 * Provides string sorting via heap sort, binary radix sort, or bucket sort,
 * along with anagram grouping built on top of those sorts.
 */
class Sorting {
 public:
  enum class Method {
    kHeapSort,
    kRadixSort,
    kBucketSort
  };

  /**
   * Constructs a Sorting instance.
   * @param sort_method The sorting algorithm used by SortString; defaults
   *        to Method::kHeapSort.
   * Time complexity: O(1).
   * Space complexity: O(1).
   */
  explicit Sorting(const Method sort_method = Method::kHeapSort)
      : sort_method_(sort_method) {}

  /**
   * Groups words that are anagrams of each other into the same vector.
   *
   * Each word is sorted by SortString and lowercased to derive its grouping
   * key; words sharing a key are grouped together, preserving the order in
   * which they appeared in the input. Groups are returned in unspecified
   * order.
   *
   * @param words The words to group.
   * @return A vector of groups, each containing all anagrams of one key.
   *
   * Time complexity: O(n * f(l)) where n is the number of words, l is the
   *        maximum word length, and f is the cost of SortString on one word
   *        (O(l log l) for heap sort, O(l) for radix/bucket sort).
   * Space complexity: O(L) where L is the total number of characters across
   *        all words, used for keys and result groups.
   */
  [[nodiscard]] std::vector<std::vector<std::string>> GroupAnagram(const
      std::vector<std::string> &words) const {
    std::unordered_map<std::string, std::vector<std::string>> group_counts = {};
    for (const std::string &word : words) {
      std::string sorted_word = this->SortString(word);
      std::ranges::transform(sorted_word, sorted_word.begin(), [](const char &c) {
          return std::tolower(c);
      });
      if (!group_counts.contains(sorted_word)) {
        group_counts[sorted_word] = std::vector<std::string>{};
      }
      group_counts[sorted_word].push_back(word);
    }
    std::vector<std::vector<std::string>> result = {};
    for (const auto &value: group_counts | std::views::values) {
      result.push_back(value);
    }
    return result;
  }

  /**
   * Sorts the characters of a string in ascending order using the sort
   * method chosen at construction.
   *
   * @param s The string to sort (taken by value; the caller's copy is
   *        left unchanged).
   * @return The sorted string.
   *
   * Time complexity: that of the configured method (HeapSort, RadixSort,
   *        or BucketSort).
   * Space complexity: that of the configured method.
   */
  [[nodiscard]] std::string SortString(std::string s) const {
    if (sort_method_ == Method::kHeapSort) {
      return HeapSort(s);
    }
    if (sort_method_ == Method::kRadixSort) {
      return RadixSort(s);
    }
    return BucketSort(s);
  }

  /**
   * Sorts a string's characters in ascending order using heap sort.
   *
   * First rearranges the string into a max-heap, then repeatedly moves the
   * maximum (front) element to the end of the shrinking heap, restoring the
   * heap property with a sift-down after each extraction.
   *
   * @param s The string to sort, modified in place.
   * @return The sorted string.
   *
   * Time complexity: O(n log n) for n characters.
   * Space complexity: O(1) auxiliary; sorting is done in place.
   */
  static std::string HeapSort(std::string &s) {
    // Anonymous local function
    auto sift_down = [&s](const size_t start, const size_t heap_size) {
      size_t curr = start;
      while (true) {
        size_t largest = curr;
        const size_t left_child = 2 * curr + 1;
        const size_t right_child = 2 * curr + 2;
        if (left_child < heap_size && s[largest] < s[left_child]) {
          largest = left_child;
        }
        if (right_child < heap_size && s[largest] < s[right_child]) {
          largest = right_child;
        }
        if (largest == curr) {
          break;
        }
        std::swap(s[curr], s[largest]);
        curr = largest;
      }
    };
    if (s.size() < 2) {
      return s;
    }
    // Reorder string into max-heap.
    // s.size() / 2 to s.size() are all leaf nodes.
    // We only need to start with s.size() / 2 - 1, then work our way up the parent nodes.
    for (size_t start = s.size() / 2; start > 0; start--) {
      sift_down(start - 1, s.size());
    }
    // Pop max elements of max-heap to end of string thus converting to
    // a sorted string (ascending order).
    for (size_t heap_size = s.size(); heap_size > 1; heap_size--) {
      std::swap(s[0], s[heap_size - 1]);
      sift_down(0, heap_size - 1);
    }
    return s;
  }

  /**
   * Sorts a string's characters in ascending order using least-significant
   * digit (LSD) binary radix sort. Works with standard ascii 0 - 127.
   *
   * Performs one stable pass per bit (7 bits total): each pass partitions
   * the characters into two buckets based on the current bit and writes
   * them back in bucket order, preserving the order established by earlier
   * (lower) bit passes.
   *
   * @param s The string to sort, modified in place.
   * @return The sorted string.
   *
   * Time complexity: O(7n) = O(n) for n characters (fixed 7-bit ASCII).
   * Space complexity: O(n) auxiliary for the buckets.
   */
  static std::string RadixSort(std::string &s) {
    // Standard ASCII: 0 to 127 base 10 -> 0 to 1111111 base 2.
    for (size_t bit = 0; bit < 7; bit++) {
      std::string zero_bucket;
      std::string one_bucket;
      for (const char &ascii_value : s) {
        // & 1u — mask with ...0001, zeroing everything except the last bit.
        if (((static_cast<unsigned char>(ascii_value) >> bit) & 1u) != 0u) {
          one_bucket.push_back(ascii_value);
        } else {
          zero_bucket.push_back(ascii_value);
        }
      }
      size_t s_idx = 0;
      for (const char &ascii_value : zero_bucket) {
        s[s_idx++] = ascii_value;
      }
      for (const char &ascii_value : one_bucket) {
        s[s_idx++] = ascii_value;
      }
    }
    return s;
  }

  /**
   * Sorts a string's characters in ascending order using bucket sort
   * (counting sort). Works with standard ascii 0 - 127.
   *
   * Counts the occurrences of each of the 128 possible ASCII characters,
   * then rewrites the string by emitting each character the counted number
   * of times in ascending order. Sorting is stable in the sense that equal
   * characters are indistinguishable.
   *
   * @param s The string to sort, modified in place.
   * @return The sorted string.
   *
   * Time complexity: O(n + 128) = O(n) for n characters.
   * Space complexity: O(128) = O(1) auxiliary for the count array.
   */
  static std::string BucketSort(std::string &s) {
    std::vector<size_t> bucket(128, 0);
    for (const char &ascii_value : s) {
      bucket[static_cast<unsigned char>(ascii_value)]++;
    }
    size_t s_idx = 0;
    for (size_t ascii_value = 0; ascii_value < bucket.size(); ascii_value++) {
      while (bucket[ascii_value] > 0) {
        s[s_idx++] = static_cast<char>(ascii_value);
        bucket[ascii_value]--;
      }
    }
    return s;
  }

 private:
  Method sort_method_;
};

}
