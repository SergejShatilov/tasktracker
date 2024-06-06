#ifndef DIALOGAUTHORIZATION_H
#define DIALOGAUTHORIZATION_H

#include <QDialog>

namespace Ui {
class DialogAuthorization;
}

class DialogAuthorization : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAuthorization(QWidget *parent = nullptr);
    ~DialogAuthorization();

    QString userName() const;
    QString password() const;

private:
    Ui::DialogAuthorization *ui;
};

#endif // DIALOGAUTHORIZATION_H
