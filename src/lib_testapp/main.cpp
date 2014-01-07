#include "TestHeader.h"
void TestSSW();
void TestLSB();
void TestBenchmark();
void testCopy();
void testWindow();

class WatermarkingTests: public QObject
{
		Q_OBJECT
private slots:
	void initTestCase()	{ }
	//void window() { testWindow(); }
	//void copy() { testCopy(); }
	//void benchmark() { TestBenchmark(); }
	//void lsb() { TestLSB(); }
	void ssw() { TestSSW(); }
	void cleanupTestCase() { }
};

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	QTest::qExec(new WatermarkingTests);
	return 0;
}

#include "main.moc"
