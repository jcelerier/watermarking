#pragma once
#include <memory>

#include "copystyle/CopyStyle.h"
#include "../Parameters.h"
#include "../Data.h"
#include "IOInterface.h"

template <typename data_type>
class FFTInputProxy;
template <typename data_type>
class FFTOutputProxy;

/**
 * @brief The IOManagerBase class
 *
 * Classe de base pour la gestion des entrées sorties.
 * Concept : séparation en buffers. Entrée et sortie marchent main dans la main
 * et peuvent se plugger les unes aux autres, ou appliquer des transformations.
 *
 * Réalise un découpage buffer par buffer des données dans vec.
 */
template <typename data_type>
class IOManagerBase : public IOInterface
{
		using size_type = typename Parameters<data_type>::size_type;
		std::vector<std::vector<data_type>> _baseData = {};
		size_type _pos = 0;

	protected:
		Parameters<data_type>& conf;

	public:
		friend class FFTInputProxy<data_type>;
		friend class FFTOutputProxy<data_type>;


		IOManagerBase(Parameters<data_type>& cfg):
			conf(cfg)
		{
		}

		virtual ~IOManagerBase() = default;

		/**
		 * @brief v Accès au vecteur sous-jacent
		 * @return  Vecteur sous-j	xznr
		 */
        virtual std::vector<std::vector<data_type> >& v()
		{
			return _baseData;
		}

		virtual std::vector<data_type>& v(size_type i)
		{
			return _baseData.at(i);
		}


		/**
		 * @brief channels
		 * @return Nombre de canaux
		 */
		virtual size_type channels() override
		{
			return v().size();
		}

		/**
		 * @brief frames
		 * @return Nombre de samples
		 */
		virtual size_type frames() override
		{
			return v()[0].size();
		}

		/**
		 * @brief pos
		 * @return Position
		 */
		size_type& pos()
		{
			return _pos;
		}
};
