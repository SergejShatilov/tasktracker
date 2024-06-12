#ifndef DIALOGEDITTASK_H
#define DIALOGEDITTASK_H

#include <QDialog>
#include <QDataWidgetMapper>
#include "httpclient/httpclient.h"
#include "models/employeesmodel.h"

namespace Ui {
class DialogEditTask;
}

class DialogEditTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditTask(HttpClient* httpClient,
                            bool isEdit = false,
                            QWidget *parent = nullptr);
    ~DialogEditTask();

    void setModel(QAbstractItemModel* model);
    void setCurrentModelIndex(const QModelIndex& index);

    void setEmployeesModel(EmployeesModel* model);
    void setExecutorFullName(const QString& fullName);
    void setTaskId(qint32 id);

    const Task& task() const;

    void setParentTask(qint32 parentId, const QString& parentName);

private slots:
    void changed(const QString &);
    void submit();
    void createNewTask();

private:
    Ui::DialogEditTask*     ui;
    HttpClient*             m_httpClient;
    QDataWidgetMapper*      m_mapper;
    Task                    m_task;
    EmployeesModel*         m_employeesModel;
};

#endif // DIALOGEDITTASK_H
