#pragma once
#include <vector>

#include "../../Parameters.h"

template <typename data_type>
/**
 * @brief The CopyStyle class
 *
 * Gère la manière dont on copie du gros vecteur
 * qui contient les données vers les petits buffers, et inversement.
 */
class CopyStyle
{
	public:
		using size_type = typename Parameters<data_type>::size_type;

		CopyStyle(const Parameters<data_type>& cfg):
			conf(cfg)
		{
		}

		virtual ~CopyStyle() = default;

		/**
		 * @brief copy Effectue une copie
		 * @param in Itérateur sur buffer d'entrée
		 * @param out Itérateur sur buffer de sortie
		 * @param pos Position dans le buffer d'entrée à partir de laquelle on copie
		 * @param big_vector_length Taille du gros vecteur
		 */
		virtual void copy(typename std::vector<data_type>::const_iterator in,
				  typename std::vector<data_type>::iterator out,
				  size_type pos,
				  size_type big_vector_length) = 0;

		/**
		 * @brief frameIncrement
		 * @return Avancement dans le buffer de base
		 */
		virtual size_type frameIncrement() = 0;

	protected:
		const Parameters<data_type>& conf;
};
