# Hnswlib.rb

[![Build Status](https://github.com/yoshoku/hnswlib.rb/actions/workflows/build.yml/badge.svg)](https://github.com/yoshoku/hnswlib.rb/actions/workflows/build.yml)
[![Gem Version](https://badge.fury.io/rb/hnswlib.svg)](https://badge.fury.io/rb/hnswlib)
[![License](https://img.shields.io/badge/License-Apache%202.0-yellowgreen.svg)](https://github.com/yoshoku/hnswlib.rb/blob/main/LICENSE.txt)
[![Documentation](https://img.shields.io/badge/api-reference-blue.svg)](https://yoshoku.github.io/hnswlib.rb/doc/)

Hnswlib.rb provides Ruby bindings for the [Hnswlib](https://github.com/nmslib/hnswlib)
that implements approximate nearest-neghbor search based on
hierarchical navigable small world graphs.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'hnswlib'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install hnswlib

Note: hnswlib.rb gives no optimization options when building native extensions.
If necessary, add the optimization options yourself during installation, as follows;

```
$ bundle config --local build.hnswlib "--with-cxxflags=-march=native"
$ bundle install
```

Or:

```
$ gem install hnswlib -- --with-cxxflags=-march=native
```

## Documentation

* [Hnswlib.rb API Documentation](https://yoshoku.github.io/hnswlib.rb/doc/)

## Usage

```ruby
require 'hnswlib'

f = 40 # length of datum point vector that will be indexed.
t = Hnswlib::HierarchicalNSW.new(space: 'l2', dim: f)
t.init_index(max_elements: 1000)

1000.times do |i|
  v = Array.new(f) { rand }
  t.add_point(v, i)
end

t.save_index('test.ann')
```

```ruby
require 'hnswlib'

u = Hnswlib::HierarchicalNSW.new(space: 'l2', dim: f)
u.load_index('test.ann')

q = Array.new(f) { rand }
p u.search_knn(q, 100) # will find the 100 nearest neighbors.
```

## License

The gem is available as open source under the terms of the [Apache-2.0 License](https://www.apache.org/licenses/LICENSE-2.0).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/yoshoku/hnswlib.rb.
This project is intended to be a safe, welcoming space for collaboration,
and contributors are expected to adhere to the [Contributor Covenant](https://contributor-covenant.org) code of conduct.
