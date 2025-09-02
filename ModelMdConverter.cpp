#include "ModelMdConverter.h"

ModelMdConverter::ModelMdConverter(QObject *parent)
    : QThread(parent)
{
    // Initialization code here
    int argc = 3;
    char *argv[3] = {"md2html", "--github", "--html-css=style.css"};
    if (initMdParser(argc, argv) != 0)
    {
        exit(1);
    }
    // process_string
}

void ModelMdConverter::run()
{
    // Implement your threaded task here
}