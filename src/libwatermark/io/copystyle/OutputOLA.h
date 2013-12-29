#pragma once

#include "OutputCopy.h"

template <typename data_type>
/**
 * @brief The OutputOLA class
 *
 * Copie en mode overlap-add. Explications :
 * http://www-dsp.elet.polimi.it/ispg/images/pdf/audio/materiale/ola.pdf
 */
class OutputOLA : public OutputCopy<data_type>
{
	public:
		OutputOLA(const Parameters<data_type>& cfg, unsigned int ofact = 2):
			OutputCopy<data_type>(cfg),
			overlapFactor(ofact)
		{
		}

		virtual typename OutputCopy<data_type>::size_type frameIncrement() override
		{
			return this->conf.bufferSize / overlapFactor;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) final override
		{
			for (auto j = 0U; (j < this->conf.bufferSize) && (pos + j < big_vector_length); ++j)
			{
				out[pos + j] += in[j];
			}
		}


	private:
		unsigned int overlapFactor = 2;
};
