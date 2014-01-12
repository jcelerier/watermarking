#pragma once
#include <algorithm>

#include "InputCopy.h"

//TODO si OLA, il faut rajouter un buffer au début pour éviter
// que ça se fasse trop normaliser
template <typename data_type>
/**
 * @brief The InputOLA class
 *
 * Copie en mode overlap-add. Explications :
 * http://www-dsp.elet.polimi.it/ispg/images/pdf/audio/materiale/ola.pdf
 */
class InputOLA : public InputCopy<data_type>
{
	public:
		InputOLA(const Parameters<data_type>& cfg, double ofact = 2, unsigned int zeropad = 0):
			InputCopy<data_type>(cfg),
			overlapFactor(ofact),
			zeroPaddedSamples(zeropad)
		{
		}

		virtual typename InputCopy<data_type>::size_type frameIncrement() final override
		{
			return copiedSamples() / overlapFactor;
		}

		typename InputCopy<data_type>::size_type copiedSamples()
		{
			return this->conf.bufferSize - zeroPaddedSamples;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) final override
		{
			if (copiedSamples() <= big_vector_length - pos)
			{
				std::copy_n(in + pos, copiedSamples(), out);
				std::fill_n(out + copiedSamples(), zeroPaddedSamples, 0);
			}
			else
			{
				std::copy_n(in + pos, big_vector_length - pos, out);
				std::fill_n(out + big_vector_length - pos, copiedSamples() - (big_vector_length - pos), 0);
			}
		}

	private:
		double overlapFactor = 2;
		unsigned int zeroPaddedSamples = 0;
};
