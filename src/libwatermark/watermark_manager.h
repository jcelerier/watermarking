#pragma once

#include <memory>

#include "watermark/algorithms.h"
#include "fft/fftmanager.h"
#include "io/InputManagerBase.h"
#include "io/OutputManagerBase.h"

typedef std::shared_ptr<WatermarkBase> Watermark_p;
typedef std::shared_ptr<OutputManagerBase> Output_p;
typedef std::shared_ptr<InputManagerBase> Input_p;
typedef std::shared_ptr<FFTManager> FFT_p;

/**
 * @brief Main class.
 *
 */
class WatermarkManager
{
	public:
		WatermarkManager(const Parameters& parameters);
		WatermarkManager(const WatermarkManager& sm);
		const WatermarkManager& operator=(const WatermarkManager& sm);
		~WatermarkManager();

		/**
		 * @brief To call when the data changes
		 *
		 */
		void onDataUpdate();

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
		 * @brief execute Runs the algorithm.
		 */
		void execute();

	private:
		Input_p _input = nullptr;
		Output_p _output = nullptr;

		Watermark_p _watermark = nullptr;

		// Ajouter data : bits à encoder
};
