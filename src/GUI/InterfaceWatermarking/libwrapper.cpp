#include <iostream>
#include <vector>

#include<QTime>
#include<QFileDialog>
#include<QMessageBox>

#include <bitset>
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

    m_gui->waveformInputWidget->setVisible(false);
    m_gui->waveformOutputWidget->setVisible(false);

    //Connecting signals between GUI and watermark library
    connect(m_gui->watermarkSelectionButton,SIGNAL(clicked()),this,SLOT(loadHostWatermarkFile()));
    connect(m_gui->selectingMethodComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateMethodConfigurationTab(int)));
    connect(m_gui->encodeButton,SIGNAL(clicked()),this,SLOT(encode()));
    connect(m_gui->decodeButton,SIGNAL(clicked()),this,SLOT(decode()));

    connect(m_gui->lsbLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodLsb()));
    connect(m_gui->sswLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodSsw()));
    connect(m_gui->compExpLoadConfigurationButton,SIGNAL(clicked()),this,SLOT(loadConfigurationScriptMethodCompExp()));

    connect(m_gui->setDefaultValueLsbPushButton,SIGNAL(clicked()),this,SLOT(setLsbDefaultConfigurationValue()));

    m_gui->actionQuit->setShortcut(tr("CTRL+Q"));
    connect(m_gui->actionQuit,SIGNAL(triggered()),qApp, SLOT(closeAllWindows()));

    connect(m_gui->actionLoadHostWatermarkFile,SIGNAL(triggered()),this,SLOT(loadHostWatermarkFile()));

    connect(m_gui->selectLsbMethodAction,SIGNAL(triggered()),this,SLOT(selectLsbMethodActionSlot()));
    connect(m_gui->selectSswMethodAction,SIGNAL(triggered()),this,SLOT(selectSswMethodActionSlot()));
    connect(m_gui->selectCompExpMethodAction,SIGNAL(triggered()),this,SLOT(selectCompExpMethodActionSlot()));

    connect(m_gui->watermarkedSelectionButton,SIGNAL(clicked()),this,SLOT(loadHostWatermarkFile()));

    connect(m_gui->textToWatermark,SIGNAL(textChanged()),this,SLOT(updateWatermarkCapacityProgressBar()));

    m_gui->usedWatermarkCapacityBar->setStyleSheet(m_ProgressBarSafe);
    m_gui->usedWatermarkCapacityBar->setAlignment(Qt::AlignCenter);

    //Initializing selection method tab
    m_gui->selectingMethodComboBox->setCurrentIndex(0);
    m_gui->selectingMethodTab->setTabEnabled(0,true);
    m_gui->selectingMethodTab->setTabEnabled(1,false);
    m_gui->selectingMethodTab->setTabEnabled(2,false);

    //Initializing watermark module
    m_gui->watermarkBeginningTime->setEnabled(false);
    m_gui->watermarkEndingTime->setEnabled(false);
    m_gui->usedWatermarkCapacityBar->setEnabled(false);

}

/**
 * @brief LibWrapper::selectLsbMethodActionSlot
 * Function triggered by the action menu to switch
 * between methods
 */
void LibWrapper::selectLsbMethodActionSlot()
{
    m_gui->selectingMethodComboBox->setCurrentIndex(0);
    m_gui->selectingDecodingMethodTab->setCurrentIndex(0);
    updateMethodConfigurationTab(0);
}

/**
 * @brief LibWrapper::selectSswMethodActionSlot
 * Function triggered by the action menu to switch
 * between methods
 */
void LibWrapper::selectSswMethodActionSlot()
{
    m_gui->selectingMethodComboBox->setCurrentIndex(1);
    m_gui->selectingDecodingMethodTab->setCurrentIndex(1);
    updateMethodConfigurationTab(1);
}

/**
 * @brief LibWrapper::selectCompExpMethodActionSlot
 * Function triggered by the action menu to switch
 * between methods
 */
void LibWrapper::selectCompExpMethodActionSlot()
{
    m_gui->selectingMethodComboBox->setCurrentIndex(2);
    m_gui->selectingDecodingMethodTab->setCurrentIndex(2);
    updateMethodConfigurationTab(2);
}

/**
 * @brief LibWrapper::loadHostWatermarkFile
 * Function triggered by clicking on the Load Host Watermark
 * button to load the audio file that will host the watermark
 */
