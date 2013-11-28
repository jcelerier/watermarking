#pragma once
#include <vector>
#include <complex>
#include <algorithm>
#include <numeric>
namespace MCLT
{
	std::complex<double> complexExp(double M, double r)
	{
		static double cst = - 2 * M_PI;
		return std::polar(1.0, cst * r / M);
	}


	std::vector<std::complex<double> > forward(std::vector<std::complex<double>> in)
	{
		auto L = in.size() - 1 * 2;
		auto M = L / 2;

		// normalized FFT of input
		std::vector<std::complex<double>> U(in.size());
		std::transform(in.begin(), in.end(), U.begin(), [M] (std::complex<double> x)
		{
			static const double cst = sqrt( 1.0 / ( 2.0 * (double)M ) );
			return cst * x;
		});

		// compute modulation function
		std::vector<int> k(M+1);
		// Ce truc est constant
		std::iota(k.begin(), k.end(), 0);

		std::vector<std::complex<double> > c(M);
		std::transform(k.begin(), k.end(), c.begin(), [&] (int i)
		{
			return complexExp(8, 2 * i + 1) * complexExp(4 * (double)M, i);
		});
		// Fin partie constante

		// modulate U into V
		std::vector<std::complex<double>> V(in.size());
		std::transform(c.begin(), c.end(), U.begin(), V.begin(), std::multiplies<std::complex<double>>());

		// compute MCLT coefficients
		std::vector<std::complex<double>> X(in.size());
		std::transform(V.begin(), V.end(), V.begin() + 1, X.begin(), [] (std::complex<double> a, std::complex<double> b)
		{
			std::complex<double> j;
			j.real(0);
			j.imag(1);
			return a * j + b;
		});

		return X;
	}

	void backward()
	{

	}

}
