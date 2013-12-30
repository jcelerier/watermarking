#pragma once
#include <vector>
#include <algorithm>

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
 *
 * A faire :
 * - Créer l'objet Window
 * - Créer l'objet InputOLA
 * - Setter le facteur d'overlap de OLA par rapport à la Window
 * - Setter la taille de la Window par rapport au zero-padding ?
 * - Préalculer les coefficients en fonction de la taille des données qui seront copiées
 * - Multiplier l'input par ces coefficients
 */
class WindowBase
{
	using size_type = typename Parameters<data_type>::size_type;

	public:
		WindowBase(std::initializer_list<unsigned int> v):
			factors(v)
		{
		}

		virtual ~WindowBase() = default;
		virtual void generate(const size_type windowSize) = 0;

		virtual void apply(std::vector<data_type>& v)
		{
			for(auto i = 0U; i < windowCoeffs.size(); ++i)
			{
				v[i] *= windowCoeffs[i];
			}
		}

		std::vector<unsigned int>& acceptableFactors()
		{
			return factors;
		}

	protected:
		std::vector<unsigned int> factors = {};
		std::vector<double> windowCoeffs = {};
};

template<typename T>
using Window_p = std::shared_ptr<WindowBase<T>>;
