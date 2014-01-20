#pragma once

#include "io/FileInput.h"
#include "io/FileOutput.h"

#include "io/BufferInput.h"
#include "io/BufferOutput.h"
#include "io/SilenceInput.h"
#include "io/DummyOutput.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "watermarkdata/SimpleWatermarkData.h"
#include "manager/WatermarkManager.h"
#include "manager/BenchmarkManager.h"
#include "watermark/LSBEncode.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "io/copystyle/InputOLA.h"
#include "io/copystyle/OutputOLA.h"
#include "benchmark/Convolution.h"
#include "benchmark/AddBrumm.h"
#include "benchmark/AddWhiteNoise.h"
#include "benchmark/Amplify.h"
#include "benchmark/Exchange.h"
#include "benchmark/Invert.h"
#include "benchmark/FFTAmplify.h"
#include "benchmark/Smooth.h"
#include "benchmark/ZeroCross.h"
#include "benchmark/Stat1.h"
#include "benchmark/FFTNoise.h"

#include "manager/BenchmarkManager.h"

#include "transform/FFTWManager.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include <string>
#include <algorithm>
#include <random>

#include "../Parameters.h"
#include "../Data.h"
#include "../watermarkdata/WatermarkData.h"
#include "../watermark/WatermarkBase.h"


template <typename benchmark_type,
          class benchmark_algo,
          typename watermark_type,
          class watermark_encode_algo,
          class watermark_decode_algo>
class BitError
{
public:
    BitError()
    {
        encodeData->setSize(7);
        encodeData->setNextBit(true);
        encodeData->setNextBit(false);
        encodeData->setNextBit(true);
        encodeData->setNextBit(true);
        encodeData->setNextBit(false);
        encodeData->setNextBit(false);
        encodeData->setNextBit(true);

    }

    void execute(std::string fileName)
    {
        // Encodage
        static_cast<FileInput<watermark_type>*>(encodeInput.get())->readFile(fileName);

        WatermarkManager encodeManager(encodeInput,
                                       encodeOutput,
                                       encodeAlgo,
                                       encodeData);

        encodeManager.prepare();
        encodeManager.execute();

        // Copie du buffer (optimiser avec un move ?)
        auto wo = static_cast<BufferOutput<watermark_type>*>(encodeOutput.get());
        auto bi = static_cast<BufferInput<benchmark_type>*>(binput.get());
        bi->readFromBufferOutput(wo);
		auto bo = static_cast<BufferOutput<benchmark_type>*>(boutput.get());


		InputCopy<double>* iCp = nullptr;
		OutputCopy<double>* oCp = nullptr;

		if(dynamic_cast<FilterProperty*>(benchAlgo.get()))
		{
			iCp = new InputFilter<double>(11, bparams);
			oCp = new OutputFilter<double>(11, bparams);
		}
		else
		{
			iCp = new InputSimple<double>(bparams);
			oCp = new OutputSimple<double>(bparams);
		}

		bi->copyHandler.reset(iCp);
		bo->copyHandler.reset(oCp);
		Input_p input;
		Output_p output;

		if(dynamic_cast<FFTProperty*>(benchAlgo.get()))
		{
			FFT_p<double> fft_m(new FFTWManager<double>(bparams));
			fft_m->setChannels((unsigned int) bi->channels());
			auto fft_i = new FFTInputProxy<double>(binput, fft_m, bparams);
			auto fft_o = new FFTOutputProxy<double>(boutput, fft_m, bparams);

			input.reset(fft_i);
			output.reset(fft_o);
		}
		else
		{
			input = binput;
			output = boutput;
		}

        // Application de la dégradation
		BenchmarkManager benchmarkManager(input,
										  output,
                                          benchAlgo);

        benchmarkManager.execute();


        // Copie du buffer (optimiser avec un move ?)

        auto wi = static_cast<BufferInput<watermark_type>*>(decodeInput.get());
        wi->readFromBufferOutput(bo);

        // Décodage
        WatermarkManager decodeManager(decodeInput,
                                       decodeOutput,
                                       decodeAlgo,
                                       decodeData);

        decodeManager.prepare();
        decodeManager.execute();

        decodeData->readSizeFromBits();

        //Calcule le nombre de bits faux
        this->computeError();

    }


    void computeError()
    {
        double E=0.0;
        encodeData->resetPosition();
        decodeData->resetPosition();
        while((!encodeData->isComplete()) && (!decodeData->isComplete()))
        {
            if (decodeData->nextBit() != encodeData->nextBit())
                E += 1;
        }

        error=E;
    }

    double getError() const
    {
        return error;
    }


private:
    /// Watermark
    Parameters<watermark_type> wparams{};

    /// Encodage
    Input_p encodeInput{new FileInput<watermark_type>(wparams)};
    Output_p encodeOutput{new BufferOutput<watermark_type>(wparams)};
    Watermark_p encodeAlgo{new watermark_encode_algo(wparams)};
    WatermarkData_p encodeData{new SimpleWatermarkData};

    /// Décodage
    Input_p decodeInput{new BufferInput<watermark_type>(wparams)};
    Output_p decodeOutput{new DummyOutput<watermark_type>(wparams)};
    Watermark_p decodeAlgo{new watermark_decode_algo(wparams)};
    WatermarkData_p decodeData{new SimpleWatermarkData};

    /// Benchmark
    Parameters<benchmark_type> bparams{};
    Input_p binput{new BufferInput<benchmark_type>(bparams)};
    Output_p boutput{new BufferOutput<benchmark_type>(bparams)};
    Benchmark_p benchAlgo{new benchmark_algo(bparams)};

    double error = 0.0;
};





