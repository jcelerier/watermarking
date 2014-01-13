#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

class SettingsManager : public QObject
{
		Q_OBJECT
	public:
		explicit SettingsManager(QObject *parent = nullptr);

	signals:

	public slots:
		void save();
		void load();
};

#endif // SETTINGSMANAGER_H
