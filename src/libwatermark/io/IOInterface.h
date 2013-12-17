#pragma once

struct IOInterface
{
		virtual ~IOInterface() = default;
		virtual long unsigned int channels() = 0;
		virtual long unsigned int frames() = 0;
};
