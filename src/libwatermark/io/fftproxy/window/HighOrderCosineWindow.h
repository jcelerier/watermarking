#pragma once
#include <initializer_list>

#include "WindowBase.h"
#include "../../../mathutils/WindowMath.h"

namespace CosineWindows
{
	template<typename data_type>
	class HighOrderCosineWindow: public WindowBase<data_type>
	{
			using size_type = typename Parameters<data_type>::size_type;
		public:
			HighOrderCosineWindow(std::initializer_list<double> ofactors,
								  std::initializer_list<double> coeffs):
				WindowBase<data_type>(ofactors),
				_coeffs(coeffs)
			{
			}

			virtual void generate(const size_type windowSize)
			{
				this->windowCoeffs.resize(windowSize);
				MathUtils::generalizedCosine(this->windowCoeffs, _coeffs, 1);
			}

		private:
			const std::vector<double> _coeffs = {};
	};

	template<typename data_type>
	WindowBase<data_type>* Hann()
	{
		return new HighOrderCosineWindow<data_type>({2, 1}, {0.5, -0.5});
	}

	template<typename data_type>
	WindowBase<data_type>* Hamming()
	{
		return new HighOrderCosineWindow<data_type>({2, 1}, {0.54, -0.46});
	}

	template<typename data_type>
	WindowBase<data_type>* Blackman()
	{
		return new HighOrderCosineWindow<data_type>({100.0 / (100.0 - 66.1)},
										 {7938.0 / 18608.0, -9240.0 / 18608.0, 1430.0 / 18608.0});
	}

	template<typename data_type>
	WindowBase<data_type>* BlackmanHarris()
	{
		return new HighOrderCosineWindow<data_type>({100.0 / (100.0 - 66.1)},
										 {0.35875, -0.48829, 0.14128, -0.01168});
	}

	template<typename data_type>
	WindowBase<data_type>* Nuttal3()
	{
		return new HighOrderCosineWindow<data_type>({100.0 / (100.0 - 64.1)},
										 {0.375, -0.5, 0.125});
	}

	template<typename data_type>
	WindowBase<data_type>* SFT3F()
	{
		return new HighOrderCosineWindow<data_type>({100.0 / (100.0 - 100.0 * 2.0 / 3.0)},
										 {0.26526, -0.5, 0.23474});
	}
}