void LibWrapper::loadHostWatermarkFile()
{

    m_inputName = QFileDialog::getOpenFileName(m_gui->centralwidget, tr("Open Audio File (.wav)"),
                                               "",
                                               tr("Audio File (*.wav)"));

    if(!m_inputName.isEmpty())
    {
        //Enabling / Updating watermark module
        m_gui->watermarkBeginningTime->setEnabled(true);
        m_gui->watermarkEndingTime->setEnabled(true);
        m_gui->usedWatermarkCapacityBar->setEnabled(true);

        m_gui->waveformInputWidget->setVisible(true);

        Parameters<short> conf;
        WatermarkManager<short> manager(conf);
        auto input = new FileInput<short>(m_inputName.toStdString(), conf);


        /* Editing watermark max length progress bar */

        m_gui->usedWatermarkCapacityBar->setMinimum(0);
        m_gui->usedWatermarkCapacityBar->setMaximum(input->frames()/8);

        //m_gui->usedWatermarkCapacityBar->setValue(m_gui->textToWatermark->toPlainText().size());

        updateWatermarkCapacityProgressBar();

        /* Computing audio input time length for initializing editing
        watermark position part */
        int inputLengthInSec = input->frames()/conf.samplingRate;

        //qDebug() << inputLengthInSec;

        QTime inputLength(0,0,0);
        inputLength = inputLength.addSecs(inputLengthInSec);

        //qDebug() << inputLength;

        m_gui->watermarkBeginningTime->setMaximumTime(inputLength);
        m_gui->watermarkEndingTime->setMaximumTime(inputLength);
        m_gui->watermarkEndingTime->setTime(inputLength);

        /* Plotting waveform using QCustomPlot module */

        //
        // TODO: plotting waveform using m_gui->waveformInputWidget
        //

        QVector<double> x,y;

        short min,max;

        x.push_back(0);
        y.push_back(input->getBaseData()[0][0]);

        min = y[0];
        max = y[0];

        for(unsigned int i = 1; i < input->frames(); i++)
        {
            x.push_back(i);
            y.push_back(input->getBaseData()[0][i]);

            if(y[i] < min) min = y[i];
            if(y[i] > max) max = y[i];
        }

        m_gui->waveformInputWidget->addGraph();
        m_gui->waveformInputWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

        connect(m_gui->waveformInputWidget->xAxis, SIGNAL(rangeChanged(QCPRange)),m_gui->waveformInputWidget->xAxis2, SLOT(setRange(QCPRange)));
        connect(m_gui->waveformInputWidget->yAxis, SIGNAL(rangeChanged(QCPRange)),m_gui->waveformInputWidget->yAxis2, SLOT(setRange(QCPRange)));

        //m_gui->waveformInputWidget->plotLayout()->insertRow(0);
        //m_gui->waveformInputWidget->plotLayout()->addElement(0, 0, new QCPPlotTitle(m_gui->waveformInputWidget, "Input audio file Waveform"));

        m_gui->waveformInputWidget->graph(0)->setData(x,y);
        m_gui->waveformInputWidget->graph(0)->setName("Input waveform");

        m_gui->waveformInputWidget->xAxis->setRange(0,input->frames());
        m_gui->waveformInputWidget->xAxis->setLabel("Input waveform");
        m_gui->waveformInputWidget->yAxis->setRange(min,max);

        m_gui->waveformInputWidget->xAxis->setTickLabels(false);
        m_gui->waveformInputWidget->yAxis->setTickLabels(false);

        m_gui->waveformInputWidget->replot();

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

    QString tempFile = QFileDialog::getOpenFileName(m_gui->centralwidget, tr("Open script file (.txt)"),
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

    QString tempFile = QFileDialog::getOpenFileName(m_gui->centralwidget, tr("Open script file (.txt)"),
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

    QString tempFile = QFileDialog::getOpenFileName(m_gui->centralwidget, tr("Open script file (.txt)"),
                                                    "",
                                                    tr("Compression-Expansion Script File (*.txt)"));

    if(!tempFile.isEmpty())
    {
        m_gui->informationHostWatermark->setText("Opened config script for Compression-Expansion method:" + tempFile);
    }
}

/**
 * @brief LibWrapper::saveConfigurationScriptMethodLsb
 * Function triggered by the save configuration script
 * button (LSB tab): allow to export configuration as
 * an XML script
 */
void LibWrapper::saveConfigurationScriptMethodLsb()
{

    m_gui->informationHostWatermark->setText("Saving configuration script query for LSB method");

    QString scriptName = QFileDialog::getSaveFileName(this, tr("Save Configuration as Script File (.wconf)"),
                                                      "./",
                                                      tr("Configuration Script File (*.wconf)"));

    //TODO

}

/**
 * @brief LibWrapper::saveConfigurationScriptMethodSsw
 * Function triggered by the save configuration script
 * button (SSW tab): allow to export configuration as
 * an XML script
 */
void LibWrapper::saveConfigurationScriptMethodSsw()
{

    m_gui->informationHostWatermark->setText("Saving configuration script query for SSW method");

    QString scriptName = QFileDialog::getSaveFileName(this, tr("Save Configuration as Script File (.wconf)"),
                                                      "./",
                                                      tr("Configuration Script File (*.wconf)"));

    //TODO
}

/**
 * @brief LibWrapper::saveConfigurationScriptMethodLsb
 * Function triggered by the save configuration script
 * button (Compression-Expansion tab): allow to export
 * configuration as an XML script
 */
void LibWrapper::saveConfigurationScriptMethodCompExp()
{

    m_gui->informationHostWatermark->setText("Saving configuration script query for Compression-Expansion method");

    QString scriptName = QFileDialog::getSaveFileName(this, tr("Save Configuration as Script File (.wconf)"),
                                                      "./",
                                                      tr("Configuration Script File (*.wconf)"));

    //TODO
}

/**
 * @brief LibWrapper::updateWatermarkCapacityProgressBar
 * Function triggered by changing the watermark text to
 * update the capacity progress bar linked
 */

void LibWrapper::updateWatermarkCapacityProgressBar()
{
    if(!m_inputName.isEmpty())
    {
        int i = m_gui->textToWatermark->toPlainText().size();
        if(i < m_gui->usedWatermarkCapacityBar->maximum())
        {
            m_gui->usedWatermarkCapacityBar->setValue(i);
            m_gui->usedWatermarkCapacityBar->setStyleSheet(m_ProgressBarSafe);
        }
        else
        {
            m_gui->usedWatermarkCapacityBar->setValue(m_gui->usedWatermarkCapacityBar->maximum());
            m_gui->usedWatermarkCapacityBar->setStyleSheet(m_ProgressBarDanger);
        }
    }
}

/**
 * @brief LibWrapper::dataToBits
 * Function that converts text data to binary sequence data.
 */
void LibWrapper::dataToBits()
{
    auto str = m_gui->textToWatermark->document()->toPlainText().toStdString();

    m_data->setSize(str.size() * 8U); // taille ici

    for (auto i = 0U; i < str.size(); ++i)
    {
        // Ce hack est affreux
        auto a = std::bitset<8>(str[i]);
        auto b = a.to_string();
        std::reverse(std::begin(b), std::end(b));
        auto c = std::bitset<8>(b);

        for(auto i = 0U; i < 8; ++i)
        {
            m_data->setNextBit(c[i]);
        }
    }
}

void LibWrapper::bitsToData()
{
    m_data->readSizeFromBits();
    std::string str = m_data->printBits();
    std::string out;

    for (auto i = 0U; i < str.size(); i += 8)
    {
        auto b = std::bitset<8>(str.substr(i, 8));
        out.push_back(static_cast<unsigned char>(b.to_ulong()));
    }

    m_gui->getDecodedDataTextEdit->setText(QString::fromStdString(out));
}

/**
 * @brief LibWrapper::encode
 * Function triggered by clicking on the Encode button:
 * launch the encoding algorithm thanks to the desired
 * method and parameters.
 */
void LibWrapper::encode()
{

    // host file loaded ? output name correctly defined ?
    if(m_inputName.isEmpty() || !defineSavedFile())
    {
        m_gui->informationHostWatermark->setText("Error: no Watermark host file defined!");
        QMessageBox::information(m_gui->centralwidget,"Warning - missing file",
                                 "Please, load a Watermark host file!");

        return;
    }

    // Displaying if watermark is too heavy for the host file
    if(m_gui->usedWatermarkCapacityBar->value() == m_gui->usedWatermarkCapacityBar->maximum())
    {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(m_gui->centralwidget, "Warning - Watermark maybe too big",
                                      "Warning: the watermark length is too big to be correctly watermarked into your selected audio file. Do you really want to encode it ?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }

    dataToBits();

    switch(m_gui->selectingMethodTab->currentIndex())
    {
    case 0:
    {
        Parameters<short> conf;
        WatermarkManager<short> manager(conf);

        auto input = new FileInput<short>(m_inputName.toStdString(), conf);
        auto output = new FileOutput<short>(conf);

        manager.data = m_data;
        manager.input.reset(input);
        manager.output.reset(output);
        manager.algorithm.reset(new LSBEncode<short>(conf));

        manager.execute();

        output->writeFile(m_outputName.toStdString().c_str());
        m_gui->informationHostWatermark->setText("LSB Method: File " + m_outputName +" successfully saved!");

        /* Computing and plotting output waveform */
        m_gui->waveformInputWidget->setVisible(true);

        QVector<double> x,y;

        short min,max;

        x.push_back(0);
        y.push_back(output->getBaseData()[0][0]);

        min = y[0];
        max = y[0];

        for(unsigned int i = 1; i < output->getFrames(); i++)
        {
            x.push_back(i);
            y.push_back(output->getBaseData()[0][i]);

            if(y[i] < min) min = y[i];
            if(y[i] > max) max = y[i];
        }

         m_gui->waveformOutputWidget->setVisible(true);

        m_gui->waveformOutputWidget->addGraph();
        m_gui->waveformOutputWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

        connect(m_gui->waveformOutputWidget->xAxis, SIGNAL(rangeChanged(QCPRange)),m_gui->waveformOutputWidget->xAxis2, SLOT(setRange(QCPRange)));
        connect(m_gui->waveformOutputWidget->yAxis, SIGNAL(rangeChanged(QCPRange)),m_gui->waveformOutputWidget->yAxis2, SLOT(setRange(QCPRange)));

        //m_gui->waveformOutputWidget->plotLayout()->insertRow(0);
        //m_gui->waveformOutputWidget->plotLayout()->addElement(0, 0, new QCPPlotTitle(m_gui->waveformOutputWidget, "Output audio file Waveform"));

        m_gui->waveformOutputWidget->graph(0)->setPen(QPen(Qt::red));

        m_gui->waveformOutputWidget->graph(0)->setData(x,y);
        m_gui->waveformOutputWidget->graph(0)->setName("Output waveform");

        m_gui->waveformOutputWidget->xAxis->setRange(0,input->frames());
        m_gui->waveformOutputWidget->yAxis->setRange(min,max);

        m_gui->waveformOutputWidget->xAxis->setLabel("Output waveform");

        m_gui->waveformOutputWidget->xAxis->setTickLabels(false);
        m_gui->waveformOutputWidget->yAxis->setTickLabels(false);

        m_gui->waveformOutputWidget->replot();

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
    default:
        m_gui->informationHostWatermark->setText("Warning: method not implemented yet");
        QMessageBox::information(m_gui->centralwidget,"Warning - method",
                                 "This method is not yet implemented!");
        break;
    }

}


/**
 * @brief LibWrapper::decode
 * Function triggered by clicking on the Decode button:
 * launch the decoding algorithm thanks to the desired
 * method and parameters.
 */
void LibWrapper::decode()
{
    if(m_inputName.isEmpty())
    {
        m_gui->informationHostWatermark->setText("Error: no Watermark host file defined!");
        QMessageBox::information(m_gui->centralwidget,"Warning - missing file",
                                 "Please, load a Watermark host file!");

        return;
    }

    switch(m_gui->selectingMethodTab->currentIndex())
    {
    case 0:
    {
        Parameters<short> conf;
        WatermarkManager<short> manager(conf);

        auto input = new FileInput<short>(m_inputName.toStdString(), conf);
        auto output = new DummyOutput<short>(conf);

        manager.data = m_data;
        manager.input.reset(input);
        manager.output.reset(output);
        manager.algorithm.reset(new LSBDecode<short>(conf));

        manager.execute();

        m_gui->informationHostWatermark->setText("LSB Method: File " + m_inputName +" successfully read!");

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
    default:
        m_gui->informationHostWatermark->setText("Warning: method not implemented yet");
        QMessageBox::information(m_gui->centralwidget,"Warning - method",
                                 "This method is not yet implemented!");
        break;
    }

    bitsToData();
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

/**
 * @brief LibWrapper::setLsbDefaultConfigurationValue
 * Function triggered by the set default config value
 * button (for LSB tab): resets configuration at default
 * values
 */
void LibWrapper::setLsbDefaultConfigurationValue()
{
    m_gui->sampleSizeSpinBox->setValue(512);
    m_gui->NumberLsbSpinBox->setValue(1);

}

/**
 * @brief LibWrapper::setSswDefaultConfigurationValue
 * Function triggered by the set default config value
 * button (for SSW tab): resets configuration at default
 * values
 */
void LibWrapper::setSswDefaultConfigurationValue()
{

}

/**
 * @brief LibWrapper::setCompExpDefaultConfigurationValue
 * Function triggered by the set default config value
 * button (for Compression-Expansion tab): resets
 * configuration at default values
 */
void LibWrapper::setCompExpDefaultConfigurationValue()
{

}
