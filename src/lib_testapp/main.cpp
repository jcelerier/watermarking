#include "TestHeader.h"
void TestLSB();
void TestBenchmark();
void testCopy();
void testWindow();
void TestRLSB();
void TestSSW();

class WatermarkingTests: public QObject
{
		Q_OBJECT
private slots:
	void window() { testWindow(); }
	/*void copy() { testCopy(); }
	void benchmark() { TestBenchmark(); }
	void lsb() { TestLSB(); }
	void rlsb() {TestRLSB(); } // passer voir le RLSBDecode.h pour utilisation
	void ssw() { TestSSW(); }*/
};

int main(int argc, char** argv)
{
	testWindow();
	//QCoreApplication app(argc, argv);
	//QTest::qExec(new WatermarkingTests);

	return 0;
}

#include "main.moc"
