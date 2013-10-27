#pragma once

//! Useful evaluation functions, like NRR and SDR.
namespace Eval
{
	/**
	* @brief Computes noise-reduction rate.
	*
	* Cf. paper on EL-SS.
	*
	* @param original Array that holds the noisy signal.
	* @param reduced Array that holds the subtracted signal.
	* @param length Length of the array.
	*
	* @return double NRR.
	*/
	double NRR(const double * original, const double * reduced, const unsigned int length);

	// Computes speech distortion quantity
	/**
	* @brief Computes Speech distortion ratio.
	*
	* Cf. paper on EL-SS.
	*
	* @param original Array that holds the noiseless original signal.
	* @param reduced Array that holds the subtracted signal.
	* @param length Length of the array.
	*
	* @return double SDR.
	*/
	double SDR(const double * original, const double * reduced, const unsigned int length);
}
