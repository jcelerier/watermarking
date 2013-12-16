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
LibWrapper::LibWrapper(Ui::MainWindow* gui):
	LibWrapper()
{
	m_gui = gui;

    //Connecting signals between GUI and watermark library
	connect(m_gui->watermarkSelectionButton,SIGNAL(clicked()),this,SLOT(loadHostWatermarkFile()));
	connect(m_gui->selectingMethodComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateMethodConfigurationTab(int)));
	connect(m_gui->encodeButton,SIGNAL(clicked()),this,SLOT(encode()));

    connect(m_gui->lsbLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodLsb()));
    connect(m_gui->sswLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodSsw()));
    connect(m_gui->compExpLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodCompExp()));

    m_gui->actionQuit->setShortcut(tr("CTRL+Q"));
    connect(m_gui->actionQuit,SIGNAL(triggered()),qApp, SLOT(closeAllWindows()));

    connect(m_gui->actionLoadHostWatermarkFile,SIGNAL(triggered()),this,SLOT(loadHostWatermarkFile()));

    connect(m_gui->selectLsbMethodAction,SIGNAL(triggered()),this,SLOT(selectLsbMethodActionSlot()));
    connect(m_gui->selectSswMethodAction,SIGNAL(triggered()),this,SLOT(selectSswMethodActionSlot()));
    connect(m_gui->selectCompExpMethodAction,SIGNAL(triggered()),this,SLOT(selectCompExpMethodActionSlot()));

    //Initializing selection method tab
	m_gui->selectingMethodComboBox->setCurrentIndex(0);
	m_gui->selectingMethodTab->setTabEnabled(0,true);
	m_gui->selectingMethodTab->setTabEnabled(1,false);
	m_gui->selectingMethodTab->setTabEnabled(2,false);

    //Initializing watermark module
    m_gui->watermarkBeginningTime->setEnabled(false);
    m_gui->watermarkEndingTime->setEnabled(false);
    m_gui->usedWatermarkCapacity->setEnabled(false);

}

void LibWrapper::selectLsbMethodActionSlot()
{
    m_gui->selectingMethodComboBox->setCurrentIndex(0);
    updateMethodConfigurationTab(0);
}

void LibWrapper::selectSswMethodActionSlot()
{
    m_gui->selectingMethodComboBox->setCurrentIndex(1);
    updateMethodConfigurationTab(1);
}

void LibWrapper::selectCompExpMethodActionSlot()
{
    m_gui->selectingMethodComboBox->setCurrentIndex(2);
    updateMethodConfigurationTab(2);
}

/**
 * @brief LibWrapper::loadHostWatermarkFile
 * Function triggered by clicking on the Load Host Watermark
 * button to load the audio file that will host the watermark
 */
void LibWrapper::loadHostWatermarkFile()
{
	m_inputName = QFileDialog::getOpenFileName(this, tr("Open Audio File"),
													"",
													tr("Audio File (*.wav)"));


	if(!m_inputName.isEmpty())
	{
        //Enabling / Updating watermark module
        m_gui->watermarkBeginningTime->setEnabled(true);
        m_gui->watermarkEndingTime->setEnabled(true);
        m_gui->usedWatermarkCapacity->setEnabled(true);

		m_gui->informationHostWatermark->setText("Opened Host Watermark file:" + m_inputName);
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
    // TODO:
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

    if(!m_inputName.isEmpty() && defineSavedFile())
    {

        dataToBits();

        switch(m_gui->selectingMethodTab->currentIndex())
        {
        case 0:
        {
            Parameters<short> conf;
            auto alg = Watermark_p<short>(new LSBEncode<short>(conf));
            sub_exec<short>(conf, alg);
            m_gui->informationHostWatermark->setText("LSB Method: File " + m_outputName +" successfully saved!");
            break;
        }
        //case 1:
        //{
            /*Parameters<double> conf;
            auto alg = Watermark_p<double>(new SSWEncode<double>(conf));
            sub_exec<double>(conf, alg);
            break;*/
        //}
            //case 2:
            // Rien pour l'instant
            //break;

        default:
            m_gui->informationHostWatermark->setText("Warning: method not implemented yet");
            break;
        }
    }
    else
    {
        m_gui->informationHostWatermark->setText("Error: no Watermark host file defined!");
    }
}

/**
 * @brief LibWrapper::defineSavedFile
 * @return boolean: true if output name is correct
 * false otherwise
 */
bool LibWrapper::defineSavedFile()
{
    m_outputName = QFileDialog::getSaveFileName(this, tr("Save Watermarked Output File (.wav)"),
                                                "./",
                                                tr("Audio File (*.wav)"));

    if(!m_outputName.isEmpty())
    {

        if(!m_outputName.endsWith(".wav"))
            m_outputName.append(".wav");
        return true;
    }
    else
        return false;
}
