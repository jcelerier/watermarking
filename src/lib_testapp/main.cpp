#include "TestHeader.h"
//void TestSSW();
void TestLSB();
void TestBenchmark();
void testCopy();
void testWindow();
void TestRLSB();

class WatermarkingTests: public QObject
{
		Q_OBJECT
private slots:
    //void initTestCase()	{ }
    //void window() { testWindow(); }
    //void copy() { testCopy(); }
    //void benchmark() { TestBenchmark(); }
    void lsb() { TestLSB(); }
    void rlsb() {TestRLSB(); } // passer voir le RLSBDecode.h pour utilisation
    //void ssw() { TestSSW(); }
    //void cleanupTestCase() { }
};

//QTEST_MAIN(WatermarkingTests)

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QTest::qExec(new WatermarkingTests);
    //TestSSW();
    //TestLSB();
	return 0;
}

#include "main.moc"
