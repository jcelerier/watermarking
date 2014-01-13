#include "BenchmarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/SilenceInput.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "benchmark/Convolution.h"
#include "benchmark/AddBrumm.h"
#include "benchmark/AddWhiteNoise.h"
#include "benchmark/Amplify.h"
#include "benchmark/Exchange.h"
#include "benchmark/Invert.h"
#include "benchmark/Smooth.h"
#include "benchmark/ZeroCross.h"
#include "benchmark/Stat1.h"
#include "../libwatermark/watermark/mask.h"

void addBrumm()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new SilenceInput<double>(conf);
	input->silence(1024 * conf.bufferSize, 1);
	auto output = new FileOutput<double>(conf);

	auto algorithm = new AddBrumm<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_brumm.wav");
}

void convolution()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("in_noise_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Convolution<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_lowpass.wav");
}

void amplify()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Convolution<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_amplify.wav");
}

void exchange()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Exchange<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_exchange.wav");
}

void smooth()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Smooth<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_smooth.wav");
}

void invert()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Invert<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_invert.wav");
}

void zerocross()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new ZeroCross<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_zerocross.wav");
}

void stat1()
{
    Parameters<double> conf;
    BenchmarkManager<double> manager(conf);

    auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
    auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

    auto algorithm = new Stat1<double>(conf);

    manager.input.reset(input);
    manager.output.reset(output);
    manager.algorithm.reset(algorithm);

    manager.execute();

    output->writeFile("out_test_stat1.wav");
}

void TestBenchmark()
{
	addBrumm();
	amplify();
	convolution();
	exchange();
	smooth();
	invert();
	zerocross();
    stat1();
    Mask::calcul();
//    double magnitude[257];//norme au carré de la fft de la trame
//    for (int i=0;i<257;i++)
//    {
//        magnitude[i]=i;
//    }

//    //définition des fréquences
//    double f[257];
//    for (int i=0;i<257;i++)
//    {
//        f[i]=(22050*i)/257;
//    }

//    //définition du bark
//    double z[257];
//    for (int i=0;i<257;i++)
//    {
//        z[i]=13*atan(0.00076*f[i]) + 3.5*atan(pow((f[i]/7500),2));
//        z[i]=floor(z[i]);
//    }

//    //energy per critical band
//    double Spz[25]={0};
//    for (int i=0;i<257;i++)
//    {
//        for (int bark=0;bark<25;bark++)
//        {
//            if (z[i]==bark)
//                Spz[bark]=Spz[bark]+magnitude[i];
//        }
//    }


//    //Basilar membrane spreading function
//    double B[11];
//    for (int i=-5;i<6;i++)
//    {
//        B[i+5]=15.91+7.5*(i+0.474)-17.5*sqrt(1+pow((i+0.474),2));
//    }
//    //linear
//    for (int i=0;i<11;i++)
//    {
//        B[i]=pow(pow(10,((double)B[i]/(double)20)),2);
//    }

//    double Sm[35];
//    double Smbis[25];//26??
//    //convolution
//    //Sm=conv(Spz,B);//conv?
//    // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
//       for(int i = 11-1; i < 35; ++i)
//       {
//           Sm[i] = 0;                             // init to 0 before accumulate

//           for(int j = i, k = 0; k < 11; --j, ++k)
//               Sm[i] += Spz[j] * B[k];
//       }

//       // convolution from out[0] to out[kernelSize-2]
//       for(int i = 0; i < 11 - 1; ++i)
//       {
//           Sm[i] = 0;                             // init to 0 before sum

//           for(int j = i, k = 0; j >= 0; --j, ++k)
//               Sm[i] += Spz[j] * B[k];
//       }

//    for (int i=0;i<25;i++)
//    {
//        Smbis[i]=Sm[i+5];
//    }

//    //Masking threshold estimate
//    double P=1;
//    for(int i=0;i<25;i++)
//    {
//        P=P*Spz[i];
//    }

//    double S=0;
//    for(int i=0;i<25;i++)
//    {
//        S=S+Spz[i];
//    }

//    double SFM;
//    SFM=10*log10(pow((P/(((double)1/(double)25)*S)),((double)1/(double)25)));

//    double alpha=fmin(double(SFM/(-60)),1);//min(SFM/-60,1)

//    double O[25];
//    for(int i=0;i<25;i++)
//    {
//        O[i]=alpha*(14.5+i)+(1-alpha)*5.5;
//    }

//    double Traw[25];
//    for(int i=0;i<25;i++)
//    {
//        Traw[i]=pow(10,(log10(Smbis[i])-((double)O[i]/(double)10)));
//    }

//    double Pz[25]={0};
//    for (int i=0;i<257;i++)
//    {
//        for (int bark=0;bark<25;bark++)
//        {
//            if (z[i]==bark)
//                Pz[bark]=Pz[bark]+1;
//        }
//    }

//    double Tnorm[25]={0};
//    for (int i=0;i<257;i++)
//    {
//        for (int bark=0;bark<25;bark++)
//        {
//            if (z[i]==bark)
//                Tnorm[bark]=(double)Traw[bark]/(double)Pz[bark];
//        }
//    }
//    for (int i=0;i<25;i++){
//    std::cout<<Tnorm[i]<<std::endl;
//    }
}
