#pragma once
#include <algorithm>

#include "InputManagerBase.h"
#include "BufferOutput.h"
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
		using size_type = typename Parameters<data_type>::size_type;

	public:
		using InputManagerBase<data_type>::InputManagerBase;

		template<typename external_type>
		void readBuffer(external_type * buf, const size_type n_frames, const size_type chans)
		{
			// 1. On définit les trucs à la bonne taille.
			if(this->channels() != chans)
				this->v().resize(chans);

			if(this->frames() != n_frames)
				for(auto& chan : this->v())
					chan.resize(n_frames);

			// 2. Vecteur temporaire qui va recevoir les données entrelacées
			std::vector<data_type> vec;
			vec.resize(this->frames() * this->channels());

			// 3. Conversion ou copie
			if(typeid(data_type) == typeid(external_type))
				std::copy(buf,
						  buf + n_frames * chans,
						  vec.begin());
			else
				std::transform(buf,
							   buf + n_frames * chans,
							   vec.begin(),
							   MathUtil::ToDouble<external_type>);

			// 4. Désentrelacement
			this->v() = MathUtil::deinterleave(vec, (unsigned int) chans, (unsigned int) n_frames);
		}


		template<typename external_type>
		void readFromBufferOutput(BufferOutput<external_type>* output)
		{
			if(typeid(data_type) == typeid(external_type))
			{
				this->v().resize(output->v().size());
				for(auto i = 0U; i < this->v().size(); ++i)
				{
					this->v(i).resize(output->v(i).size());
					std::copy(output->v(i).begin(),
								   output->v(i).end(),
								   this->v(i).begin());
				}

			}
			else if(typeid(external_type) == typeid(short) &&
					typeid(data_type) == typeid(double)) // Short to double
			{
				this->v().resize(output->v().size());
				for(auto i = 0U; i < this->v().size(); ++i)
				{
					this->v(i).resize(output->v(i).size());
					std::transform(output->v(i).begin(),
								   output->v(i).end(),
								   this->v(i).begin(),
								   MathUtil::ToDouble<external_type>);
				}
			}
			else if(typeid(external_type) == typeid(double)) // Double to short
			{
				this->v().resize(output->v().size());
				for(auto i = 0U; i < this->v().size(); ++i)
				{
					this->v(i).resize(output->v(i).size());
					std::transform(output->v(i).begin(),
								   output->v(i).end(),
								   this->v(i).begin(),
								   MathUtil::FromDouble<data_type>);
				}
			}

		}
};
