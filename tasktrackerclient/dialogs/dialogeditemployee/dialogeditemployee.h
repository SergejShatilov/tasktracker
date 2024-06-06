#ifndef DIALOGEDITEMPLOYEE_H
#define DIALOGEDITEMPLOYEE_H

#include <QDialog>

#include "httpclient/httpclient.h"

namespace Ui {
class DialogEditEmployee;
}

class DialogEditEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditEmployee(std::shared_ptr<HttpClient> httpClient,
                                QWidget *parent = nullptr);
    ~DialogEditEmployee();

    void setLastName(const QString& lastName);
    void setFirstName(const QString& firstName);
    void setPatronymic(const QString& patronymic);
    void setPosition(const QString& position);
    void setEmail(const QString& email);
    void setPhone(const QString& phone);

    const Employee& employee() const;

private slots:
    void changed(const QString &);
    void createNewEmployee();

private:
    Ui::DialogEditEmployee *ui;
    std::shared_ptr<HttpClient> m_httpClient;
    Employee m_employee;
};

#endif // DIALOGEDITEMPLOYEE_H
