#ifndef LIBWRAPPER_H
#define LIBWRAPPER_H

#include <QtWidgets>

#include "ui_mainwindow.h"
#include "libwatermark/WatermarkManager.h"
#include "libwatermark/io/FileInput.h"
#include "libwatermark/io/FileOutput.h"

#include "libwatermark/watermark/LSBEncode.h"
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
	void loadHostWatermarkFile();
	void updateMethodConfigurationTab(int i);
    void loadConfigurationScriptMethodLsb();
    void loadConfigurationScriptMethodSsw();
    void loadConfigurationScriptMethodCompExp();

private:
	template<typename T>
	void sub_exec(Parameters<T>& conf, Watermark_p<T> algorithm)
	{
		WatermarkManager<T> manager(conf);

		auto input = new FileInput<T>(m_inputName.toStdString(), conf);
		auto output = new FileOutput<T>(conf);

		manager.data.reset(m_data);
		manager.input.reset(input);
		manager.output.reset(output);
		manager.algorithm = algorithm;

		manager.execute();

		output->writeFile(m_outputName.toStdString().c_str());
	}

	void dataToBits();
	void bitsToData();

	Ui::MainWindow* m_gui;

	QString m_inputName;
	QString m_outputName;

	WatermarkData_p m_data;

};

#endif // LIBWRAPPER_H
