#include <QThread>
#include "md2html.h"

class MdFile
{
    public:
        MdFile(const string& path) : m_sFullPath(path) {}
        ~MdFile() = default;

    private:
        string m_sFullPath;
};

class ModelMdConverter : public QThread
{
    Q_OBJECT

public:
    explicit ModelMdConverter(QObject *parent = 0);
    ~ModelMdConverter() = default;
    void setInputRoot(const string &inputRoot) { m_sInputRoot = inputRoot; }
    void setOutputRoot(const string &outputRoot) { m_sOutputRoot = outputRoot; }

protected:
    void run(); // Override this to implement your threaded task

private:
    string m_sInputRoot;
    string m_sOutputRoot;
};