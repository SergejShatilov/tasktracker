#ifndef DIALOGEDITEMPLOYEE_H
#define DIALOGEDITEMPLOYEE_H

#include <QDialog>
#include <QDataWidgetMapper>

#include "httpclient/httpclient.h"

namespace Ui {
class DialogEditEmployee;
}

class DialogEditEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditEmployee(HttpClient* httpClient,
                                bool isEdit = false,
                                QWidget *parent = nullptr);
    ~DialogEditEmployee();

    void setModel(QAbstractItemModel* model);
    void setCurrentModelIndex(const QModelIndex& index);

    void setLastName(const QString& lastName);
    void setFirstName(const QString& firstName);
    void setPatronymic(const QString& patronymic);
    void setPosition(const QString& position);
    void setEmail(const QString& email);
    void setPhone(const QString& phone);

    const Employee& employee() const;

private slots:
    void changed(const QString &);
    void submit();
    void createNewEmployee();

private:
    Ui::DialogEditEmployee* ui;
    HttpClient*             m_httpClient;
    QDataWidgetMapper*      m_mapper;
    Employee                m_employee;
};

#endif // DIALOGEDITEMPLOYEE_H
