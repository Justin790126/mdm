#include "ModelMdConverter.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

// Returns true if successful, false otherwise
bool removeDirRecursively(const QString &dirPath) {
  QDir dir(dirPath);

  if (!dir.exists())
    return true; // Nothing to do

  QFileInfoList entries =
      dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
  foreach (QFileInfo entry, entries) {
    if (entry.isDir()) {
      // Recursive call for directories
      if (!removeDirRecursively(entry.absoluteFilePath()))
        return false;
    } else {
      // Remove files
      if (!QFile::remove(entry.absoluteFilePath()))
        return false;
    }
  }
  // Remove the now-empty directory
  return dir.rmdir(dirPath);
}

ModelMdConverter::ModelMdConverter(QObject *parent) : QThread(parent) {
  // Initialization code here
  int argc = 3;
  char *argv[3] = {"md2html", "--github", "--html-css=style.css"};
  if (initMdParser(argc, argv) != 0) {
    exit(1);
  }
  // process_string
}

ModelMdConverter::~ModelMdConverter() {
  for (MdFile *file : m_vMdFiles)
    delete file;
}

void ModelMdConverter::collectMarkdownFiles() {
  m_vMdFiles.clear();
  m_vMdFiles.reserve(128);
  QDirIterator it(QString::fromStdString(m_sInputRoot), QStringList() << "*.md",
                  QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString filePath = it.next();
    m_vMdFiles.push_back(new MdFile(filePath.toStdString()));
  }
  m_vMdFiles.shrink_to_fit();
}

vector<string> ModelMdConverter::ParseDstFolders() {
  vector<string> dstFolders;
  dstFolders.reserve(m_vMdFiles.size());
  for (size_t i = 0; i < m_vMdFiles.size(); i++) {
    string inputFullpath = m_vMdFiles[i]->getMdPath();

    string baseNameWithoutExtension = QFileInfo(QString::fromStdString(inputFullpath))
                                          .completeBaseName()
                                          .toStdString();
    string htmlBaseName = baseNameWithoutExtension + ".html";

    size_t pos = inputFullpath.find(m_sInputRoot);
    if (pos != std::string::npos) {
      inputFullpath.replace(pos, m_sInputRoot.length(), m_sOutputRoot);
      // cout << "Output file: " << inputFullpath << endl;
      size_t lastSlash = inputFullpath.find_last_of("/\\");
      if (lastSlash != std::string::npos) {
        std::string folderPath = inputFullpath.substr(0, lastSlash);
        // Use folderPath as needed

        dstFolders.push_back(folderPath);

        string htmlPath = folderPath + "/" + htmlBaseName;
        m_vMdFiles[i]->setHtmlPath(htmlPath);
      }
    }
  }
  return dstFolders;
}

void ModelMdConverter::CreateDstFolders(const vector<string> &dstFolders) {

  // remove all by m_sOutputRoot
  removeDirRecursively(QString::fromStdString(m_sOutputRoot));

  for (const string &folder : dstFolders) {
    QDir dir(QString::fromStdString(folder));
    if (!dir.exists()) {
      dir.mkpath(".");
    }
  }
}

string ModelMdConverter::ParseMdRawFile(const string &filePath, const string& rootFolder)
{
    ifstream inFile(filePath);
    if (!inFile) {
        cerr << "Unable to open file " << filePath << endl;
        return "";
    }

    string rawContent((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Convert markdown to HTML
    char* htmlContent = process_string(rawContent.c_str());
    if (htmlContent) {
        // Save or use the HTML content as needed
        string htmlStr(htmlContent);
        free(htmlContent); // Free the allocated memory
        return htmlStr;
    } else {
        cerr << "Markdown to HTML conversion failed for file " << filePath << endl;
        return "";
    }
}

void ModelMdConverter::run() {
  // Implement your threaded task here
  collectMarkdownFiles();

  vector<string> dstFolders = ParseDstFolders();

  CreateDstFolders(dstFolders);

  // iterate m_vMdFiles and convert to html
    for (size_t i = 0; i < m_vMdFiles.size(); i++) {
        string inputFullpath = m_vMdFiles[i]->getMdPath();
        
        string rawContent = ParseMdRawFile(inputFullpath, dstFolders[i]);


        m_vMdFiles[i]->setHtmlContent(rawContent);

        // Write HTML content to file
        cout << m_vMdFiles[i]->getHtmlPath() << endl;

        FILE *outFile = fopen(m_vMdFiles[i]->getHtmlPath().c_str(), "w");
        if (outFile) {
            fputs(m_vMdFiles[i]->getHtmlContent().c_str(), outFile);
            fclose(outFile);
        } else {
            cerr << "Unable to open output file " << m_vMdFiles[i]->getHtmlPath() << endl;
        }

    }

  
}