#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings* settings,
                            QWidget *parent = nullptr);
    ~SettingsDialog();

private:
    void pressOk();

private:
    Ui::SettingsDialog* ui;
    Settings*           m_settings;
};

#endif // SETTINGSDIALOG_H
