#ifndef LIBWRAPPER_H
#define LIBWRAPPER_H

#include <QWidget>

#include "ui_mainwindow.h"
#include "settingsmanager.h"
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
		friend class SettingsManager;
	public:
		LibWrapper();
		~LibWrapper();
		LibWrapper(Ui::MainWindow* gui);


	public slots:
		// Propre a l'interface avec l'api
		void encode();
		void decode();

		void generateSSWSequences();

		void applyDegradation();

		// Trucs de l'interface qui ont rien à faire ici
		void loadHostWatermarkFile();

		void updateMethodConfigurationTab(int i);

		void selectLsbMethodActionSlot();
		void selectSswMethodActionSlot();
		void selectCompExpMethodActionSlot();

		void setLsbDefaultConfigurationValue();
		void setSswDefaultConfigurationValue();
		void setCompExpDefaultConfigurationValue();

		void updateWatermarkCapacityProgressBar();

		void loadTextWatermarkFile();


		void loadFileToDecode();

		void setBufferSize(int size);

	private:


		template <typename T>
		void drawOutputWaveform(FileOutput<T>* output, Parameters<T>& conf)
		{
			m_gui->waveformInputWidget->setVisible(true);

			QVector<double> x(output->frames()), y(output->frames());
			QVector<T> y0 = QVector<T>::fromStdVector(output->v(0));

			std::transform(y0.begin(), y0.end(), y.begin(),
						   [&] (T x) { return double(x) / conf.normFactor(); });
			std::iota(x.begin(), x.end(), 0);


			m_gui->waveformOutputWidget->addGraph();
			m_gui->waveformOutputWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

			connect(m_gui->waveformOutputWidget->xAxis, SIGNAL(rangeChanged(QCPRange)),m_gui->waveformOutputWidget->xAxis2, SLOT(setRange(QCPRange)));
			connect(m_gui->waveformOutputWidget->yAxis, SIGNAL(rangeChanged(QCPRange)),m_gui->waveformOutputWidget->yAxis2, SLOT(setRange(QCPRange)));

			//m_gui->waveformOutputWidget->plotLayout()->insertRow(0);
			//m_gui->waveformOutputWidget->plotLayout()->addElement(0, 0, new QCPPlotTitle(m_gui->waveformOutputWidget, "Output audio file Waveform"));

			m_gui->waveformOutputWidget->graph(0)->setPen(QPen(Qt::red));

			m_gui->waveformOutputWidget->graph(0)->setData(x,y);
			m_gui->waveformOutputWidget->graph(0)->setName("Output waveform");

			m_gui->waveformOutputWidget->xAxis->setRange(0,output->frames());
			m_gui->waveformOutputWidget->yAxis->setRange(-1,1);

			m_gui->waveformOutputWidget->xAxis->setLabel("Last output waveform");

			m_gui->waveformOutputWidget->replot();

		}

		void dataToBits();
		void bitsToData();

		bool defineSavedFile();

		Ui::MainWindow* m_gui;

		QString m_inputName;
		QString m_outputName;

		int m_nbChannels;
		int m_nbFrames;

		QString m_ProgressBarDanger = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}";
		QString m_ProgressBarSafe= "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}";


		// Trucs de la librairie
		WatermarkData_p m_data;
		WatermarkManager m_manager;

		SettingsManager m_settings;

		// Pour SSW
		Parameters<double> sswParams;
		void plotInput(FileInput<short>* input, unsigned int sr);
};

#endif // LIBWRAPPER_H
