#pragma once
#include <memory>
#include <complex>

#define DEBUG(x) std::cerr << #x << ": " << ( x ) << std::endl;
#ifndef M_PI
#define M_PI 3.141592653589793238462
#endif

/**
 * @brief The Parameters struct
 *
 * Contient des paramètres utiles et pertinent pour l'application
 * de la watermark ou d'effets audio.
 *
 * Explication au niveau des tailles de buffer :
 *
 * La taille de buffer ici est la taille de base : si par exemple on a un fichier audio de 2048 échantillons
 * (au sens sndfile, i.e. un échantillon peut être mono, stereo, 5.1, 22.6...)
 * Alors avec un buffer de 512, et pas d'overlap, on aura 4 passages par l'algorithme choisi.
 *
 * Cependant, lorsqu'on applique des FFT ou autre, la taille du buffer qu'on récupère dans l'algorithme peut changer.
 * Par exemple, avec FFTW, on ne récupère que la partie de gauche du spectre, vu qu'il est symmétrique, plus le 0,
 * ce qui fait 257 samples.
 *
 * En règle général, il vaut donc mieux ne jamais supposer de taille de buffer et faire
 * "jusqu'à buffer.size()".
 */
template <typename data_type>
struct Parameters
{
		using size_type = long unsigned int; /**< A utiliser pour les longueurs de tableau, etc. */
		using complex_type = std::complex<data_type>; /**< A utiliser pour les nombres complexes. */

		size_type samplingRate = 44100U; /**< Fréquence d'échantillonnage. */
		size_type bufferSize = 512U; /**< Taille de buffer. */

		/**
		 * @brief normFactor Facteur de normalisation
		 * @return La valeur d'une sample maximale pour le type choisi.
		 *
		 * Exemple : si on travaille en doubles ça renvoie 1, en shorts 32768, etc..
		 */
		data_type normFactor()
		{
			if(typeid(data_type) == typeid(double))
				return 1.0;
			if(typeid(data_type) == typeid(short))
				return std::pow(2, 15);
			return std::pow(2, 31);
		}
};
