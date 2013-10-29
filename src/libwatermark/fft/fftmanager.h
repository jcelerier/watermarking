#pragma once
#include <complex>
#include <vector>
#include "../Parameters.h"

/**
 * @brief Interface to follow for implementation of FFT algorithms.
 */
class FFTManager
{
	public:
		using size_type = Parameters::size_type;
		using data_type = Parameters::data_type;

	protected:
		std::vector<std::complex<data_type>> _spectrum = {};
		std::vector<data_type> _in = {};
		std::vector<data_type> _out = {};

		const Parameters& conf;


	public:

		FFTManager(const Parameters &);

		virtual FFTManager* clone() = 0;

		virtual ~FFTManager();

		/**
		 * @brief input
		 * @return a pointer to the input data.
		 */
		auto input() -> decltype(std::ref(_in))
		{
			return _in;
		}

		/**
		 * @brief output
		 * @return a pointer to the output data.
		 */
		auto output() -> decltype(std::ref(_out))
		{
			return _out;
		}

		/**
		 * @brief Spectrum
		 * @return The spectrum
		 */
		auto spectrum() -> decltype(std::ref(_spectrum))
		{
			return _spectrum;
		}

		/**
		 * @brief Forward FFT.
		 *
		 * From time domain to spectral domain.
		 */
		virtual void forward() const = 0;

		/**
		 * @brief Backward FFT
		 *
		 * From spectral domain to time domain.
		 */
		virtual void backward() const = 0;

		virtual void updateSize() = 0;

		/**
		 * @brief Spectrum size
		 * @return Returns the size of the spectrum array.
		 */
		virtual size_type spectrumSize() const;

		/**
		 * @brief Normalization factor
		 *
		 * Sometimes, the fft implementation might give an output which needs to be normalized.
		 * @return The factor by which every sample of the ouput must be multiplied.
		 */
		virtual data_type normalizationFactor() const = 0;




};
