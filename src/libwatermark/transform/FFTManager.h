#pragma once
#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>

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
		std::vector<std::vector<complex_type>> _spectrum = {};
		std::vector<std::vector<data_type>> _in = {};
		std::vector<std::vector<data_type>> _out = {};

		const Parameters<data_type>& conf;

	public:
		FFTManager(const Parameters<data_type> & config):
			conf(config)
		{
		}

		FFTManager(unsigned int channels, const Parameters<data_type> & config):
			conf(config)
		{
			setChannels(channels);
		}

		virtual ~FFTManager() = default;

		/**
		 * @brief setChannels Set number of channels.
		 * @param n number of channels.
		 */
		virtual void setChannels(unsigned int n)
		{
			_in.resize(n);
			_out.resize(n);
			_spectrum.resize(n);

			for(auto i = 0U; i < n; ++i)
			{
				_in[i].resize(conf.bufferSize);
				_out[i].resize(conf.bufferSize);
				_spectrum[i].resize(spectrumSize());
			}
		}

		/**
		 * @brief input
		 * @return a pointer to the input data.
		 */
		virtual std::vector<std::vector<data_type> >& input() final
		{
			return _in;
		}

		/**
		* @brief output
		* @return a pointer to the output data.
		*/
		virtual std::vector<std::vector<data_type> >& output() final
		{
			return _out;
		}

		/**
		 * @brief Spectrum
		 * @return The spectrum
		 */
		virtual std::vector<std::vector<complex_type> >&& moveSpectrum() final
		{
			return std::move(_spectrum);
		}

		virtual std::vector<std::vector<complex_type> >& spectrum() final
		{
			return _spectrum;
		}

		/**
		 * @brief Spectrum
		 * @return The spectrum
		 */
		virtual void setSpectrum(std::vector<std::vector<complex_type>>& v) final
		{
			_spectrum = v;
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

template <typename T>
using  FFT_p = std::shared_ptr<FFTManager<T>>;
