
#include "dialogedittask.h"
#include "ui_dialogedittask.h"

// =============================================================================
DialogEditTask::DialogEditTask(std::shared_ptr<HttpClient> httpClient,
                               bool isEdit,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditTask),
    m_httpClient(httpClient),
    m_mapper(new QDataWidgetMapper(this))
{
    ui->setupUi(this);

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    connect(ui->lineEditName, &QLineEdit::textChanged,
            this, &DialogEditTask::changed);

    if (isEdit) {
        connect(ui->pushButtonOk, &QPushButton::clicked,
                this, &DialogEditTask::submit);
    } else {
        connect(ui->pushButtonOk, &QPushButton::clicked,
                this, &DialogEditTask::createNewTask);
    }
}

// =============================================================================
DialogEditTask::~DialogEditTask()
{
    delete ui;
}

// =============================================================================
void DialogEditTask::setModel(QAbstractItemModel* model)
{
    m_mapper->setSubmitPolicy(QDataWidgetMapper::SubmitPolicy::ManualSubmit);
    m_mapper->setModel(model);
}

// =============================================================================
void DialogEditTask::setCurrentModelIndex(const QModelIndex& index) {
    m_mapper->setCurrentModelIndex(index);
}

// =============================================================================
const Task& DialogEditTask::task() const {
    return m_task;
}

// =============================================================================
void DialogEditTask::changed(const QString &)
{
    ui->pushButtonOk->setEnabled(!ui->lineEditName->text().isEmpty());
}

// =============================================================================
void DialogEditTask::submit()
{

}

// =============================================================================
void DialogEditTask::createNewTask()
{
    m_task.setName(ui->lineEditName->text());
    m_task.setState(Task::State::NotStarted);
    m_task.setExecutor(1);
    m_task.setStart(ui->dateEditStart->date());
    m_task.setDuration(ui->spinBoxDuration->value());
    m_task.setParent(1);
    m_task.setDescription(ui->textEditDescription->toPlainText());

    ui->pushButtonOk->setEnabled(false);
    if (!m_httpClient->addTask(m_task)) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    accept();
}

// =============================================================================
