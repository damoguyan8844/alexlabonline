#ifndef SAMEGM_H
#define SAMEGM_H

#include <QtGui/QMainWindow>
#include "ui_samegm.h"

class SameGm : public QMainWindow
{
	Q_OBJECT

public:
	SameGm(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SameGm();

private:
	Ui::SameGmClass ui;
};

#endif // SAMEGM_H
