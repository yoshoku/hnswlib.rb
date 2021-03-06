## [0.6.2] - 2022-06-25

- Refactor codes and configs with RuboCop and clang-format.
- Change to raise ArgumentError when non-array object is given to distance method.

## [0.6.1] - 2022-04-30

- Change the `search_knn` method of `BruteforceSearch` to output warning message instead of rasing RuntimeError
when the number of search results is less than the requested number of neighbors.
- Fix to raise RuntimeError when failing to open file on the `load_index` method of `BruteforceSearch`.
- Add guard to not free unallocated memory space.

## [0.6.0] - 2022-04-16

**Breaking change:**

- Change the `search_knn` method of `HierarchicalNSW` to output warning message instead of rasing RuntimeError
when the number of search results is less than the requested number of neighbors.

## [0.5.3] - 2022-03-05

- Add error handling for std::runtime_error throwed from hnswlib.
- Add memory release for existing search index when loading index.

## [0.5.2] - 2022-02-19

- Update bundled hnswlib version to 0.6.2.

## [0.5.1] - 2022-02-11

- Update bundled hnswlib version to 0.6.1.
- Update documentations.
- Introduce conventional commits.

## [0.5.0] - 2021-12-12

- Update bundled hnswlib version to 0.6.0.

## [0.4.0] - 2021-09-12

- Add type declaration file.

## [0.3.0] - 2021-08-08

- Rename `Hnswlib::Index` to `Hnswlib::HnswIndex` (for compatibility, `Hnswlib::Index` has been an alis for `Hnswlib::HnswIndex`).
- Update API documentation.

## [0.2.0] - 2021-08-02

- Add binding class for the BruteforceSearch.
- Add type check for arguments of initialize methods of BruteforceSearch and HierarchicalNSW.
- Add dummy constructor call at memory allocation for class to prevent occuring segment fault on GC when initialize method is failed.

## [0.1.1] - 2021-07-25

- Fix to use `rb_obj_is_isntance_of` for klass comparison due to type error when loading search index on irb 1.3.x: [issue #1](https://github.com/yoshoku/hnswlib.rb/issues/1)
- Update API documentation.

## [0.1.0] - 2021-07-24

- Initial release
