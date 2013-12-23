#ifndef LIBWRAPPER_H
#define LIBWRAPPER_H

#include <QWidget>

#include "ui_mainwindow.h"
#include "libwatermark/WatermarkManager.h"
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

private:
	void dataToBits();
	void bitsToData();

	bool defineSavedFile();

	Ui::MainWindow* m_gui;

	QString m_inputName;
	QString m_outputName;

	WatermarkData_p m_data;

};

#endif // LIBWRAPPER_H
