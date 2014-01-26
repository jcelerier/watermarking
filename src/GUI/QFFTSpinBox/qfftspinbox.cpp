#include "qfftspinbox.h"
#include <iostream>
QFFTSpinBox::QFFTSpinBox(QWidget *parent) :
	QSpinBox(parent)
{
	this->setValue(512);
}


void QFFTSpinBox::stepBy(int steps)
{
	if(steps > 0)
		this->setValue(this->value()*2);
	else if(steps < 0 && this->value() >= 32)
		this->setValue(this->value()/2);
}
