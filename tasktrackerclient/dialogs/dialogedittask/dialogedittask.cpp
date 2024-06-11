
#include "dialogedittask.h"
#include "ui_dialogedittask.h"

// =============================================================================
DialogEditTask::DialogEditTask(HttpClient* httpClient,
                               bool isEdit,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditTask),
    m_httpClient(httpClient),
    m_mapper(new QDataWidgetMapper(this)),
    m_task(Task())
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
    m_mapper->addMapping(ui->lineEditName, 1);
    m_mapper->addMapping(ui->textEditDescription, 7, "plainText");
    m_mapper->addMapping(ui->dateEditStart, 4);
    m_mapper->addMapping(ui->spinBoxDuration, 5);
}

// =============================================================================
void DialogEditTask::setCurrentModelIndex(const QModelIndex& index)
{
    if (m_mapper->model() == nullptr)
        return;

    QModelIndex idIndex = m_mapper->model()->index(index.row(), 0);

    m_task.setId(m_mapper->model()->data(idIndex).toInt());

    m_mapper->setCurrentModelIndex(index);
}

// =============================================================================
const Task& DialogEditTask::task() const {
    return m_task;
}

// =============================================================================
void DialogEditTask::setParentTask(qint32 parentId, const QString& parentName)
{
    m_task.setParentId(parentId);
    ui->labelParent->setText(parentName);
}

// =============================================================================
void DialogEditTask::changed(const QString &)
{
    ui->pushButtonOk->setEnabled(!ui->lineEditName->text().isEmpty());
}

// =============================================================================
void DialogEditTask::submit()
{
    m_task.setName(ui->lineEditName->text());
    m_task.setState(Task::State::NotStarted);
    m_task.setExecutorId(1);
    m_task.setStart(ui->dateEditStart->date());
    m_task.setDuration(ui->spinBoxDuration->value());
    m_task.setDescription(ui->textEditDescription->toPlainText());

    ui->pushButtonOk->setEnabled(false);
    if (!m_httpClient->changeTask(m_task.id(), m_task)) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    m_mapper->submit();
    accept();
}

// =============================================================================
void DialogEditTask::createNewTask()
{
    m_task.setName(ui->lineEditName->text());
    m_task.setState(Task::State::NotStarted);
    m_task.setExecutorId(1);
    m_task.setStart(ui->dateEditStart->date());
    m_task.setDuration(ui->spinBoxDuration->value());
    m_task.setDescription(ui->textEditDescription->toPlainText());

    ui->pushButtonOk->setEnabled(false);

    Task task = m_httpClient->addTask(m_task);

    if (!task.isValid()) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    m_task = task;

    accept();
}

// =============================================================================
