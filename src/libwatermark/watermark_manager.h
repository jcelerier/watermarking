#pragma once

#include <memory>

#include "Parameters.h"
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
		 * @brief execute Runs the algorithm.
		 */
		void execute();


		Input_p _input = nullptr;
		Output_p _output = nullptr;
		Watermark_p _watermark = nullptr;

	public:
		// Ajouter data : bits à encoder
		const Parameters& conf;
};
