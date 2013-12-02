#pragma once
#include <algorithm>

#include "InputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class SilenceInput : public InputManagerBase<data_type>
{
		using IOManagerBase<data_type>::v;
		using IOManagerBase<data_type>::frames;

		using size_type = typename Parameters<data_type>::size_type;

	public:
		SilenceInput(Parameters<data_type>& cfg):
			InputManagerBase<data_type>(cfg)
		{
		}

		SilenceInput(InputCopy<data_type>* icp, Parameters<data_type>& cfg):
			InputManagerBase<data_type>(icp, cfg)
		{
		}

		void silence(const size_type n_frames, const size_type chans, const data_type val = 0)
		{
			// 1. On définit les trucs à la bonne taille.
			if(this->channels() != chans)
				v().resize(chans);

			if(frames() != n_frames)
				for(auto& chan : v())
					chan.resize(n_frames);

			for(auto& channel : v())
				std::fill(channel.begin(), channel.end(), val);
		}
};
