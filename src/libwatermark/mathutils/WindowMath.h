#pragma once
#include <cmath>
// Maths pour les fonctions de fenêtrage
// Explication du n ou n-1 :
// Page discussion windowing wikipedia et :
// On the use of Windows for Harmonic Analysis with the Discrete Fourier Transform, Proceedings of the IEEE, Vol.66, No.1, January 1978, pp 51-83.
namespace MathUtils
{
	/**
	 * @brief generalizedCosine Generates a window function of the high-order generalized cosine family
	 * @param data Where to put the window coefficients
	 * @param coeffs The a0, a1, etc.. coeffs.
	 * @param parity 0 for dft-even, 1 for dft-odd (MATLAB)
	 */
	inline void generalizedCosine(std::vector<double>& data, const std::vector<double>& coeffs, int parity = 0)
	{
		const double c = 2.0 * M_PI / (data.size() - parity);

		//TODO tester inversion de boucle pour voir plus rapide
		for(auto i = 0U; i < data.size(); i++)
			for(auto k = 0U; k < coeffs.size(); k++)
				data[i] += coeffs[k] * std::cos(c * k * i);
	}
}
