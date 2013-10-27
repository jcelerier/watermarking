#pragma once

#include <memory>

#include "watermark/algorithms.h"
#include "fft/fftmanager.h"

typedef std::shared_ptr<WatermarkBase> Watermark_p;
typedef std::shared_ptr<FFTManager> FFT_p;

/**
 * @brief Main class.
 *
 */
class WatermarkManager
{
	public:
		enum DataSource { File, Buffer };

		/**
		 * @brief Constructor
		 *
		 * @param fft_Size Wanted size of FFT. 256 or 512 are good choices. Must be a power of two.
		 * @param sampling_Rate Sampling rate of the audio.
		 */
		WatermarkManager(const unsigned int fft_Size, const unsigned int sampling_Rate);
		WatermarkManager(const WatermarkManager& sm);
		const WatermarkManager& operator=(const WatermarkManager& sm);
		/**
		 * @brief Destructor.
		 *
		 */
		~WatermarkManager();

		/**
		 * @brief Returns the modified buffer (the subtracted one).
		 *
		 * @return double Pointer to the modified buffer.
		 */
		double *getData() const;

		/**
		 * @brief Generates all the data needed from the given parameters
		 *
		 * For instance, if using EL algorithm, will load loudness countour data from disk.
		 */
		void onDataUpdate();

		/**
		 * @brief Returns the size of the internal buffer / audio file.
		 *
		 * @return unsigned int size.
		 */
		unsigned int getLength() const;

		/**
		 * @brief Returns the original buffer, to perform NRR computation for instance.
		 *
		 * @return double Pointer to the original buffer.
		 */
		double *getNoisyData();

		/**
		 * @brief Reads a file into the internal buffer.
		 *
		 * @param str Path to the file.
		 * @return unsigned int Size of the file.
		 */
		unsigned int readFile(const char * str);

		/**
		 * @brief Reads a buffer into the internal buffer.
		 *
		 * @param buffer Short buffer to read from.
		 * @param length Length of the buffer.
		 * @return unsigned int Length of the buffer (useless?).
		 */
		unsigned int readBuffer(const short * buffer, const unsigned int length);

		/**
		 * @brief Writes into a buffer.
		 *
		 * @param buffer Pointer to the buffer to write to.
		 */
		void writeBuffer(short * const buffer) const;

		/**
		 * @brief Undoes all change on the processed audio data.
		 *
		 * Effectively copies orig_data into data.
		 *
		 */
		void initDataArray();


		/**
		 * @brief Returns the chosen FFT size.
		 *
		 * @return unsigned int FFT size.
		 */
		unsigned int FFTSize() const;

		/**
		 * @brief Sets the FFT size.
		 *
		 * Resets most arrays and data, because their size and meaning changes.
		 * Keeps the original and processed array, however, because they are not dependent on this parameter.
		 *
		 * @param value FFT size.
		 */
		void setFftSize(const unsigned int value);

		/**
		 * @brief Returns the size of the spectrum.
		 *
		 * We use FFTW r2c functions so the size is FFT_SIZE / 2 + 1.
		 *
		 * @return unsigned int Spectrum size.
		 */
		unsigned int spectrumSize() const;

		/**
		 * @brief Returns the sampling rate.
		 *
		 * @return unsigned int Sampling rate.
		 */
		unsigned int getSamplingRate() const;

		/**
		 * @brief Sets the sampling rate.
		 *
		 * Resets most arrays and data, because their size and meaning changes.
		 * Keeps the original and processed array, however, because they are not dependent on this parameter.
		 *
		 * @param value Sampling rate.
		 */
		void setSamplingRate(const unsigned int value);

		/**
		 * @brief Reads parameters from subtraction.conf file.
		 *
		 * Syntax of this file : one parameter per line, on this order :
			alpha
			beta
			alphawt
			betawt
			iterations
			noise algo (std / martin / wavelets)
			algo (std / el / ga)
		 *
		 */
		void readParametersFromFile();


		/**
		 * @brief getSubtractionImplementation
		 * @return Used subtraction algorithm.
		 */
		WatermarkBase* getWatermarkImplementation() const;
		/**
		 * @brief setSubtractionImplementation
		 * @param value Subtraction to use.
		 */
		void setWatermarkImplementation(WatermarkBase* value);

		/**
		 * @brief getFrameIncrement
		 *
		 * If OLA is enabled, it will only return the "effective" sample number, not including the samples to zero.
		 *
		 * @return Number of samples in each frame.
		 */
		unsigned int getFrameIncrement() const;


		/**
		 * @brief bypass
		 * @return true if the processing is bypassed, false if not.
		 */
		bool bypass();


		/**
		 * @brief enableOLA Enables overlap-add.
		 */
		void enableOLA();
		/**
		 * @brief disableOLA Disables overlap-add.
		 */
		void disableOLA();
		/**
		 * @brief setOLA To set overlap-add with a boolean.
		 * @param val True to enable, false to disable.
		 */
		void setOLA(const bool val);
		/**
		 * @brief OLAenabled
		 * @return true if overlap-add is enabled.
		 */
		bool OLAenabled() const;

		/**
		 * @brief execute Runs the algorithm.
		 */
		void execute();

	private:
		DataSource dataSource() const;

		/**
		 * @brief Initializes the needed arrays when a change of FFT size is performed.
		 *
		 */
		void onFFTSizeUpdate();

		//*** Data copying algorithms ***//
		/**
		 * @brief copyInput High level handler for input copying.
		 *
		 * @param pos Sample where the copy must start.
		 */
		void copyInput(const unsigned int pos);

		/**
		 * @brief copyOutput High level handler for output copying.
		 *
		 * @param pos Sample where the copy must start.
		 */
		void copyOutput(const unsigned int pos);

		/**
		 * @brief Copies untouched value into inner fft-sized buffer for transformation.
		 *
		 * @param pos Sample where the copy must start.
		 */
		void copyInputSimple(const unsigned int pos);

		/**
		 * @brief Copies subtracted values into file or large buffer after transformation.
		 *
		 * @param pos Sample where the copy must start.
		 */
		void copyOutputSimple(const unsigned int pos);

		/**
		 * @brief Copies untouched value into inner fft-sized buffer for transformation.
		 *
		 * Uses the overlap-add method (adds a zero-padding).
		 *
		 * @param pos
		 */
		void copyInputOLA(const unsigned int pos);

		/**
		 * @brief Copies subtracted values into file or large buffer after transformation.
		 *
		 * Uses the overlap-add method (+= instead of =).
		 *
		 * NOT SUITED FOR THREADING (yet).
		 *
		 * @param pos
		 */
		void copyOutputOLA(const unsigned int pos);


		//*** Members ***//
		DataSource _dataSource = DataSource::Buffer;

		unsigned int _samplingRate = 0; /**< TODO */
		FFT_p _fft = nullptr;

		// Algorithms
		Watermark_p _watermark = nullptr;

		// Storage
		unsigned int _tabLength = 0; /**< TODO */

		double *_data = nullptr; /**< TODO */
		double *_origData = nullptr; /**< TODO */


		bool _useOLA = false;
		unsigned int _ola_frame_increment = 0; /**< TODO */
		unsigned int _std_frame_increment = 0; /**< TODO */

		unsigned int _iterations = 1; /**< TODO */


		// For measurements
		bool _bypass = false;
};
