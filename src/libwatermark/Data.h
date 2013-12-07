#pragma once
#include <vector>
#include <memory>

/**
 * @brief The IData struct
 *
 * Ok, ce truc est pas tout à fait évident à comprendre.
 * "à quoi ça sert une classe ou y'a qu'un destructeur ????"
 * Tout le problème vient du système de typage du C++ très strict.
 *
 * Grosso modo, on veut pouvoir avoir une fonction qui puisse prendre
 * indépendemment un vecteur de doubles ou de complexes
 * (en fait un vecteur de vecteurs si on est en multicanal).
 *
 * Les deux possibilités sont soit de faire deux héritages, l'un avec un vecteur de double
 * et l'autre avec un vecteur de complexe, ou alors de faire une template ce qui m'a
 * semblé plus logique.
 */
struct IData
{
		virtual ~IData() = default;
};

using Audio_p = std::unique_ptr<IData>;


template<typename T>
/**
 * @brief The CData struct
 *
 * Ici on a donc la "classe réelle" qu'on utilise toujours et qui hérite de IData. (I pour Interface ou Imaginaire,
 * C pour Concrète).
 *
 * C'est pour ça q'il faut recopier la ligne ignoble avec des static_cast de machin truc en début d'algo.
 */
struct CData : public IData
{
	std::vector<std::vector<T>> _data = {};
};
