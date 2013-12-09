#pragma once

#include "OutputManagerBase.h"

template <typename data_type>
/**
 * @brief The DummyOutput class
 *
 * Ecrit dans rien
 */
class DummyOutput : public OutputManagerBase<data_type>
{
	public:
		DummyOutput(Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(cfg)
		{
		}

		DummyOutput(OutputCopy<data_type> * oc, Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(oc, cfg)
		{
		}

		virtual void writeNextBuffer(Audio_p&) override
		{
		}
};
