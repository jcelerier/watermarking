#include <iostream>
#include "watermark_manager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "watermark/GainTest.h"
using namespace std;

int main()
{
	Parameters conf;
	auto manager = new WatermarkManager(conf);
	manager->_input.reset(new FileInput("test_file.raw", conf));
	auto output(std::shared_ptr<FileOutput>(new FileOutput(conf)));
	manager->_output = output;
	manager->_watermark.reset(new GainTest(conf));

	manager->execute();

	output->writeFile("out_test.raw");


	cout << "Hello World!" << endl;
	return 0;
}

