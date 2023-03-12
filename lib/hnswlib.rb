# frozen_string_literal: true

require_relative 'hnswlib/version'
require_relative 'hnswlib/hnswlibext'

module Hnswlib
  # HnswIndex is a class that provides functions for k-nearest eighbors search.
  #
  # @example
  #   require 'hnswlib'
  #
  #   index = Hnswlib::HnswIndex.new(n_features: 100, max_item: 10000)
  #
  #   5000.times do |item_id|
  #     item_vec = Array.new(100) { rand - 0.5 }
  #     index.add_item(item_id, item_vec)
  #   end
  #
  #   index.get_nns_by_item(0, 100)
  #
  class HnswIndex
    # Returns the metric of index.
    # @return [String]
    attr_reader :metric

    # Create a new search index.
    #
    # @param n_features [Integer] The number of features (dimensions) of stored vector.
    # @param max_item [Integer] The maximum number of items.
    # @param metric [String] The distance metric between vectors ('l2' or 'dot').
    # @param m [Integer] The maximum number of outgoing connections in the graph
    # @param ef_construction [Integer] The size of the dynamic list for the nearest neighbors. It controls the index time/accuracy trade-off.
    # @param random_seed [Integer] The seed value using to initialize the random generator.
    # @param allow_replace_removed [Boolean] The flag to replace removed element when adding new element.
    def initialize(n_features:, max_item:, metric: 'l2', m: 16, ef_construction: 200,
                   random_seed: 100, allow_replace_removed: false)
      @metric = metric
      space = @metric == 'dot' ? 'ip' : 'l2'
      @index = Hnswlib::HierarchicalNSW.new(space: space, dim: n_features)
      @index.init_index(max_elements: max_item, m: m, ef_construction: ef_construction,
                        random_seed: random_seed, allow_replace_deleted: allow_replace_removed)
    end

    # Add item to be indexed.
    #
    # @param i [Integer] The ID of item.
    # @param v [Array] The vector of item.
    # @param replace_removed [Boolean] The flag to replace a removed element.
    # @return [Boolean]
    def add_item(i, v, replace_removed: false)
      @index.add_point(v, i, replace_deleted: replace_removed)
    end

    # Return the item vector.
    #
    # @param i [Integer] The ID of item.
    # @return [Array]
    def get_item(i)
      @index.get_point(i)
    end

    # Remove the item vector.
    #
    # @param i [Integer] The ID of item.
    def remove_item(i)
      @index.mark_deleted(i)
    end

    # Search the n closest items.
    #
    # @param i [Integer] The ID of query item.
    # @param n [Integer] The number of nearest neighbors.
    # @param include_distances [Boolean] The flag indicating whether to returns all corresponding distances.
    # @param filter [Proc] The function that filters elements by its labels.
    # @return [Array<Integer> or Array<Array<Integer>, Array<Float>>]
    def get_nns_by_item(i, n, include_distances: false, filter: nil)
      v = @index.get_point(i)
      ids, dists = @index.search_knn(v, n, filter: filter)
      include_distances ? [ids, dists] : ids
    end

    # Search the n closest items.
    #
    # @param v [Array] The vector of query item.
    # @param n [Integer] The number of nearest neighbors.
    # @param include_distances [Boolean] The flag indicating whether to returns all corresponding distances.
    # @param filter [Proc] The function that filters elements by its labels.
    # @return [Array<Integer> or Array<Array<Integer>, Array<Float>>]
    def get_nns_by_vector(v, n, include_distances: false, filter: nil)
      ids, dists = @index.search_knn(v, n, filter: filter)
      include_distances ? [ids, dists] : ids
    end

    # Reize the search index.
    #
    # @param new_max_item [Integer] The maximum number of items.
    def resize_index(new_max_item)
      @index.reisze_index(new_max_item)
    end

    # Set the size of the dynamic list for the nearest neighbors.
    #
    # @param ef [Integer] The size of the dynamic list.
    def set_ef(ef)
      @index.set_ef(ef)
    end

    # Save the search index to disk.
    #
    # @param filename [String] The filename of search index.
    def save(filename)
      @index.save_index(filename)
    end

    # Load a search index from disk.
    #
    # @param filename [String] The filename of search index.
    # @param allow_replace_removed [Boolean] The flag to replace removed element when adding new element.
    def load(filename, allow_replace_removed: false)
      @index.load_index(filename, allow_replace_deleted: allow_replace_removed)
    end

    # Calculate the distances between items.
    #
    # @param i [Integer] The ID of item.
    # @param j [Integer] The ID of item.
    # @return [Float or Integer]
    def get_distance(i, j)
      vi = @index.get_point(i)
      vj = @index.get_point(j)
      @index.space.distance(vi, vj)
    end

    # Return the number of items in the search index.
    #
    # @return [Integer]
    def n_items
      @index.current_count
    end

    # Returns the number of features of indexed item.
    #
    # @return [Integer]
    def n_features
      @index.space.dim
    end

    # Return the maximum number of items.
    #
    # @return [Integer]
    def max_item
      @index.max_elements
    end
  end

  # Index is alias of HnswIndex
  Index = ::Hnswlib::HnswIndex
end
