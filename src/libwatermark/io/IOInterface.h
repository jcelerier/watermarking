#pragma once

class IOInterface
{
	public:
		virtual ~IOInterface() = default;
		virtual long unsigned int channels() = 0;
		virtual long unsigned int frames() = 0;
};

class InputManagerInterface
{
	public:
		virtual ~InputManagerInterface() = default;
		virtual Audio_p getNextBuffer() = 0;
};

using Input_p = std::shared_ptr<InputManagerInterface>;

class OutputManagerInterface
{
	public:
		virtual ~OutputManagerInterface() = default;
		virtual void writeNextBuffer(Audio_p& abstract_buffer) = 0;
};

using Output_p = std::shared_ptr<OutputManagerInterface>;
