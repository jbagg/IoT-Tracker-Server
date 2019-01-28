#include <QApplication>
#include <QScreen>
#include "dualboxlayout.h"

DualBoxLayout::DualBoxLayout(Direction pdir, Direction ldir) : QBoxLayout(QBoxLayout::TopToBottom), portraitDirection(pdir), landscapeDirection(ldir)
{
	QScreen *s = qApp->primaryScreen();
	connect(s, &QScreen::orientationChanged, this, &DualBoxLayout::orientationChanged);
	s->setOrientationUpdateMask(Qt::LandscapeOrientation | Qt::InvertedLandscapeOrientation | Qt::PortraitOrientation | Qt::InvertedPortraitOrientation);
	orientationChanged(s->orientation());
}

void DualBoxLayout::setLayoutDirections(QBoxLayout::Direction pdir, QBoxLayout::Direction ldir)
{
	portraitDirection = pdir;
	landscapeDirection = ldir;
	orientationChanged(qApp->primaryScreen()->orientation());
}

void DualBoxLayout::orientationChanged(Qt::ScreenOrientation so)
{
	QBoxLayout::Direction newDirection;

	if (QApplication::primaryScreen()->isLandscape(so))
		newDirection = landscapeDirection;
	else
		newDirection = portraitDirection;

	if (direction() != newDirection) {
		setDirection(newDirection);
		layoutDirectionChanged(newDirection);
	}
}
