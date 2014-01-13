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
	QString fileOut = QFileDialog::getSaveFileName(nullptr, tr("Save configuration file (.ini)"));

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
			break;

		case 2: // C-Exp
			break;

		default:
			break;
	}
}

void SettingsManager::load()
{

	// 1. Demander nom fichier sauvegarde
	QString fileOut = QFileDialog::getOpenFileName(nullptr, tr("Open configuration file (.ini)"));

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
			break;

		case 2: // C-Exp
			break;

		default:
			break;
	}

}
