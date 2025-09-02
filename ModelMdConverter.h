#include <QThread>
#include "md2html.h"

class ModelMdConverter : public QThread
{
    Q_OBJECT

public:
    explicit ModelMdConverter(QObject *parent = 0);

protected:
    void run(); // Override this to implement your threaded task
};