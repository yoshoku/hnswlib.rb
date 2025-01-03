# frozen_string_literal: true

RSpec.describe Hnswlib::BruteforceSearch do
  let(:dim) { 3 }
  let(:max_elements) { 4 }
  let(:space) { 'l2' }
  let(:index) { described_class.new(space: space, dim: dim) }

  before { index.init_index(max_elements: max_elements) }

  describe '#add_pint' do
    it 'adds new point' do
      expect(index.add_point([1, 2, 3], 0)).to be(true)
    end

    context 'when given non-array object to 1st argument' do
      it 'raises ArgumentError' do
        expect { index.add_point('[1, 2, 3]', 0) }.to raise_error(ArgumentError, /Expect point vector to be Ruby Array/)
      end
    end

    context 'when given array with mis-matched sizes to 1st argument' do
      it 'raises ArgumentError', :aggregate_failures do
        expect do
          index.add_point([1] * (dim + 1), 0)
        end.to raise_error(ArgumentError, /Array size does not match to index dimensionality/)
        expect do
          index.add_point([1] * (dim - 1), 0)
        end.to raise_error(ArgumentError, /Array size does not match to index dimensionality/)
      end
    end

    context 'when given non-integer object to 2nd argument' do
      it 'raises ArgumentError' do
        expect do
          index.add_point([1] * dim, '0')
        end.to raise_error(ArgumentError, /Expect index to be Ruby Integer/)
      end
    end

    context 'when the index is full' do
      before do
        max_elements.times { |t| index.add_point([1, 2, 3], t) }
      end

      it 'raises RuntimeError' do
        expect do
          index.add_point([1, 2, 3], max_elements)
        end.to raise_error(RuntimeError, /The number of elements exceeds the specified limit/)
      end
    end
  end

  describe '#remove_point' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([2, 3, 4], 1)
    end

    it 'removes stored point', :aggregate_failures do
      expect(index.current_count).to eq(2)
      index.remove_point(0)
      expect(index.current_count).to eq(1)
    end
  end

  describe '#search_knn' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([1, 1, 3], 1)
      index.add_point([2, 2, 3], 2)
      index.add_point([2, 2, 1], 3)
    end

    context "when space is 'l2'" do
      it 'searches nearest neighbors' do
        expect(index.search_knn([1, 2, 2.5], 2)).to match([[0, 1], [0.25, 1.25]])
      end

      context 'when given filter function' do
        let(:filter) do
          proc(&:odd?)
        end

        it 'returns filtered serch results' do
          expect(index.search_knn([1, 2, 3], 4, filter: filter)[0]).to match([1, 3])
        end
      end
    end

    context "when space is 'ip'" do
      let(:space) { 'ip' }
      let(:result) { index.search_knn([1, 2, 2.5], 2) }

      it 'searches nearest neighbors based on 1 subtract inner product', :aggregate_failures do
        expect(result[0]).to match([2, 0])
        expect(result[1]).to match([-12.5, -11.5])
      end
    end

    context "when space is 'cosine'" do
      let(:space) { 'cosine' }
      let(:result) { index.search_knn([1, 2, 2.5], 2) }

      it 'searches nearest neighbors based on cosine distance', :aggregate_failures do
        expect(result[0]).to match([0, 2])
        expect(result[1]).to be_within(1e-6).of([0.00397616, 0.0238129])
      end
    end
  end

  describe '#init_index' do
    before do
      index.add_point([1, 2, 5], 0)
      index.add_point([1, 2, 4], 1)
      index.add_point([1, 2, 3], 2)
      index.init_index(max_elements: 2)
    end

    it 'initializes search index', :aggregate_failures do
      expect(index.max_elements).to eq(2)
      expect(index.current_count).to eq(0)
    end
  end

  describe '#save_index and #load_index' do
    let(:filename) { File.expand_path("#{__dir__}/bruteforce.ann") }
    let(:loaded_index) { described_class.new(space: space, dim: dim) }

    before do
      index.add_point([1, 2, 5], 0)
      index.add_point([1, 2, 4], 1)
      index.add_point([1, 2, 3], 2)
    end

    it 'saves and loads index', :aggregate_failures do
      index.save_index(filename)
      loaded_index.load_index(filename)
      expect(loaded_index.max_elements).to eq(max_elements)
      expect(loaded_index.current_count).to eq(3)
      expect(loaded_index.search_knn([1, 2, 3], 2)).to match([[2, 1], [0.0, 1.0]])
    end
  end

  describe '#max_elements' do
    it 'returns the maximum number of elements' do
      expect(index.max_elements).to eq(max_elements)
    end
  end

  describe '#current_count' do
    before { index.add_point([1, 2, 3], 0) }

    it 'returns the number of elements currently indexed' do
      expect(index.current_count).to eq(1)
    end
  end
end
