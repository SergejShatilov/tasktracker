
#include "taskdialog.h"
#include "ui_taskdialog.h"

// =============================================================================
TaskDialog::TaskDialog(DbRemoteManager* dbManager,
                       QWidget *parent,
                       Task* task) :
    QDialog(parent),
    ui(new Ui::TaskDialog),
    m_dbManager(dbManager),
    m_task(task)
{
    ui->setupUi(this);
    ui->pushButtonOk->setEnabled(false);

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    connect(ui->pushButtonOk, &QPushButton::clicked,
            this, &TaskDialog::clickedOk);

    connect(ui->lineEditName, &QLineEdit::textChanged,
            this, &TaskDialog::changed);

    // Загружаем список сотрудников
    ui->comboBoxExecutor->addItem(tr("<Not Selected>"), 0);
    for (auto employee : dbManager->employees())
        ui->comboBoxExecutor->addItem(employee->fullName(), employee->id());

    // Если объект не существует, значит создаем новую задачу,
    // Иначе редактируем уже существующую
    if (m_task == nullptr)
    {
        setWindowTitle(tr("Create new task..."));
        ui->dateEditDeadline->setDate(QDate::currentDate());
        m_task = new Task(this->parent());
        m_funcHandler = [this](Task* task) {
            return m_dbManager->addTask(task);
        };
    }
    else
    {
        setWindowTitle(tr("Edit task..."));
        ui->lineEditName->setText(m_task->name());
        ui->textEditDescription->setText(m_task->description());
        ui->dateEditDeadline->setDate(m_task->deadline());

        int currentIndex = 0;

        if (m_task->executor() != nullptr)
        {
            auto executor = static_cast<Employee*>(m_task->executor());
            for (int i = 0; i < dbManager->employees().size(); ++i)
            {
                if (dbManager->employees().at(i) == executor)
                    currentIndex = (i + 1);
            }
        }

        ui->comboBoxExecutor->setCurrentIndex(currentIndex);

        m_funcHandler = [this](Task* task) {
            return m_dbManager->changeTask(task);
        };
    }
}

// =============================================================================
TaskDialog::~TaskDialog()
{
    delete ui;
}

// =============================================================================
const Task* TaskDialog::task() const {
    return m_task;
}

// =============================================================================
void TaskDialog::setTaskParent(Task* taskParent)
{
    if (taskParent != nullptr)
    {
        m_task->setParent(taskParent);
        m_task->setParentId(taskParent->id());
    }
}

// =============================================================================
void TaskDialog::clickedOk()
{
    if (m_funcHandler == nullptr)
        return;

    m_task->setName(ui->lineEditName->text());
    m_task->setState(Task::State::NotStarted);
    m_task->setDeadline(ui->dateEditDeadline->date());
    m_task->setDescription(ui->textEditDescription->toPlainText());

    // Если исполнитель не выбран
    if (ui->comboBoxExecutor->currentData().toInt() == 0)
    {
        // Если у задачи был исполнитель,
        // то у этого исполнителя удаляем эту задачу
        if (m_task->executor() != nullptr)
        {
            auto executor = static_cast<Employee*>(m_task->executor());
            executor->removeTask(m_task);
        }

        m_task->setExecutorId(0);
        m_task->setExecutor(nullptr);
    }
    else
    {
        auto executor = m_dbManager->employees().at(
            ui->comboBoxExecutor->currentIndex() - 1
        );

        // Если у задачи до этого был исполнитель,
        // то у него удаляем эту задачу
        if (m_task->executor() != nullptr)
        {
            auto executor = static_cast<Employee*>(m_task->executor());
            executor->removeTask(m_task);
        }

        m_task->setExecutorId(executor->id());
        m_task->setExecutor(executor);
        executor->addTask(m_task);
    }

    ui->pushButtonOk->setEnabled(false);

    if (m_funcHandler(m_task))
    {
        accept();
        return;
    }

    ui->pushButtonOk->setEnabled(true);
}

// =============================================================================
void TaskDialog::changed(const QString& str) {
    ui->pushButtonOk->setEnabled(!str.isEmpty());
}

// =============================================================================
