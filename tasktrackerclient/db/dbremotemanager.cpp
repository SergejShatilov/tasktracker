
#include "dbremotemanager.h"

#include <QMessageBox>

#define DBREMOTEMANAGER_TIMEOUT_MS  (2000)

// =============================================================================
DbRemoteManager::DbRemoteManager(QObject* parent) :
    HttpClient(parent),
    m_dbName("undefined")
{
    connect(this, &HttpClient::messageError,
            this, &DbRemoteManager::showError, Qt::DirectConnection);
}

// =============================================================================
void DbRemoteManager::setDbName(const QString& dbName) {
    m_dbName = dbName;
}

const QString& DbRemoteManager::dbName() const {
    return m_dbName;
}

// =============================================================================
bool DbRemoteManager::createDb(const QString &dbName)
{
    QJsonObject jObj;
    jObj["dbName"] = dbName;

    return makeRequest(
        Method::Post,
        "/",
        jObj
    );
    return false;
}

// =============================================================================
bool DbRemoteManager::checkExistDb(const QString &dbName)
{
    return makeRequest(
        Method::Get,
        QString("/%1").arg(dbName),
        QByteArray(),
        nullptr,
        DBREMOTEMANAGER_TIMEOUT_MS
    );
}

// =============================================================================
bool DbRemoteManager::addEmployee(Employee* employee)
{
    QByteArray bufRecv;
    bool res = makeRequest(
        Method::Post,
        QString("/%1/employees").arg(m_dbName),
        employee->toJson(),
        &bufRecv,
        DBREMOTEMANAGER_TIMEOUT_MS
    );

    if (res && !bufRecv.isEmpty())
    {
        employee->fromJson(bufRecv);
        emit addedEmployee(employee);
        m_listEmployees.append(employee);
    }

    return res;
}

// =============================================================================
bool DbRemoteManager::changeEmployee(const Employee* employee)
{
    return makeRequest(
        Method::Put,
        QString("/%1/employees/%2").arg(m_dbName).arg(employee->id()),
        employee->toJson(),
        nullptr,
        DBREMOTEMANAGER_TIMEOUT_MS
    );
}

// =============================================================================
bool DbRemoteManager::removeEmployee(Employee* employee)
{
    bool res = makeRequest(
        Method::Delete,
        QString("/%1/employees/%2").arg(m_dbName).arg(employee->id()),
        QByteArray(),
        nullptr,
        DBREMOTEMANAGER_TIMEOUT_MS
    );

    if (!res)
        return res;

    // Всем задачам этого сотрудника сбросить исполнителя
    for (auto obj : employee->tasks())
    {
        auto task = static_cast<Task*>(obj);
        task->setExecutorId(0);
        if (!changeTask(task))
            return false;
    }

    m_listEmployees.removeOne(employee);
    emit removedEmployee(employee);

    return res;
}

// =============================================================================
bool DbRemoteManager::addTask(Task* task)
{
    QByteArray bufRecv;
    bool res = makeRequest(
        Method::Post,
        QString("/%1/tasks").arg(m_dbName),
        task->toJson(),
        &bufRecv,
        DBREMOTEMANAGER_TIMEOUT_MS
    );

    if (res && !bufRecv.isEmpty())
    {
        task->fromJson(bufRecv);
        emit addedTask(task);
        m_listTasks.append(task);
    }

    return res;
}

// =============================================================================
bool DbRemoteManager::changeTask(const Task* task)
{
    return makeRequest(
        Method::Put,
        QString("/%1/tasks/%2").arg(m_dbName).arg(task->id()),
        task->toJson(),
        nullptr,
        DBREMOTEMANAGER_TIMEOUT_MS
    );
}

