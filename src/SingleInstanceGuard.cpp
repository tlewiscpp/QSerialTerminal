#include "SingleInstanceGuard.h"

#include <QCryptographicHash>


namespace  {

QString generateKeyHash(const QString& key, const QString& salt)
{
    QByteArray data{};
    data.append(key.toUtf8());
    data.append(salt.toUtf8());
    data = QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex();
    return data;
}

} //namespace 


SingleInstanceGuard::SingleInstanceGuard( const QString& key ) : 
        m_key{key},
        m_memoryLockKey{generateKeyHash(key, "_memoryLockKey")},
        m_sharedMemoryKey{generateKeyHash(key, "_sharedMemoryKey")},
        m_sharedMemory{this->m_sharedMemoryKey},
        m_memoryLock{m_memoryLockKey, 1}
{
    this->m_memoryLock.acquire();
    {
        QSharedMemory nixOsFix{this->m_sharedMemoryKey};    // Fix for *nix: http://habrahabr.ru/post/173281/
        nixOsFix.attach();
    }
    this->m_memoryLock.release();
}


bool SingleInstanceGuard::isAnotherProcessRunning()
{
    if (this->m_sharedMemory.isAttached()) {
        return false;
    }

    this->m_memoryLock.acquire();
    bool isRunning{this->m_sharedMemory.attach()};
    if (isRunning) {
        this->m_sharedMemory.detach();
    }
    this->m_memoryLock.release();

    return isRunning;
}

bool SingleInstanceGuard::tryLockProcess()
{
#if defined(ANDROID)
    return true;
#endif //defined(ANDROID)
    if (this->isAnotherProcessRunning() ) {
        return false;
    }

    this->m_memoryLock.acquire();
    bool result{this->m_sharedMemory.create(sizeof(quint64))};
    this->m_memoryLock.release();
    if (!result) {
        this->releaseProcess();
        return false;
    }
    return true;
}

void SingleInstanceGuard::releaseProcess()
{
    this->m_memoryLock.acquire();
    if (this->m_sharedMemory.isAttached()) {
        this->m_sharedMemory.detach();
    }
    this->m_memoryLock.release();
}


SingleInstanceGuard::~SingleInstanceGuard()
{
#if !defined(ANDROID)
    this->releaseProcess();
#endif //!defined(ANDROID)
}
