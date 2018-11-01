#ifndef TRMIDOUBLECHECKBOX_SINGLEINSTANCEGUARD_H
#define TRMIDOUBLECHECKBOX_SINGLEINSTANCEGUARD_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class SingleInstanceGuard
{
public:
    SingleInstanceGuard(const QString& key);
    ~SingleInstanceGuard();

    bool isAnotherProcessRunning();
    bool tryLockProcess();
    void releaseProcess();

private:
    const QString m_key;
    const QString m_memoryLockKey;
    const QString m_sharedMemoryKey;

    QSharedMemory m_sharedMemory;
    QSystemSemaphore m_memoryLock;

    Q_DISABLE_COPY(SingleInstanceGuard)
};


#endif //TRMIDOUBLECHECKBOX_SINGLEINSTANCEGUARD_H
