#pragma once
#include <complex>
#include <functional>
#include <iterator>
#include <type_traits>

//! Mathematic utilities.
namespace MathUtil
{
	/**
	 * @brief Converts a complex into a double corresponding to its power.
	 * @param val Complex value
	 * @return x^2 + y^2
	 */
	template <typename data_type>
	inline data_type CplxToPower(const std::complex<data_type> val)
	{
		return std::norm(val);
	}


	/**
	 * @brief Converts a complex into a double corresponding to its phase.
	 * @param val Complex value
	 * @return arc tan(y, x)
	 */
	template <typename data_type>
	inline data_type CplxToPhase(const std::complex<data_type> val)
	{
		return std::arg(val);
	}


	/**
	 * @brief Computes power and phase array from the magnitude spectrum.
	 *
	 * @param in Input spectrum.
	 * @param powoutput Power output.
	 * @param phaseoutput Phase output.
	 * @param size Size of array.
	 */
	template <typename data_type>
	void computePowerAndPhaseSpectrum(const std::complex<data_type> * const in, data_type * const powoutput, data_type * const phaseoutput, const unsigned int size)
	{
		std::transform(in, in + size, powoutput, CplxToPower<data_type>);
		std::transform(in, in + size, phaseoutput, CplxToPhase<data_type>);
	}


	/**
	 * @brief Implementation of a fast map-reduce operation.
	 *
	 * This tries to follow C++ STL guidelines : takes an iterator, etc...
	 * @param in Input iterator
	 * @param size Number of elements to compute
	 * @param baseval Base value, like for std::accumulate
	 * @param map Mapping function. Must take an element of type *InputIterator, and return an element of type T.
	 * @param reduce Reduction function. Must take two elements of type T and return an element of type T. std::plus is a good candidate most of the time.
	 */
	template <typename T, typename InputIterator, typename MapFunction, typename ReductionFunction>
	T mapReduce_n(const InputIterator in,
				  const unsigned int size,
				  const T baseval,
				  const MapFunction map,
				  const ReductionFunction reduce)
	{
		T val = baseval;

#pragma omp parallel
		{
			T map_val = baseval;

#pragma omp for nowait
			for (auto i = 0U; i < size; ++i)
			{
				map_val = reduce(map_val, map(*(in + i)));
			}

#pragma omp critical
			val = reduce(val, map_val);
		}

		return val;
	}

	/**
	 * @brief Implementation of a fast map-reduce operation.
	 *
	 * This version is to be used when there is two arrays to map-reduce into one value.
	 *
	 * This tries to follow C++ STL guidelines : takes an iterator, etc...
	 * @param in Input iterator
	 * @param in2 Second input iterator
	 * @param size Number of elements to compute
	 * @param baseval Base value, like for std::accumulate
	 * @param map Mapping function. Must take two elements of type *InputIterator, and return an element of type T.
	 * @param reduce Reduction function. Must take two elements of type T and return an element of type T. std::plus is a good candidate most of the time.
	 */
	template <class T, class InputIterator, class InputIterator2,  class MapFunction, class ReductionFunction>
	T mapReduce2_n(const InputIterator in, const InputIterator2 in2, const unsigned int size, const T baseval, const MapFunction map, const ReductionFunction reduce)
	{
		T val = baseval;

#pragma omp parallel
		{
			T map_val = baseval;

#pragma omp for nowait
			for (auto i = 0U; i < size; ++i)
			{
				map_val = reduce(map_val, map(*(in + i), *(in2 + i)));
			}

#pragma omp critical
			val = reduce(val, map_val);
		}

		return val;
	}

	/**
	 * @brief Computes power array from the magnitude spectrum.
	 *
	 * @param in Input spectrum.
	 * @param powoutput Power output.
	 * @param size Size of array.
	 */
	template <typename data_type>
	void computePowerSpectrum(const std::complex<data_type> * const in, data_type * const powoutput, const unsigned int size)
	{
		std::transform(in, in + size, powoutput, CplxToPower<data_type>);
	}

	/**
	 * @brief energy Returns the average energy for a full spectrum
	 *
	 * @param tab Array of values
	 * @param length Length of tab
	 * @return Energy
	 */
	template <typename data_type>
	data_type energy(const data_type *tab, const unsigned int length)
	{
		return mapReduce_n(tab, length, 0.0, [] (data_type x) { return std::pow(x, 2);}, std::plus<data_type>());
	}

	/**
	 * @brief abssum Returns the sum of the absolute values in an array
	 *
	 * @param tab An array
	 * @param length Length of tab
	 * @return Sum of the absolute values
	 */
	template <typename data_type>
	data_type abssum(const data_type *tab, const unsigned int length)
	{
		return mapReduce_n(tab, length, 0.0, [] (data_type x) { return std::abs(x); },  std::plus<data_type>());
	}

	/**
	 * @brief Puts a signed 16bit integer (red book) between the -1 / 1 range in double.
	 *
	 * @param x Integer to convert.
	 * @return double Corresponding floating point value.
	 */
	template<typename T>
	double ToDouble(const T x)
	{
		const double normalizationFactor = std::pow(2.0, -(sizeof(T) * 8.0 - 1.0));
		return x * normalizationFactor;
	}
	/**
	 * @brief Puts a double between -1 and 1 into a 16 bit signed integer (red book).
	 *
	 * @param x Double to convert.
	 * @return short Corresponding short value.
	 */
	template<typename T>
	T FromDouble(const double x)
	{
		const unsigned long denormalizationFactor = (unsigned long) std::pow(2, sizeof(T) * 8 - 1);
		return x * denormalizationFactor;
	}

	// Permet de passer d'un format entrelacé (alternance sample gauche et droit par exemple)
	// à un format désentrelacé (un tableau par canal)
	template<typename T>
	std::vector<std::vector<T> > deinterleave(std::vector<T>& in, unsigned int channels, unsigned int frames)
	{
		std::vector<std::vector<T>> out;
		out.resize(channels);

		for(auto channel = 0U; channel < channels; ++channel)
		{
			out[channel].resize(frames);
			for(auto frame = 0U; frame < frames; ++frame)
				out[channel][frame] = in[frame * channels + channel];
		}

		return out;
	}

	template<typename T>
	std::vector<T> interleave(std::vector<std::vector<T>>& in)
	{
		std::vector<T> out;
		auto channels = in.size();
		auto frames = in[0].size();
		out.resize(channels * frames);

		for(auto channel = 0U; channel < channels; ++channel)
			for(auto frame = 0U; frame < frames; ++frame)
				out[frame * channels + channel] = in[channel][frame];

		return out;
	}

	// Retourne sqrt(somme des carrés / n)
	template <typename data_type>
	data_type RMS(const data_type * tab, const unsigned int length)
	{
		return sqrt(energy<data_type>(tab, length) / length);
	}

}
