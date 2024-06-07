#ifndef DIALOGEDITTASK_H
#define DIALOGEDITTASK_H

#include <QDialog>
#include <QDataWidgetMapper>
#include "httpclient/httpclient.h"

namespace Ui {
class DialogEditTask;
}

class DialogEditTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditTask(std::shared_ptr<HttpClient> httpClient,
                            bool isEdit = false,
                            QWidget *parent = nullptr);
    ~DialogEditTask();

    void setModel(QAbstractItemModel* model);
    void setCurrentModelIndex(const QModelIndex& index);

    const Task& task() const;

private slots:
    void changed(const QString &);
    void submit();
    void createNewTask();

private:
    Ui::DialogEditTask *ui;
    std::shared_ptr<HttpClient> m_httpClient;
    QDataWidgetMapper* m_mapper;
    Task m_task;
};

#endif // DIALOGEDITTASK_H