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
