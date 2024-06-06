#ifndef DIALOGOPENDB_H
#define DIALOGOPENDB_H

#include <QDialog>

#include "httpclient/httpclient.h"

namespace Ui {
class DialogOpenDb;
}

class DialogOpenDb : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpenDb(std::shared_ptr<HttpClient> httpClient,
                          bool isNewDb = true,
                          QWidget *parent = nullptr);
    ~DialogOpenDb();

    void setDbName(const QString& dbname);
    QString dbName() const;

private slots:
    void createNewDb();
    void openDb();
    void changedName(const QString& name);

private:
    Ui::DialogOpenDb *ui;
    std::shared_ptr<HttpClient> m_httpClient;
};

#endif // DIALOGOPENDB_H
