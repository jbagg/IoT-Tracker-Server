#ifndef DUALBOXLAYOUT_H
#define DUALBOXLAYOUT_H

#include <QBoxLayout>

class DualBoxLayout : public QBoxLayout
{
    Q_OBJECT

public:
	DualBoxLayout(QBoxLayout::Direction pdir = QBoxLayout::TopToBottom, QBoxLayout::Direction ldir = QBoxLayout::LeftToRight);
	void setLayoutDirections(QBoxLayout::Direction pdir, QBoxLayout::Direction ldir);

signals:
	void layoutDirectionChanged(QBoxLayout::Direction);

private:
	QBoxLayout::Direction portraitDirection, landscapeDirection;

private slots:
	void orientationChanged(Qt::ScreenOrientation so);
};

#endif // DUALBOXLAYOUT_H
