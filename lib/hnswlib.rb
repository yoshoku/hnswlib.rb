# frozen_string_literal: true

require_relative 'hnswlib/version'
require_relative 'hnswlib/hnswlibext'

module Hnswlib
  # Index is a class that provides functions for k-nearest eighbors search.
  #
  # @example
  #   require 'hnswlib'
  #
  #   index = Hnswlib::Index.new(n_features: 100, max_item: 10000)
  #
  #   5000.times do |item_id|
  #     item_vec = Array.new(100) { rand - 0.5 }
  #     index.add_item(item_id, item_vec)
  #   end
  #
  #   index.get_nns_by_item(0, 100)
  #
  class Index
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
    def initialize(n_features:, max_item:, metric: 'l2', m: 16, ef_construction: 200, random_seed: 100)
      @metric = metric
      space = if @metric == 'dot'
                Hnswlib::InnerProductSpace.new(n_features)
              else
                Hnswlib::L2Space.new(n_features)
              end
      @index = Hnswlib::HierarchicalNSW.new(
        space: space, max_elements: max_item, m: m, ef_construction: ef_construction, random_seed: random_seed
      )
    end

    # Add item to be indexed.
    #
    # @param i [Integer] The ID of item.
    # @param v [Array] The vector of item.
    # @return [Boolean]
    def add_item(i, v)
      @index.add_point(v, i)
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
    # @return [Array]
    def remove_item(i)
      @index.mark_deleted(i)
    end

    # Search the n closest items.
    #
    # @param i [Integer] The ID of query item.
    # @param n [Integer] The number of nearest neighbors.
    # @param include_distances [Boolean] The flag indicating whether to returns all corresponding distances.
    # @return [Array<Integer> or Array<Array<Integer>, Array<Float>>]
    def get_nns_by_item(i, n, include_distances: false)
      v = @index.get_point(i)
      ids, dists = @index.search_knn(v, n)
      include_distances ? [ids, dists] : ids
    end

    # Search the n closest items.
    #
    # @param v [Array] The vector of query item.
    # @param n [Integer] The number of nearest neighbors.
    # @param include_distances [Boolean] The flag indicating whether to returns all corresponding distances.
    # @return [Array<Integer> or Array<Array<Integer>, Array<Float>>]
    def get_nns_by_vector(v, n, include_distances: false)
      ids, dists = @index.search_knn(v, n)
      include_distances ? [ids, dists] : ids
    end

    # Reize the search index.
    #
    # @param new_max_item [Integer] The maximum number of items.
    def resize_index(new_max_item)
      @index.reisze_index(new_max_item)
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
    def load(filename)
      @index.load_index(filename)
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
end
