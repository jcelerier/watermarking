#include "qfftspinbox.h"
#include "qfftspinboxplugin.h"

#include <QtPlugin>

QFFTSpinboxPlugin::QFFTSpinboxPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void QFFTSpinboxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool QFFTSpinboxPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *QFFTSpinboxPlugin::createWidget(QWidget *parent)
{
	return new QFFTSpinBox(parent);
}

QString QFFTSpinboxPlugin::name() const
{
	return QLatin1String("QFFTSpinBox");
}

QString QFFTSpinboxPlugin::group() const
{
	return QLatin1String("");
}

QIcon QFFTSpinboxPlugin::icon() const
{
	return QIcon();
}

QString QFFTSpinboxPlugin::toolTip() const
{
	return QLatin1String("");
}

QString QFFTSpinboxPlugin::whatsThis() const
{
	return QLatin1String("FFT Spinbox");
}

bool QFFTSpinboxPlugin::isContainer() const
{
	return false;
}

QString QFFTSpinboxPlugin::domXml() const
{
	return QLatin1String("<widget class=\"QFFTSpinBox\" name=\"qFFTSpinBox\">\n</widget>\n");
}

QString QFFTSpinboxPlugin::includeFile() const
{
	return QLatin1String("../QFFTSpinBox/qfftspinbox.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qfftspinboxplugin, QFFTSpinboxPlugin)
#endif // QT_VERSION < 0x050000
