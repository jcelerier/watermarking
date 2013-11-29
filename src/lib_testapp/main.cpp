#include "mathutils/MCLT.h"


void TestSSW();
void TestLSB();
void TestMCLT()
{
	MCLTManager<double> m;
	std::vector<std::complex<double>> b(256);
	for(int i = 0; i < 256; i++)
		b[i] = std::complex<double>(i, i);

	m.forward(b);
	m.backward(b);
}
int main()
{
	TestLSB();
	TestSSW();
	return 0;
}

