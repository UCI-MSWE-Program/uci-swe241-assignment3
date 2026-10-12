# Anagram Grouping and Sorting

This task involves grouping a list of strings into anagram buckets by
implementing custom sorting algorithms from scratch without relying on built-in
sorting APIs.

[TOC]

## Task description

An Anagram is a word or phrase formed by rearranging the letters of a different
word or phrase, typically using all the original letters exactly once. For
example, "mango" and "ogman" are anagrams.

Given an array of strings, group the words that are anagrams to each other. You
can return the answer in any order.

## Implementation steps

1.  Implement `List<List<String>> groupAnagram(List<String> strings)`
    This function takes a list of strings and returns a list of lists of
    strings.
2.  Implement `String sortString(String str)`
    Sort all given strings by their characters (ASCII code) to group the
    anagrams. After sorting, strings that converge to the same string are
    grouped together in the same anagram bucket. You must implement the sorting
    classes from scratch and cannot use sorting APIs.
3.  Choose two sorting algorithms
    Select two of the following algorithms to implement and compare their
    performance:
    *   Mergesort
    *   Quicksort
    *   Heapsort
    *   Radix sort
4.  Write sample test cases to validate your implementation.

## Example output

Input:

```json
["bucket", "rat", "mango", "tango", "ogtan", "tar"]
```

Output:
```json
[["bucket"], ["rat", "tar"], ["mango"], ["tango", "ogtan"]]
```