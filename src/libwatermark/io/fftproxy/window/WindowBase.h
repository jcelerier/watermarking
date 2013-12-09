#pragma once
#include <vector>

#include "../../../Parameters.h"

template <typename data_type>
/**
 * @brief The WindowBase class
 *
 * Classe pour les fonctions de fenêtrage
 */
class WindowBase
{
	using size_type = typename Parameters<data_type>::size_type;

	public:
		WindowBase(const Parameters<data_type>& cfg):
					conf(cfg)
		{
		}

		virtual ~WindowBase() = default;

		virtual void apply(std::vector<data_type>& v, const size_type increment) = 0;

	private:
		const Parameters<data_type>& conf;
};

template<typename T>
using Window_p = std::shared_ptr<WindowBase<T>>;
