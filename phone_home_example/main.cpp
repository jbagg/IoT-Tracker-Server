#include <QCoreApplication>
#include "link.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	Link link;
	link.srvConnect();
}
