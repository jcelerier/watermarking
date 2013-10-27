#include <cmath>
#include <algorithm>
#include <functional>
#include "math_util.h"

namespace MathUtil
{
	double CplxToPower(const std::complex<double> val)
	{
		return std::norm(val);
	}

	double CplxToPhase(const std::complex<double> val)
	{
		return std::arg(val);
	}

	double energy(const double * tab, const unsigned int length)
	{
		return mapReduce_n(tab, length, 0.0, [] (double x) { return std::pow(x, 2);}, std::plus<double>());
	}

	double abssum(const double * tab, const unsigned int length)
	{
		return mapReduce_n(tab, length, 0.0, [] (double x) { return std::abs(x); },  std::plus<double>());
	}

	void computePowerAndPhaseSpectrum(const std::complex<double> * const in, double * const powoutput, double * const phaseoutput, const unsigned int size)
	{
		std::transform(in, in + size, powoutput, CplxToPower);
		std::transform(in, in + size, phaseoutput, CplxToPhase);
	}

	void computePowerSpectrum(const std::complex<double> * const in, double * const powoutput, const unsigned int size)
	{
		std::transform(in, in + size, powoutput, CplxToPower);
	}

	double ShortToDouble(const short x)
	{
		const double normalizationFactor = 1.0 / std::pow(2, sizeof(short) * 8 - 1);
		return x * normalizationFactor;
	}

	short DoubleToShort(const double x)
	{
		const unsigned int denormalizationFactor = (int) std::pow(2, sizeof(short) * 8 - 1);
		return (short) (x * denormalizationFactor);
	}
}
