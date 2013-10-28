#pragma once
#include "copystyle/input.h"
#include "copystyle/output.h"
#include <memory>

class IOManagerBase
{
	public:
		using size_type = unsigned int;
		using data_type = double;

		virtual size_type getBufferSize() final;
		virtual void setBufferSize(size_type bufferSize) final;


	protected:
		std::shared_ptr<Input> _input_copy_impl = nullptr;
		std::shared_ptr<Output> _output_copy_impl = nullptr;

		size_type _dataLength = 0;
		size_type _pos = 0;

		size_type _bufferSize = 512; /**< TODO */

		size_type _ola_frame_increment = 0; /**< TODO */
		size_type _std_frame_increment = 0; /**< TODO */
};
