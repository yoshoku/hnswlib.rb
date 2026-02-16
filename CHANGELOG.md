## [[0.9.2](https://github.com/yoshoku/hnswlib.rb/compare/v0.9.1...v0.9.2)] - 2026-02-16

- Remove libstdc++ loading check from native extensions build step.

## [[0.9.1](https://github.com/yoshoku/hnswlib.rb/compare/v0.9.0...v0.9.1)] - 2025-11-23

- Fix to use require for compatibility with distributions installing extensions separately.

## [[0.9.0](https://github.com/yoshoku/hnswlib.rb/compare/v0.8.1...v0.9.0)] - 2023-12-16

- Update bundled hnswlib version to 0.8.0.
  - Multi-vector document search and epsilon search, which are added only to the C++ version, are not supported. These features will be supported in future release.

## [0.8.1] - 2023-03-18

- Update the type declarations of HierarchicalNSW and BruteforceSearch along with recent changes.

## [0.8.0] - 2023-03-14

**Breaking change:**

- Change to give a String to the space argument of the `initialize` method
in [HierarchicalNSW](https://yoshoku.github.io/hnswlib.rb/doc/Hnswlib/HierarchicalNSW.html) and [BruteforceSearch](https://yoshoku.github.io/hnswlib.rb/doc/Hnswlib/BruteforceSearch.html).
- Add `init_index` method to HierarchicalNSW and BruteforceSearch.
Along with this, some arguments of `initialize` method moved to `init_index` method.
  ```ruby
  require 'hnswlib'

  n_features = 3
  max_elements = 10

  hnsw = Hnswlib::HierarchicalNSW.new(space: 'l2', dim: n_features)
  hnsw.init_index(max_elements: max_elements, m: 16, ef_construction: 200, random_seed: 42, allow_replace_deleted: false)

  bf = Hnswlib::BruteforceSearch.new(space: 'l2', dim: n_features)
  bf.init_index(max_elements: max_elements)
  ```
- Deprecate [HnswIndex](https://yoshoku.github.io/hnswlib.rb/doc/Hnswlib/HnswIndex.html) has interface similar to Annoy.

## [0.7.0] - 2023-03-04

- Update bundled hnswlib version to 0.7.0.
- Add support for replacing an element marked for deletion with a new element.
- Add support filtering function by label in search_knn method of BruteforeceSearch and HierarchicalNSW.

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
