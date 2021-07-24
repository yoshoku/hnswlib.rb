# frozen_string_literal: true

RSpec.describe Hnswlib do
  describe Hnswlib::Index do
    let(:n_features) { 4 }
    let(:metric) { 'l2' }
    let(:n_items) { 5 }
    let(:max_item) { n_items * 2 }
    let(:n_neighbors) { 3 }
    let(:index) { described_class.new(n_features: n_features, max_item: max_item, metric: metric, random_seed: 84) }

    before do
      index.add_item(0, [0, 1, 2, 3])
      index.add_item(1, [1, 1, 2, 3])
      index.add_item(2, [1, 1, 2, 4])
      index.add_item(3, [2, 1, 2, 5])
      index.add_item(4, [2, 1, 2, 6])
    end

    describe '#get_nns_by_item' do
      context 'when include_distances is false' do
        subject { index.get_nns_by_item(0, n_neighbors) }

        it 'returns id list of nearest neighbors' do
          expect(subject).to match([0, 1, 2])
        end
      end

      context 'when include_distances is true' do
        subject { index.get_nns_by_item(0, n_neighbors, include_distances: true) }

        it 'returns id list and distances of nearest neighbors' do
          expect(subject).to match([[0, 1, 2], [0, 1, 2]])
        end
      end
    end

    describe '#get_nns_by_vector' do
      let(:query) { [0, 1, 2, 3] }

      context 'when include_distances is false' do
        subject { index.get_nns_by_vector(query, n_neighbors) }

        it 'returns id list of nearest neighbors' do
          expect(subject).to match([0, 1, 2])
        end
      end

      context 'when include_distances is true' do
        subject { index.get_nns_by_vector(query, n_neighbors, include_distances: true) }

        it 'returns id list and distances of nearest neighbors' do
          expect(subject).to match([[0, 1, 2], [0, 1, 2]])
        end
      end
    end

    describe '#get_item' do
      subject { index.get_item(0) }

      it 'returns the item vector specified by ID' do
        expect(subject).to match([0, 1, 2, 3])
      end
    end

    describe '#n_fetures' do
      subject { index.n_features }

      it 'returns the number of features' do
        expect(subject).to eq(n_features)
      end
    end

    describe '#metric' do
      subject { index.metric }

      it 'returns the distance metric' do
        expect(subject).to eq(metric)
      end
    end

    describe '#n_items' do
      subject { index.n_items }

      it 'returns the number of indexed items' do
        expect(subject).to eq(n_items)
      end
    end

    describe '#max_item' do
      subject { index.max_item }

      it 'returns the maximum number of items' do
        expect(subject).to eq(max_item)
      end
    end

    describe '#save and #load' do
      let(:filename) { File.expand_path("#{__dir__}/test.ann") }

      before { index.save(filename) }

      it 'load the saved index', :aggregate_failures do
        loaded = described_class.new(n_features: n_features, metric: metric, max_item: n_items)
        loaded.load(filename)
        expect(loaded.n_items).to eq(index.n_items)
        expect(loaded.get_item(0)).to match(index.get_item(0))
      end
    end

    describe '#get_distance' do
      subject { index.get_distance(0, 1) }

      before do
        index.add_item(0, [1, 0, 1, 2])
        index.add_item(1, [3, 1, 1, 2])
      end

      context "when metric is 'dot'" do
        let(:metric) { 'dot' }

        it 'returns 1 - dot product between items' do
          # 1 - (1 * 3 + 0 * 1 + 1 * 1 + 2 * 2)
          expect(subject).to be_within(1e-8).of(-7)
        end
      end

      context "when metric is 'l2'" do
        let(:metric) { 'l2' }

        it 'returns squared error between items' do
          # (1 - 3)**2 + (0 - 1)**2 + (1 - 1)**2 + (2 - 2)**2
          expect(subject).to be_within(1e-8).of(5)
        end
      end
    end
  end
end
