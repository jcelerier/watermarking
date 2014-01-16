#pragma once

#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/SilenceInput.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "benchmark/BitError.h"
#include "watermarkdata/SimpleWatermarkData.h"
#include "manager/WatermarkManager.h"
#include "manager/BenchmarkManager.h"
#include "watermark/LSBEncode.h"
#include <string>
#include <algorithm>
#include <random>

#include "../Parameters.h"
#include "../Data.h"
#include "../watermarkdata/WatermarkData.h"
#include "../watermark/WatermarkBase.h"


template <class benchmark_algo>
class BitError
{

    public:
        BitError(std::string input_name, benchmark_algo algo, WatermarkData* data)
        {
            input = new FileInput<short>(input_name, conf);
            this->data = data;
            this->algo = algo;
        }

        void execute()
        {

        Parameters<double> conf2;

        WatermarkManager manager(Input_p(input),
                                 Output_p(output),
                                 Watermark_p(new LSBEncode<short>(conf)),
                                 WatermarkData_p(data));

        manager.prepare();
        manager.execute();


        auto output2 = new FileOutput<double>(new OutputFilter<double>(11, conf2), conf2);


        BenchmarkManager benchmarkManager(Input_p(output),
                                          Output_p(output2),
                                          algo);


        benchmarkManager.execute();

        this->computeError(Input_p(benchmarkManager.output()));

        }


//    void computeError(Input_p input) //overrideAudio_p& data, WatermarkData& watermark2
//    {
//       double E=0;
//       WatermarkData* watermark = data;
//       //auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

//        for(auto& buffer : input->getNextBuffer())
//        {
//            for(auto i = 0U; i < buffer.size(); i ++)
//            {
//                if(!watermark->isComplete())
//                {
//                    if (buffer[i]!= watermark->nextBit())
//                  E+=1;
//                }
//            }
//        }
//        error=E;//watermark2.readSizeFromBits();

//        std::cout<<"plouf"<<std::endl;
//    }

        double getError() const
        {
        return error;
        }


    private:
        Parameters<short> conf;
        FileInput<short> input ;
        FileOutput<short> output = new FileOutput<short>(conf);
        WatermarkData* data;
        benchmark_algo algo;
        double error = 0.0;


        //    auto input = new FileInput<short>(input_name, conf);
        //    auto output = new FileOutput<short>(conf);
};





