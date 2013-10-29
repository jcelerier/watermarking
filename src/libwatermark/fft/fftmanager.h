#pragma once
#include <complex>
#include <algorithm>
#include <vector>
#include "../Parameters.h"
#include <iostream>
/**
 * @brief Interface to follow for implementation of FFT algorithms.
 */
class FFTManager
{
	public:
		using size_type = Parameters::size_type;
		using data_type = Parameters::data_type;
		using complex_type = Parameters::complex_type;

	protected:
		std::vector<complex_type> _spectrum = {};
		std::vector<data_type> _in = {};
		std::vector<data_type> _out = {};

		const Parameters& conf;


	public:

		FFTManager(const Parameters & config):
			conf(config)
		{
			_in.resize(conf.bufferSize);
			_out.resize(conf.bufferSize);
			_spectrum.resize(spectrumSize());
		}

		virtual FFTManager* clone() = 0;

		virtual ~FFTManager()
		{

		}

		/**
		 * @brief input
		 * @return a pointer to the input data.
		 */
		std::vector<data_type>& input()
		{
			return _in;
		}

		/**
		 * @brief output
		 * @return a pointer to the output data.
		 */
		std::vector<data_type>& output()
		{
			return _out;
		}

		/**
		 * @brief Spectrum
		 * @return The spectrum
		 */
		std::vector<complex_type>& spectrum()
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
		virtual data_type normalizationFactor() const = 0;
};
