# frozen_string_literal: true

RSpec.describe Hnswlib::HierarchicalNSW do
  let(:dim) { 3 }
  let(:max_elements) { 4 }
  let(:ef_construction) { 100 }
  let(:em) { 10 }
  let(:space) { 'l2' }
  let(:index) do
    described_class.new(space: space, dim: dim, max_elements: max_elements, ef_construction: ef_construction, m: em)
  end

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

  describe '#get_point' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([2, 3, 4], 1)
    end

    it 'returns specified point' do
      expect(index.get_point(0)).to match([1, 2, 3])
    end

    context 'when specifying non-existent label' do
      it 'raises RuntimeError' do
        expect { index.get_point(2) }.to raise_error(RuntimeError, /Label not found/)
      end
    end
  end

  describe '#get_ids' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([2, 3, 4], 2)
    end

    it 'returns label list' do
      expect(index.get_ids).to contain_exactly(0, 2)
    end

    context 'when index is empty' do
      let(:empty_index) { described_class.new(space: space, dim: dim, max_elements: max_elements) }

      it 'returns empty array', :aggregate_failures do
        expect(empty_index.get_ids).to be_a(Array)
        expect(empty_index.get_ids).to be_empty
      end
    end
  end

  describe '#mark_deleted' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([2, 3, 4], 1)
      index.add_point([3, 4, 5], 2)
    end

    it 'marks stored point as deleted', :aggregate_failures do
      index.mark_deleted(0)
      expect(index.search_knn([1, 2, 3], 1).first).to match([1])
    end
  end

  describe '#unmark_deleted' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([2, 3, 4], 1)
      index.add_point([3, 4, 5], 2)
      index.mark_deleted(0)
    end

    it 'unmarks deleted stored point', :aggregate_failures do
      index.unmark_deleted(0)
      expect(index.search_knn([1, 2, 3], 1).first).to match([0])
    end
  end

  describe '#resize_index' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([2, 3, 4], 1)
      index.add_point([3, 4, 5], 2)
      index.add_point([4, 5, 6], 3)
    end

    it 'resizes the maximum number of elements' do
      index.resize_index(max_elements + 1)
      expect(index.max_elements).to eq(max_elements + 1)
    end

    context 'when resizing to a size smaller than the number of elements' do
      it 'resizes the maximum number of elements' do
        expect do
          index.resize_index(max_elements - 1)
        end.to raise_error(ArgumentError, /Cannot resize, max element is less than the current number of elements/)
      end
    end
  end

  describe '#search_knn' do
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([1, 1, 3], 1)
      index.add_point([2, 2, 4], 2)
      index.add_point([2, 2, 1], 3)
    end

    it 'searches nearest neighbors' do
      expect(index.search_knn([1, 2, 2.5], 2)).to match([[0, 1], [0.25, 1.25]])
    end

    context 'when given filter function' do
      let(:filter) do
        proc { |label| label.odd? }
      end

      it 'returns filtered serch results' do
        expect(index.search_knn([1, 2, 3], 4, filter: filter)[0]).to match([1, 3])
      end
    end
  end

  describe '#save_index and #load_index' do
    let(:filename) { File.expand_path("#{__dir__}/bruteforce.ann") }
    let(:loaded_index) { described_class.new(space: space, dim: dim, max_elements: max_elements) }

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
    before do
      index.add_point([1, 2, 3], 0)
      index.add_point([1, 2, 4], 1)
    end

    it 'returns the number of elements currently indexed' do
      expect(index.current_count).to eq(2)
    end
  end

  describe '#ef_construction' do
    it 'returns the ef_construction value' do
      expect(index.ef_construction).to eq(ef_construction)
    end
  end

  describe '#m' do
    it 'returns the M value' do
      expect(index.m).to eq(em)
    end
  end

  describe '#set_ef and #get_ef' do
    before { index.set_ef(50) }

    it 'returns the ef value' do
      expect(index.get_ef).to eq(50)
    end
  end

  describe 'allow_replace_deleted argument' do
    context 'when allow_replace_deleted is false' do
      let(:index) { described_class.new(space: space, dim: dim, max_elements: 4, allow_replace_deleted: false) }

      before do
        index.add_point([1, 0, 2], 0)
        index.add_point([1, 1, 2], 1)
        index.add_point([1, 2, 2], 2)
        index.add_point([1, 3, 2], 3)
        index.mark_deleted(0)
      end

      it 'raises RuntimeError', :aggregate_failures do
        expect do
          index.add_point([1, 4, 2], 4)
        end.to raise_error(RuntimeError, /The number of elements exceeds the specified limit/)
        expect do
          index.add_point([1, 4, 2], 4, replace_deleted: true)
        end.to raise_error(RuntimeError, /Replacement of deleted elements is disabled in constructor/)
      end
    end

    context 'when allow_replace_deleted is true' do
      let(:index) { described_class.new(space: space, dim: dim, max_elements: 4, allow_replace_deleted: true) }

      before do
        index.add_point([1, 0, 2], 0)
        index.add_point([1, 1, 2], 1)
        index.add_point([1, 2, 2], 2)
        index.add_point([1, 3, 2], 3)
        index.mark_deleted(0)
      end

      it 'replaces the marked point as deleted', :aggregate_failures do
        expect { index.add_point([1, 4, 2], 4, replace_deleted: true) }.not_to raise_error
        expect(index.get_point(4)).to match([1, 4, 2])
      end
    end
  end
end
