#include <QThread>
#include <QDirIterator>
#include <vector>
#include <iostream>
#include "md2html.h"

using namespace std;

class MdFile
{
    public:
        MdFile(const string& path) : m_sFullPath(path) {}
        ~MdFile() = default;

        string getFullPath() const { return m_sFullPath; }

    private:
        string m_sFullPath;
};

class ModelMdConverter : public QThread
{
    Q_OBJECT

public:
    explicit ModelMdConverter(QObject *parent = 0);
    ~ModelMdConverter();
    void setInputRoot(const string &inputRoot) { m_sInputRoot = inputRoot; }
    void setOutputRoot(const string &outputRoot) { m_sOutputRoot = outputRoot; }
    void collectMarkdownFiles();
protected:
    void run(); // Override this to implement your threaded task

private:
    string m_sInputRoot;
    string m_sOutputRoot;

    vector<MdFile*> m_vMdFiles;
};