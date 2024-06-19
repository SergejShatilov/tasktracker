
#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <QDialog>

#include "db/dbremotemanager.h"

namespace Ui {
    class EmployeeDialog;
}

class EmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeDialog(DbRemoteManager* dbManager,
                            QWidget *parent = nullptr,
                            Employee* employee = nullptr);
    ~EmployeeDialog();

    const Employee* employee() const;

private slots:
    void clickedOk();
    void changed(const QString& str);

private:
    Ui::EmployeeDialog* ui;
    DbRemoteManager*    m_dbManager;
    Employee*           m_employee;

    std::function<bool(Employee* employee)> m_funcHandler;
};

#endif // EMPLOYEEDIALOG_H
