#pragma once
#include "IOManagerBase.h"

// Note : les output DOIVENT faire un delete du buffer qu'elles prennent en entrée.
// Ou std::move ?
class OutputManagerBase : public IOManagerBase
{
		OutputManagerBase() = default;
		virtual ~OutputManagerBase() = default;

		virtual void setNextBuffer(data_type* buffer) = 0;
};
