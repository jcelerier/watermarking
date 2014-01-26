#ifndef QFFTSPINBOX_H
#define QFFTSPINBOX_H

#include <QSpinBox>

class QFFTSpinBox : public QSpinBox
{
		Q_OBJECT

	public:
		QFFTSpinBox(QWidget *parent = 0);
		virtual void stepBy(int steps);
};

#endif
