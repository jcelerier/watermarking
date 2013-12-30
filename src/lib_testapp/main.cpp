#include "transform/MCLT.h"
#include "mathutils/math_util.h"
#include <iostream>
#include <vector>
void TestSSW();
void TestLSB();
void TestBenchmark();
void testCopy();
void testWindow();

int main()
{
	testWindow();
	testCopy();
	TestBenchmark();
	TestLSB();
	//TestSSW();
	return 0;
}

