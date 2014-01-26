#include "settingsmanager.h"
#include <QFileDialog>
#include "libwrapper.h"
SettingsManager::SettingsManager(QObject *parent):
	QObject(parent)
{
}

void SettingsManager::save()
{
	// 1. Demander nom fichier sauvegarde
	QString fileOut = QFileDialog::getSaveFileName(nullptr, tr("Save configuration file (*.ini)"), "", "Settings (*.ini)");

	LibWrapper* p = dynamic_cast<LibWrapper*>(parent());

	// 2. Ecrire
	QSettings s(fileOut, QSettings::IniFormat);
	int method = p->m_gui->selectingMethodComboBox->currentIndex();
	s.setValue("Method", method);

	switch(method)
	{
		case 0: // LSB
			s.setValue("LSB/nbBits", p->m_gui->NumberLsbSpinBox->value());
			s.setValue("LSB/SubMethod", p->m_gui->lsbMethodUsedComboBox->currentIndex());
			break;

		case 1: // SSW
			s.setValue("SSW/PNSequence", readSSWLine(0));
			s.setValue("SSW/FreqRange", readSSWLine(1));
			s.setValue("SSW/seqSize", p->m_gui->sswSeqSize->value());
			s.setValue("SSW/bufferSize", p->m_gui->sswBufferSize->value());
			s.setValue("SSW/amplitude", p->m_gui->sswAmpl->value());
			s.setValue("SSW/threshold", p->m_gui->sswThres->value());
			break;

		case 2: // C-Exp
			break;

		default:
			break;
	}

	s.setValue("Time/Beginning", p->m_gui->watermarkBeginningTime->time());
	s.setValue("Time/End", p->m_gui->watermarkEndingTime->time());
}

void SettingsManager::load()
{

	// 1. Demander nom fichier sauvegarde
	QString fileOut = QFileDialog::getOpenFileName(nullptr, tr("Open configuration file (*.ini)"), "", "Settings (*.ini)");
	subLoad(fileOut);
}

void SettingsManager::subLoad(QString fileOut)
{
	LibWrapper* p = dynamic_cast<LibWrapper*>(parent());

	// 2. Lire
	QSettings s(fileOut, QSettings::IniFormat);
	int method = s.value("Method").toInt();
	p->m_gui->selectingMethodComboBox->setCurrentIndex(method);

	switch(method)
	{
		case 0: // LSB
			p->m_gui->NumberLsbSpinBox->setValue(s.value("LSB/nbBits").toInt());
			p->m_gui->lsbMethodUsedComboBox->setCurrentIndex(s.value("LSB/SubMethod").toInt());
			break;

		case 1: // SSW
			writeSSWLine(0, s.value("SSW/PNSequence").toList());
			writeSSWLine(1, s.value("SSW/FreqRange").toList());
			p->m_gui->sswSeqSize->setValue(s.value("SSW/seqSize").toInt());
			p->m_gui->sswBufferSize->setValue(s.value("SSW/bufferSize").toInt());
			p->m_gui->sswAmpl->setValue(s.value("SSW/amplitude").toDouble());
			p->m_gui->sswThres->setValue(s.value("SSW/threshold").toDouble());
			break;

		case 2: // C-Exp
			break;

		default:
			break;
	}

	p->m_gui->watermarkBeginningTime->setTime(s.value("Time/Beginning").toTime());
	p->m_gui->watermarkEndingTime->setTime(s.value("Time/End").toTime());
}

QList<QVariant> SettingsManager::readSSWLine(int line)
{
	QList<QVariant> out;
	LibWrapper* p = dynamic_cast<LibWrapper*>(parent());

	for(int i = 0; i < p->m_gui->sswSequences->columnCount(); i++)
		out.append(p->m_gui->sswSequences->item(line, i)->text().toInt());

	return out;

}

void SettingsManager::writeSSWLine(int line, QList<QVariant> l)
{
	LibWrapper* p = dynamic_cast<LibWrapper*>(parent());
	p->m_gui->sswSequences->setColumnCount(l.size());
	for(int i = 0; i < l.size(); i++)
	{
		p->m_gui->sswSequences->setItem(line, i, new QTableWidgetItem(QString::number(l[i].toInt())));
	}
}
