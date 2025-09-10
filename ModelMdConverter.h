#include <QThread>
#include <QDirIterator>
#include <vector>
#include <iostream>
#include <fstream>
#include "md2html.h"

using namespace std;

class MdFile
{
    public:
        MdFile(const string& path) : m_sMdPath(path) {}
        ~MdFile() = default;

        string getMdPath() const { return m_sMdPath; }
        void setHtmlContent(const string& html) { m_sHtmlContent = std::move(html); }
        string getHtmlContent() const { return m_sHtmlContent; }
        void setHtmlPath(const string& path) { m_sHtmlPath = path; }
        string getHtmlPath() const { return m_sHtmlPath; }

    private:
        string m_sMdPath;
        string m_sHtmlPath;

        string m_sHtmlContent;
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

private:
    vector<string> ParseDstFolders();

    void CreateDstFolders(const vector<string> &dstFolders);

    string ParseMdRawFile(const string &filePath, const string& rootFolder);
};