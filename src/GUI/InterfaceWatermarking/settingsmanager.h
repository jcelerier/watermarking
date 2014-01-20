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

	public:
		QList<QVariant> readSSWLine(int line);
		void writeSSWLine(int line, QList<QVariant> l);
		void subLoad(QString fileOut);
};

#endif // SETTINGSMANAGER_H
