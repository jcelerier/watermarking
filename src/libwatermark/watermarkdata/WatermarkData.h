#pragma once
#include <vector>
#include <stdint.h>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <memory>
/**
 * @brief The WatermarkData class
 *
 * Cette classe contient les données que l'on veut watermarker.
 * Elle est abstraite et ne peut être instanciée, en effet on peut vouloir différents modes de fonctionnement.
 *
 * Dans cette classe la convention est la suivante : size_bits représente toujours le nombre
 * de bits dans lesquels on lit la taille (en pratique sizeof(_size)).
 */
class WatermarkData
{
	public:
		virtual ~WatermarkData() = default;

		/**
		 * @brief setSize Utilisé pour dire combien de bits de donnée on va mettre
		 * @param size nombre de bits de donnée attendus.
		 *
		 * Cette méthode :
		 * - VIDE LE TABLEAU DE BITS.
		 * - rajoute au début du tableau de bits un "header" de taille fixée
		 *   à 64 bits contenant la taille du message à lire par la suite.
		 *
		 * Il faut donc l'appeler au début, avant d'avoir ajouté des données.
		 */
		void setSize(const uint64_t size)
		{
			const auto size_bits = sizeof(_size) * 8U;
			_size = size;

			bits.clear();
			bits.resize(size_bits);

			std::bitset<size_bits> num(_size);

			for(auto i = 0U; i < size_bits; ++i)
			{
				//std::cerr << num[i] << " ";
				bits[i] = num[i];
			}
		}

		/**
		 * @brief readSizeFromBits
		 * @return Le nombre de bits de données.
		 *
		 * Lit les 64 premiers bits, les stocke et les renvoie.
		 */
		uint64_t readSizeFromBits()
		{
			const auto size_bits = sizeof(_size) * 8U;
			std::bitset<size_bits> num;

			if(bits.size() >= 64)
			{
				for(auto i = 0U; i < size_bits; ++i)
					num[i] = bits[i];

				return _size = num.to_ullong();
			}
			else
			{
				return _size = 0;
			}
		}

		/**
		 * @brief printBits
		 *
		 * Affiche les bits de donnée.
		 * TODO : surchrage qui prend un flux personnalisé à la place.
		 * (ou opérateur de flux pour la classe)
		 */
		std::string printBits()
		{
			std::stringstream s;
			const auto size_bits = sizeof(_size) * 8U;

			if(bits.size() >= _size + size_bits)
			{
				for(auto i = size_bits; i < size_bits + _size; ++i)
				{
					s << bits[i];
				}
			}
			else
			{
				std::cerr << "Erreur: impossible de lire les données" << std::endl;
			}

			//std::cerr << s.str() << std::endl;
			return s.str();
		}

		/**
		 * @brief resetPosition Remet le curseur de lecture à 0
		 *
		 * Utilisé lorsqu'on veut écrire plusieurs fois la watermark dans
		 * les données audio.
		 * Note : remet à 0, header inclus. Donc on recopie le header.
		 */
		void resetPosition()
		{
			_position = 0;
		}

		/**
		 * @brief resetData
		 *
		 * Remet les données à 0
		 */
		void resetData()
		{
			resetPosition();
			bits.clear();
			_size = 0;
		}

		/**
		 * @brief nextBit
		 * @return Valeur du prochain bit
		 */
		virtual bool nextBit() = 0;

		/**
		 * @brief setNextBit
		 * @param b prochain bit
		 */
		virtual void setNextBit(bool b) = 0;

		/**
		 * @brief isComplete
		 * @return vrai si on a fini de lire les bits.
		 */
		virtual bool isComplete() = 0;

		std::vector<bool>& getBaseBits()
		{
			return bits;
		}

		int compareBits(std::vector<bool>& otherBits)
		{
			int e = 0;
			for(auto i = 0U; i < std::min(bits.size(), otherBits.size()); i++)
				if(bits[i] != otherBits[i])
					e++;

			if(otherBits.size() < bits.size())
				e += std::abs(bits.size() - otherBits.size());
			return e;
		}

	protected:
		std::vector<bool> bits = { }; /**< Les données. */
		uint64_t _size = 0; /**< Le nombre de bits de donnée. i.e. NE CONTIENT PAS LE NOMBRE DE BITS DU HEADER. */
		unsigned int _position = 0; /**< Curseur utilisé pour savoir ou on en est. */
};


using WatermarkData_p = std::shared_ptr<WatermarkData>;
