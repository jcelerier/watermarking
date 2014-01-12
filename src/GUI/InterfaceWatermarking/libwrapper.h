#ifndef LIBWRAPPER_H
#define LIBWRAPPER_H

#include <QWidget>

#include "ui_mainwindow.h"
#include "libwatermark/manager/WatermarkManager.h"
#include "libwatermark/io/FileInput.h"
#include "libwatermark/io/FileOutput.h"
#include "libwatermark/io/DummyOutput.h"
#include "libwatermark/watermark/LSBEncode.h"
#include "libwatermark/watermark/LSBDecode.h"
#include "libwatermark/watermark/SSWEncode.h"
#include "libwatermark/watermarkdata/SimpleWatermarkData.h"

#include <memory>

class LibWrapper : public QWidget
{
Q_OBJECT

public:
    LibWrapper();
	~LibWrapper();
	LibWrapper(Ui::MainWindow* gui);


public slots:
	void encode();
    void decode();
	void loadHostWatermarkFile();
	void updateMethodConfigurationTab(int i);

    void loadConfigurationScriptMethodLsb();
    void loadConfigurationScriptMethodSsw();
    void loadConfigurationScriptMethodCompExp();

    void saveConfigurationScriptMethodLsb();
    void saveConfigurationScriptMethodSsw();
    void saveConfigurationScriptMethodCompExp();

    void selectLsbMethodActionSlot();
    void selectSswMethodActionSlot();
    void selectCompExpMethodActionSlot();

    void setLsbDefaultConfigurationValue();
    void setSswDefaultConfigurationValue();
    void setCompExpDefaultConfigurationValue();

    void updateWatermarkCapacityProgressBar();

    void loadTextWatermarkFile();

private:
	void dataToBits();
	void bitsToData();

	bool defineSavedFile();

	Ui::MainWindow* m_gui;

	QString m_inputName;
	QString m_outputName;

    int m_nbFramesBase;
    int m_sampleSizeBase;
    int m_computedNbFrames;

	WatermarkData_p m_data;

    QString m_ProgressBarDanger = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}";
    QString m_ProgressBarSafe= "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}";

};

#endif // LIBWRAPPER_H
