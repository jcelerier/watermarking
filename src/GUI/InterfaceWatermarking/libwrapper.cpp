#include <iostream>
#include <vector>

#include "libwrapper.h"
#include "libwatermark/mathutils/math_util.h"


/**
 * @brief LibWrapper::LibWrapper
 * Default constructor.
 */
LibWrapper::LibWrapper():
	m_data(new SimpleWatermarkData)
{


}

/**
 * @brief LibWrapper::~LibWrapper
 * Destructor.
 */
LibWrapper::~LibWrapper()
{
	delete m_data;
}

/**
 * @brief LibWrapper::LibWrapper
 * Overloaded constructor to get a pointer to the Graphical User Interface
 * containing every of its elements.
 * @param gui: pointer to the Graphical User Interface defined with Qt Designer (to link signals etc.)
 */
LibWrapper::LibWrapper(Ui::MainWindow* gui)
{
	m_gui = gui;

	connect(m_gui->watermarkSelectionButton,SIGNAL(clicked()),this,SLOT(loadHostWatermarkFile()));
	connect(m_gui->selectingMethodComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateMethodConfigurationTab(int)));
	connect(m_gui->encodeButton,SIGNAL(clicked()),this,SLOT(encode()));

    connect(m_gui->lsbLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodLsb()));
    connect(m_gui->sswLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodSsw()));
    connect(m_gui->compExpLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodCompExp()));

    //Initializing selection method tab
	m_gui->selectingMethodComboBox->setCurrentIndex(0);
	m_gui->selectingMethodTab->setTabEnabled(0,true);
	m_gui->selectingMethodTab->setTabEnabled(1,false);
	m_gui->selectingMethodTab->setTabEnabled(2,false);

}

/**
 * @brief LibWrapper::loadHostWatermarkFile
 * Function triggered by clicking on the Load Host Watermark
 * button to load the audio file that will host the watermark
 */
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

/**
 * @brief LibWrapper::updateMethodConfigurationTab
 * Function triggered by selecting any method tab.
 * @param i: index tab to enable
 */
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

/**
 * @brief LibWrapper::loadConfigurationScriptMethodLsb
 * Function triggered by clicking on the Load Configuration Script
 * for LSB method button: allow to load a configuration script
 * to apply pre-defined parameters.
 */
void LibWrapper::loadConfigurationScriptMethodLsb()
{
 //TODO: loading a configuration script for LSB method

    QString tempFile = QFileDialog::getOpenFileName(this, tr("Open script file"),
                                                    "",
                                                    tr("LSB Script File (*.txt)"));

    if(!tempFile.isEmpty())
    {
        m_gui->informationHostWatermark->setText("Opened config script for LSB method:" + tempFile);
    }
}

/**
 * @brief LibWrapper::loadConfigurationScriptMethodSsw
 * Function triggered by clicking on the Load Configuration Script
 * for SSW method button: allow to load a configuration script
 * to apply pre-defined parameters.
 */
void LibWrapper::loadConfigurationScriptMethodSsw()
{
 //TODO: loading a configuration script for SSW method

    QString tempFile = QFileDialog::getOpenFileName(this, tr("Open script file"),
                                                    "",
                                                    tr("SSW Script file(*.txt)"));

    if(!tempFile.isEmpty())
    {
        m_gui->informationHostWatermark->setText("Opened config script for SSW method:" + tempFile);
    }

}

/**
 * @brief LibWrapper::loadConfigurationScriptMethodCompExp
 * Function triggered by clicking on the Load Configuration Script
 * for Compression-Expansion method button: allow to load a configuration
 * script to apply pre-defined parameters.
 */
void LibWrapper::loadConfigurationScriptMethodCompExp()
{
 //TODO: loading a configuration script for Compression-Expansion method

    QString tempFile = QFileDialog::getOpenFileName(this, tr("Open script file"),
                                                    "",
                                                    tr("Compression-Expansion Script File (*.txt)"));

    if(!tempFile.isEmpty())
    {
        m_gui->informationHostWatermark->setText("Opened config script for Compression-Expansion method:" + tempFile);
    }
}

/**
 * @brief LibWrapper::dataToBits
 * Function that converts text data to binary sequence data.
 */
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

/**
 * @brief LibWrapper::encode
 * Function triggered by clicking on the Encode button:
 * launch the encoding algorithm thanks to the desired
 * method and parameters.
 */
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
