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
		using OutputManagerBase<data_type>::OutputManagerBase;

		virtual void writeNextBuffer(Audio_p&) override
		{
		}
};
