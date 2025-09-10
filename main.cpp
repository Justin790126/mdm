#include <QtGui>
#include <iostream>
#include "ModelMdConverter.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModelMdConverter* md = new ModelMdConverter();
    md->setInputRoot("/home/justin126/workspace/mdm/doc");
    md->setOutputRoot("/home/justin126/workspace/mdm/html");
    md->start();

    while (!md->isFinished()) {
        QCoreApplication::processEvents();
        usleep(10000); // Sleep for 10 milliseconds
    }

    return a.exec();
}
