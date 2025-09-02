#include <QtGui>
#include <iostream>
#include "ModelMdConverter.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ModelMdConverter* md = new ModelMdConverter();
    md->setInputRoot("/home/justin126/workspace/mdm/doc");
    md->start();

    return a.exec();
}
