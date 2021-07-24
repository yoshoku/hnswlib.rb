# Hnswlib

Welcome to your new gem! In this directory, you'll find the files you need to be able to package up your Ruby library into a gem. Put your Ruby code in the file `lib/hnswlib`. To experiment with that code, run `bin/console` for an interactive prompt.

TODO: Delete this and the text above, and describe your gem

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'hnswlib'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install hnswlib

## Usage

```
require 'hnswlib'

f = 40 # length of item vector that will be indexed.
t = Hnswlib::Index.new(n_features: f, max_item: 1000)

1000.times do |i|
  v = Array.new(f) { rand }
  t.add_item(i, v)
end

t.save('test.ann')

u = Hnswlib::Index.new(n_features: f, max_item: 1000)
u.load('test.ann')
p u.get_nns_by_item(0, 100) # will find the 100 nearest neighbors.
```

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/yoshoku/hnswlib.rb.
This project is intended to be a safe, welcoming space for collaboration,
and contributors are expected to adhere to the [code of conduct](https://github.com/yoshoku/hnswlib.rb/blob/main/CODE_OF_CONDUCT.md).
