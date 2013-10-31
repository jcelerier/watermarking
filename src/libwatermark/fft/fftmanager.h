#pragma once
#include <complex>
#include <algorithm>
#include <vector>
#include <iostream>

#include "../Parameters.h"

/**
 * @brief Interface to follow for implementation of FFT algorithms.
 */
template <typename data_type>
class FFTManager
{
	public:
		using size_type = typename Parameters<data_type>::size_type;
		using complex_type = typename Parameters<data_type>::complex_type;

	protected:
		std::vector<complex_type> _spectrum = {};
		std::vector<data_type> _in = {};
		std::vector<data_type> _out = {};

		const Parameters<data_type>& conf;


	public:

		FFTManager(const Parameters<data_type> & config):
			conf(config)
		{
			_in.resize(conf.bufferSize);
			_out.resize(conf.bufferSize);
			_spectrum.resize(spectrumSize());
		}

		virtual ~FFTManager() = default;

		/**
		 * @brief input
		 * @return a pointer to the input data.
		 */
		virtual std::vector<data_type>& input() final
		{
			return _in;
		}

		/**
		 * @brief output
		 * @return a pointer to the output data.
		 */
		virtual std::vector<data_type>& output() final
		{
			return _out;
		}

		/**
		 * @brief Spectrum
		 * @return The spectrum
		 */
		virtual std::vector<complex_type>& spectrum() final
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
		virtual size_type spectrumSize() const
		{
			return conf.bufferSize / 2 + 1;
		}

		/**
		 * @brief Normalization factor
		 *
		 * Sometimes, the fft implementation might give an output which needs to be normalized.
		 * @return The factor by which every sample of the ouput must be multiplied.
		 */
		virtual double normalizationFactor() const = 0;
};
