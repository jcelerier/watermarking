#include <iostream>
#include <vector>

#include "libwrapper.h"
#include "libwatermark/mathutils/math_util.h"


LibWrapper::LibWrapper():
	m_data(new SimpleWatermarkData)
{


}

LibWrapper::~LibWrapper()
{
	delete m_data;
}

LibWrapper::LibWrapper(Ui::MainWindow* gui)
{
	m_gui = gui;

	connect(m_gui->watermarkSelectionButton,SIGNAL(clicked()),this,SLOT(loadHostWatermarkFile()));
	connect(m_gui->selectingMethodComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateMethodConfigurationTab(int)));
	connect(m_gui->encodeButton,SIGNAL(clicked()),this,SLOT(encode()));

	//Initializing selection method tab
	m_gui->selectingMethodComboBox->setCurrentIndex(0);
	m_gui->selectingMethodTab->setTabEnabled(0,true);
	m_gui->selectingMethodTab->setTabEnabled(1,false);
	m_gui->selectingMethodTab->setTabEnabled(2,false);

}

void LibWrapper::loadHostWatermarkFile()
{

	QString tempFile = QFileDialog::getOpenFileName(this, tr("Open Audio File"),
													"",
													tr("Audio File (*.wav)"));



	if(!tempFile.isEmpty())
	{
		m_gui->informationHostWatermark->setText("Opened Host Watermark file:" + tempFile);
	}
}

void LibWrapper::updateMethodConfigurationTab(int i)
{

	switch(i)
	{

		case 0: // lsb method selected
			m_gui->selectingMethodTab->setTabEnabled(0,true);
			m_gui->selectingMethodTab->setTabEnabled(1,false);
			m_gui->selectingMethodTab->setTabEnabled(2,false);
			m_gui->selectingMethodTab->setCurrentIndex(0);

			break;

		case 1: // ssw method selected
			m_gui->selectingMethodTab->setTabEnabled(0,false);
			m_gui->selectingMethodTab->setTabEnabled(1,true);
			m_gui->selectingMethodTab->setTabEnabled(2,false);
			m_gui->selectingMethodTab->setCurrentIndex(1);
			break;

		case 2: // compression-expansion method selected
			m_gui->selectingMethodTab->setTabEnabled(0,false);
			m_gui->selectingMethodTab->setTabEnabled(1,false);
			m_gui->selectingMethodTab->setTabEnabled(2,true);
			m_gui->selectingMethodTab->setCurrentIndex(2);
			break;

		default:
			break;

	}
}

void LibWrapper::dataToBits()
{
	// 1. Calculer la taille en bits
	//m_data->setSize(taille); // taille ici

	// 2. Faire des m_data->setNextBit(true / false) selon.

	m_data->setSize(7);
	m_data->setNextBit(true);
	m_data->setNextBit(false);
	m_data->setNextBit(true);
	m_data->setNextBit(true);
	m_data->setNextBit(false);
	m_data->setNextBit(false);
	m_data->setNextBit(true);
}

void LibWrapper::encode()
{
	dataToBits();

	switch(m_gui->selectingMethodTab->currentIndex())
	{
		case 0:
		{
			Parameters<short> conf;
			auto alg = Watermark_p<short>(new LSBEncode<short>(conf));
			sub_exec<short>(conf, alg);
			break;
		}
		case 1:
		{
			/*Parameters<double> conf;
			auto alg = Watermark_p<double>(new SSWEncode<double>(conf));
			sub_exec<double>(conf, alg);
			break;*/
		}
		//case 2:
			// Rien pour l'instant
			//break;
	}
}
