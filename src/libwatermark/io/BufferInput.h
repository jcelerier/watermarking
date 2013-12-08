#pragma once
#include <algorithm>

#include "InputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
/**
 * @brief The BufferInput class
 *
 * Lit (et convertit potentiellement)
 * un gros buffer (genre 2048 samples)
 */
class BufferInput : public InputManagerBase<data_type>
{
		using IOManagerBase<data_type>::v;
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;

		using size_type = typename Parameters<data_type>::size_type;

	public:
		BufferInput(Parameters<data_type>& cfg):
			InputManagerBase<data_type>(cfg)
		{
		}

		template<typename external_type>
		void readBuffer(external_type * buffer, const size_type n_frames, const size_type chans)
		{
			// 1. On définit les trucs à la bonne taille.
			if(channels() != chans)
				v().resize(chans);

			if(frames() != n_frames)
				for(auto& chan : v())
					chan.resize(n_frames);

			// 2. Vecteur temporaire qui va recevoir les données entrelacées
			std::vector<data_type> vec;
			vec.resize(frames() * channels());

			// 3. Conversion ou copie
			if(typeid(data_type) == typeid(external_type))
				std::copy(buffer,
						  buffer + n_frames * chans,
						  vec.begin());
			else
				std::transform(buffer,
							   buffer + n_frames * chans,
							   vec.begin(),
							   MathUtil::ToDouble<external_type>);

			// 4. Désentrelacement
			v() = MathUtil::deinterleave(vec, (unsigned int) chans, (unsigned int) n_frames);
		}
};
