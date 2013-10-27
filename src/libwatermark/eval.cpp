#include <cmath>
#include <functional>

#include "mathutils/math_util.h"

namespace Eval
{
	double NRR(const double *original, const double *reduced, const unsigned int length)
	{
		return 10.0 * std::log10(MathUtil::energy(original, length) / MathUtil::energy(reduced, length));
	}

	double SDR(const double * original, const double * reduced, const unsigned int length)
	{
		double gamma = MathUtil::abssum(original, length) / MathUtil::abssum(reduced, length);

		double res = MathUtil::mapReduce2_n(original, reduced, length, 0.0,
											[&] (const double x, const double y) { return std::pow(x - gamma * y, 2);},
											std::plus<double>());

		return 10.0 * std::log10(MathUtil::energy(original, length) / res);
	}
}
