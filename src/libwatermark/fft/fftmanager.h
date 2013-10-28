#pragma once
#include <complex>
#include "../Parameters.h"

/**
 * @brief Interface to follow for implementation of FFT algorithms.
 */
class FFTManager
{
	public:
		using size_type = Parameters::size_type;
		using data_type = Parameters::data_type;

		FFTManager(const Parameters &);
		FFTManager(const FFTManager& fm);
		const FFTManager& operator=(const FFTManager& fm);
		virtual FFTManager* clone() = 0;

		virtual ~FFTManager();

		/**
		 * @brief input
		 * @return a pointer to the input data.
		 */
		data_type* input() const;

		/**
		 * @brief output
		 * @return a pointer to the output data.
		 */
		data_type* output() const;

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

		/**
		 * @brief Spectrum
		 * @return The spectrum
		 */
		std::complex<data_type>* spectrum() const;

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



	protected:
		std::complex<data_type>* _spectrum = nullptr;
		data_type *_in = nullptr;
		data_type *_out = nullptr;

		const Parameters& conf;

};