// =============================================================================
bool DbRemoteManager::removeTask(Task* task, bool isExtract)
{
    bool res = makeRequest(
        Method::Delete,
        QString("/%1/tasks/%2").arg(m_dbName).arg(task->id()),
        QByteArray(),
        nullptr,
        DBREMOTEMANAGER_TIMEOUT_MS
    );

    if (!res)
        return res;

    m_listTasks.removeOne(task);

    // У сотрудника, который был ответственный за
    // эту задачу, теперь ее не будет
    if (task->executor() != nullptr)
    {
        auto executor = static_cast<Employee*>(task->executor());
        executor->removeTask(task);
    }

    // Если необходимо извлечь все подзадачи из удаляемой суммарной задачи
    // То меняем им id родителя
    if (isExtract)
    {
        // По умолчанию id родительской задачи будет 0,
        // Если у удаляемой задачи была родительская задача,
        // то эта задача станет родительской для всех дочерних задач
        // удаляемой задачи
        qint32 parentId = 0;
        if (task->parentId() != 0)
        {
            auto taskParent = static_cast<Task*>(task->parent());
            parentId = taskParent->id();
        }

        // Для всех дочерних задач назначаем id родителя и
        // фиксируем на сервере
        for (auto child : task->children())
        {
            auto taskChild = static_cast<Task*>(child);
            taskChild->setParentId(parentId);
            if (!changeTask(taskChild))
                return false;
        }
    }
    else
    {
        // Функция, которая удалит рекурсивно подзадачи
        std::function<bool(Task* task)> funcRemoveTask;
        funcRemoveTask = [this, &funcRemoveTask](Task* task)
        {
            for (auto child : task->children())
            {
                auto childTask = static_cast<Task*>(child);

                // Если существуют подзадачи, то сначала удаляем их
                if (!child->children().isEmpty())
                {
                    if (!funcRemoveTask(static_cast<Task*>(child)))
                        return false;
                }

                // Удаляем эту задачу у ее исполнителя
                if (childTask->executor() != nullptr)
                {
                    auto executor = static_cast<Employee*>(
                        childTask->executor()
                    );
                    executor->removeTask(task);
                }

                bool res = makeRequest(
                    Method::Delete,
                    QString("/%1/tasks/%2")
                            .arg(m_dbName)
                            .arg(childTask->id()),
                    QByteArray(),
                    nullptr,
                    DBREMOTEMANAGER_TIMEOUT_MS
                );

                if (!res)
                    return false;
            }

            return true;
        };

        res = funcRemoveTask(task);
    }

    emit removedTask(task, isExtract);

    return res;
}

// =============================================================================
void DbRemoteManager::update()
{
    if (!getEmployees())
        return;

    QList<Task*> listTasksTop;

    if (!getTasks(listTasksTop))
        return;

    // Функция поиска сотрудника по его id
    auto funcEmployeeById = [this](qint32 id) -> QObject*
    {
        for (auto employee : m_listEmployees)
        {
            if (employee->id() == id)
                return employee;
        }

        return nullptr;
    };

    // Пробегаемся по вссем задачам и назначаем им
    // объекты-исполнителей по id исполнителя
    for (auto task : m_listTasks)
    {
        if (task->executorId() == 0)
            continue;

        auto executor = static_cast<Employee*>(
            funcEmployeeById(task->executorId())
        );

        if (executor == nullptr)
            continue;

        task->setExecutor(executor);
        executor->addTask(task);
    }

    emit updateEmployees(m_listEmployees);
    emit updateTasks(listTasksTop);
}

// =============================================================================
const QList<Employee*> DbRemoteManager::employees() const {
    return m_listEmployees;
}

// =============================================================================
void DbRemoteManager::showError(const QString& error)
{
    QMessageBox message(
        QMessageBox::Critical,
        tr("Error"),
        error,
        QMessageBox::Ok
    );
    message.exec();
}

// =============================================================================
bool DbRemoteManager::getEmployees()
{
    QByteArray bufRecv;
    bool res = makeRequest(
        Method::Get,
        QString("/%1/employees").arg(m_dbName),
        QByteArray(),
        &bufRecv,
        DBREMOTEMANAGER_TIMEOUT_MS
    );

    if (!res)
        return res;

    m_listEmployees = Employee::createListFromJson(bufRecv, this);

    return res;
}

// =============================================================================
bool DbRemoteManager::getTasks(QList<Task*>& listTasksTop)
{
    QByteArray bufRecv;
    bool res = makeRequest(
        Method::Get,
        QString("/%1/tasks").arg(m_dbName),
        QByteArray(),
        &bufRecv,
        DBREMOTEMANAGER_TIMEOUT_MS
    );

    if (!res)
        return res;

    m_listTasks = Task::createListFromJson(bufRecv, this);

    // Функция поиска задачи по ее id
    auto funcFindTaskById = [this](qint32 id) -> QObject*
    {
        for (auto task : m_listTasks)
        {
            if (task->id() == id)
                return task;
        }

        return nullptr;
    };

    // Делаем связывания родительской и дочерних задач
    for (auto task : m_listTasks)
    {
        // Если первого уровня
        if (task->parentId() == 0)
        {
            listTasksTop.append(task);
            continue;
        }

        // Ищем родителя этой задачи, если не найдем,
        // то осталяем в первом уровне
        auto taskParent = funcFindTaskById(task->parentId());
        if (taskParent == nullptr)
        {
            listTasksTop.append(task);
            continue;
        }

        // Добавляем родителя
        task->setParent(taskParent);
    }

    return res;
}

// =============================================================================

