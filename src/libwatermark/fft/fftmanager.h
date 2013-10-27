#pragma once
#include <complex>

/**
 * @brief Interface to follow for implementation of FFT algorithms.
 */
class FFTManager
{
	public:
		FFTManager();
		FFTManager(const FFTManager& fm);
		const FFTManager& operator=(const FFTManager& fm);
		virtual FFTManager* clone() = 0;

		virtual ~FFTManager();

		/**
		 * @brief input
		 * @return a pointer to the input data.
		 */
		double* input() const;

		/**
		 * @brief output
		 * @return a pointer to the output data.
		 */
		double* output() const;

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
		std::complex<double>* spectrum() const;

		/**
		 * @brief Spectrum size
		 * @return Returns the size of the spectrum array.
		 */
		virtual unsigned int spectrumSize() const;

		/**
		 * @brief Normalization factor
		 *
		 * Sometimes, the fft implementation might give an output which needs to be normalized.
		 * @return The factor by which every sample of the ouput must be multiplied.
		 */
		virtual double normalizationFactor() const = 0;

		/**
		 * @brief size
		 * @return FFT size.
		 */
		unsigned int size() const;

		virtual void updateSize(const unsigned int);

	protected:
		std::complex<double>* _spectrum = nullptr;
		double *_in = nullptr;
		double *_out = nullptr;
		unsigned int _fftSize = 0;

};
