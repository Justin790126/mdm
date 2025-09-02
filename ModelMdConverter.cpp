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

ModelMdConverter::~ModelMdConverter()
{
    for (MdFile* file : m_vMdFiles)
        delete file;
}

void ModelMdConverter::collectMarkdownFiles()
{
    m_vMdFiles.clear();
    QDirIterator it(QString::fromStdString(m_sInputRoot),
                    QStringList() << "*.md",
                    QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        m_vMdFiles.push_back(new MdFile(filePath.toStdString()));
    }
}


void ModelMdConverter::run()
{
    // Implement your threaded task here
    collectMarkdownFiles();
    for (size_t i = 0 ; i < m_vMdFiles.size(); i++) {
        cout << "Processing file: " << m_vMdFiles[i]->getFullPath() << endl;
    }
}