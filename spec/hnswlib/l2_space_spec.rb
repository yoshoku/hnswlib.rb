# frozen_string_literal: true

RSpec.describe Hnswlib::L2Space do
  let(:dim) { 3 }
  let(:space) { described_class.new(dim) }

  describe '#distance' do
    it 'calculates squared Euclidean distance between two arrays', :aggregate_failures do
      expect(space.distance([1, 2, 3], [3, 4, 5])).to be_within(1e-6).of(12)
      expect(space.distance([0.1, 0.2, 0.3], [0.3, 0.4, 0.5])).to be_within(1e-6).of(0.12)
    end

    context 'when given an array with a length different from the number of dimensions', :aggregate_failures do
      it 'raises ArgumentError' do
        expect do
          space.distance([1, 2, 3, 4],
                         [3, 4, 5])
        end.to raise_error(ArgumentError, /Array size does not match to space dimensionality/)
        expect do
          space.distance([1, 2, 3],
                         [3, 4])
        end.to raise_error(ArgumentError, /Array size does not match to space dimensionality/)
      end
    end

    context 'when given a non-array argument', :aggregate_failures do
      it 'raises ArgumentError' do
        expect { space.distance(nil, [3, 4, 5]) }.to raise_error(ArgumentError, /Expect input vector to be Ruby Array/)
        expect { space.distance([1, 2, 3], nil) }.to raise_error(ArgumentError, /Expect input vector to be Ruby Array/)
      end
    end
  end

  describe '#dim' do
    it 'returns the number of dimensions' do
      expect(space.dim).to eq(dim)
    end
  end
end
