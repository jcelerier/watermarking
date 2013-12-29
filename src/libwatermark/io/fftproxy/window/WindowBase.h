#pragma once
#include <vector>

#include "../../../Parameters.h"

template <typename data_type>
/**
 * @brief The WindowBase class
 *
 * Classe pour les fonctions de fenêtrage.
 * choix des facteurs d'overlap : https://ccrma.stanford.edu/~jos/parshl/Choice_Hop_Size.html
 * fenêtres : wikipedia.
 *
 * Facteur d'overlap :
 *	1: on avance entièrement
 *  2: on avance de 50%
 *  4: on avance de 25%
 *  etc...
 */
class WindowBase
{
	using size_type = typename Parameters<data_type>::size_type;

	public:
		WindowBase(std::initializer_list<double> v):
			factors(v)
		{
		}

		virtual ~WindowBase() = default;

		virtual void apply(std::vector<data_type>& v, const size_type increment) = 0;

		std::vector<unsigned int>& acceptableFactors()
		{
			return factors;
		}

	protected:
		std::vector<double> factors = {};
};

template<typename T>
using Window_p = std::shared_ptr<WindowBase<T>>;
