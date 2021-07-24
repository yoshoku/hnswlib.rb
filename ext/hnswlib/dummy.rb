# frozen_string_literal: true

# The following code is dummy code for document generation.

module Hnswlib
  # L2Space is a class that calculates squared Euclidean distance for search index.
  # This class is used internally.
  class L2Space
    # Create a new L2Space.
    #
    # @param dim [Integer] The number of dimensions (features).
    def initialize(dim)
      @dim = dim
    end

    # Calculate the squared Euclidean distance between items:
    # d = sum((Ai - Bi)^2)
    #
    # @param arr_a [Array<Float>] The vector of item A.
    # @param arr_b [Array<Float>] The vector of item B.
    # @return [Float]
    def distance(arr_a, arr_b)
      a.zip(b).sum { |v| (v[0] - v[1])**2 }
    end
  end

  # InnerProductSpace is a class that calculates dot product for search index.
  # This class is used internally.
  class InnerProductSpace
    # Create a new InnerProductSpace.
    #
    # @param dim [Integer] The number of dimensions (features).
    def initialize(dim)
      @dim = dim
    end

    # Calculate the dot product between items:
    # d = 1.0 - sum(Ai * Bi)
    #
    # @param arr_a [Array<Float>] The vector of item A.
    # @param arr_b [Array<Float>] The vector of item B.
    # @return [Float]
    def distance(arr_a, arr_b)
      1.0 - a.zip(b).sum { |v| v[0] * v[1] }
    end
  end

  # HierarchicalNSW is a class that provides functions for approximate k-NN search.
  # This class is used internally.
  class HierarchicalNSW
    # Returns the metric space of search index.
    # @return [L2Space | InnerProductSpace]
    attr_reader :space

    # Create a new HierarchicalNSW.
    #
    # @param space [L2Space | InnerProductSpace] The metric space of search index.
    # @param max_elements [Integer] The maximum number of items.
    # @param m [Integer] The maximum number of outgoing connections in the graph
    # @param ef_construction [Integer] The size of the dynamic list for the nearest neighbors.
    # @param random_seed [Integer] The seed value using to initialize the random generator.
    def initialize(space:, max_elements:, m: 16, ef_construction: 200, random_seed: 100)
      @space = space
    end

    # Add item to be indexed.
    #
    # @param arr [Array] The vector of item.
    # @param idx [Integer] The ID of item.
    # @return [Boolean]
    def add_point(arr, idx); end

    # Search the k closest items.
    #
    # @param arr [Array] The vector of query item.
    # @param k [Integer] The number of nearest neighbors.
    # @return [Array<Array<Integer>, Array<Float>>]
    def search_knn(arr, k); end

    # Save the search index to disk.
    #
    # @param filename [String] The filename of search index.
    def save_index(filename); end

    # Load a search index from disk.
    #
    # @param filename [String] The filename of search index.
    def load_index(filename); end

    # Return the item vector.
    #
    # @param idx [Integer] The ID of item.
    # @return [Array]
    def get_point(idx); end

    # Return the indices of stored items.
    #
    # @return [Array]
    def get_ids; end

    # Mark the item as deleted.
    #
    # @param idx [Integer] The ID of item.
    def mark_deleted(idx); end

    # Reize the search index.
    #
    # @param new_max_item [Integer] The maximum number of items.
    def resize_index(new_max_item); end

    # Set the size of the dynamic list for the nearest neighbors.
    #
    # @param new_ef [Integer] The size of the dynamic list.
    def set_ef(new_ef); end

    # Return the maximum number of items.
    #
    # @return [Integer]
    def max_elements; end

    # Return the number of items in the search index.
    #
    # @return [Integer]
    def current_count; end
  end
end
