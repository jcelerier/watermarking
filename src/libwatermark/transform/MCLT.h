#pragma once
#include <vector>
#include <complex>
#include <algorithm>
#include <numeric>

#include "../Parameters.h"

template<typename data_type>
class MCLTManager
{
	public:
		using size_type = typename Parameters<data_type>::size_type;
		using complex_type = typename Parameters<data_type>::complex_type;

		void forward(std::vector<complex_type>& in)
		{
			initDataFw(in.size());

			// modulate U into V
			std::transform(c.begin(), c.end(), in.begin(), V.begin(), std::multiplies<complex_type>());
			// compute MCLT coefficients
			std::fill(in.begin(), in.end(), complex_type(0, 0));
			std::transform(V.begin(), V.end() - 1, V.begin() + 1, in.begin(), [this] (complex_type a, complex_type b)
			{
				return a * j + b;
			});
		}

		void backward(std::vector<complex_type>& in)
		{
			//	determine # of subbands, M
			auto M = in.size() - 1;
			initDataBw(in.size());

			//	allocate vector Y
			std::vector<complex_type> y(2*M);

			//	map X into Y
			for(auto i = 0U; i < M; ++i)
			{
				y[i+1] = 0.25 * conj(c[i]) * (in[i] - j * in[i + 1]);
			}
			y[0] =   std::sqrt(0.125) * (in[0].real() + in[0].imag());
			y[M] = - std::sqrt(0.125) * (in[M].real() + in[M].imag());

			std::copy_n(y.begin(), in.size(), in.begin());
		}

	private:
		complex_type complexExp(const double M, const double r)
		{
			static double cst = - 2 * M_PI;
			return std::polar(1.0, cst * r / M);
		}

		void initDataFw(const size_type spectrum_size)
		{
			if(k.size() != spectrum_size)
			{
				k.resize(spectrum_size);
				c.resize(spectrum_size);
				U.resize(spectrum_size);
				V.resize(spectrum_size);

				std::iota(k.begin(), k.end(), 0);
				std::transform(k.begin(), k.end(), c.begin(), [&] (int i)
				{
					return complexExp(8, 2 * i + 1) * complexExp(4 * ((double)spectrum_size - 1), i);
				});
			}
		}


		void initDataBw(const size_type spectrum_size)
		{
			auto size = spectrum_size - 2;
			if(k.size() != size)
			{
				k.resize(size);
				c.resize(size);

				std::iota(k.begin(), k.end(), 1);
				std::transform(k.begin(), k.end(), c.begin(), [&] (int i)
				{
					return complexExp(8, 2 * i + 1) * complexExp(4 * ((double)spectrum_size - 1), i);
				});
			}
		}

		std::vector<int> k = {};
		std::vector<complex_type> c = {};
		std::vector<complex_type> U = {};
		std::vector<complex_type> V = {};

		const complex_type j = std::complex<double>(0.0, 1.0);

};
