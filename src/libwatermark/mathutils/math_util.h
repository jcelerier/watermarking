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
	//TODO std::bind ?
	double CplxToPower(const std::complex<double> val)
	{
		return std::norm(val);
	}


	/**
	 * @brief Converts a complex into a double corresponding to its phase.
	 * @param val Complex value
	 * @return arc tan(y, x)
	 */
	double CplxToPhase(const std::complex<double> val)
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
	void computePowerAndPhaseSpectrum(const std::complex<double> * const in, double * const powoutput, double * const phaseoutput, const unsigned int size);


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
	void computePowerSpectrum(const std::complex<double> * const in, double * const powoutput, const unsigned int size);

	/**
	 * @brief energy Returns the average energy for a full spectrum
	 *
	 * @param tab Array of values
	 * @param length Length of tab
	 * @return Energy
	 */
	double energy(const double *tab, const unsigned int length);

	/**
	 * @brief abssum Returns the sum of the absolute values in an array
	 *
	 * @param tab An array
	 * @param length Length of tab
	 * @return Sum of the absolute values
	 */
	double abssum(const double *tab, const unsigned int length);

	/**
	 * @brief Puts a signed 16bit integer (red book) between the -1 / 1 range in double.
	 *
	 * @param x Integer to convert.
	 * @return double Corresponding floating point value.
	 */
	double ShortToDouble(const short x);
	/**
	 * @brief Puts a double between -1 and 1 into a 16 bit signed integer (red book).
	 *
	 * @param x Double to convert.
	 * @return short Corresponding short value.
	 */
	short DoubleToShort(const double x);

}
