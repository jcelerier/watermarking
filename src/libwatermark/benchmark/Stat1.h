#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"

template <typename data_type>
class Stat1 : public BenchmarkBase<data_type>
{
        using BenchmarkBase<data_type>::conf;
        using size_type = typename Parameters<data_type>::size_type;

    public:
        Stat1(const Parameters<data_type>& configuration):
            BenchmarkBase<data_type>(configuration)
        {
			this->_name = "Stat1";
        }

        virtual void operator()(Audio_p& data) override
        {
			auto& channelsData = getAudio<data_type>(data);

            for(auto& sampleData : channelsData)
            {
                for(auto i = 1U; i < sampleData.size(); i ++)
                {
                    sampleData[i]=(sampleData[i-1]+sampleData[i]+sampleData[i+1])/3;
                }
            }
        }

};
