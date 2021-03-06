#ifndef QSERIALTERMINAL_GLOBALDEFINITIONS_H
#define QSERIALTERMINAL_GLOBALDEFINITIONS_H

#include <QDebug>
#include <QString>
#include <QtGlobal>

#ifndef LOG_DEBUG
#    define LOG_DEBUG(x) qDebug(x)
#endif
#ifndef LOG_INFO
#    define LOG_INFO(x) qInfo(x)
#endif
#ifndef LOG_WARN
#    define LOG_WARN(x) qWarning(x)
#endif
#ifndef LOG_CRITICAL
#    define LOG_CRITICAL(x) qCritical(x)
#endif
#ifndef LOG_FATAL
#    define LOG_FATAL(x) qFatal(x)
#endif

#ifndef QS_NUMBER
#    define QS_NUMBER(x) QString::number(x)
#endif

#ifndef STRING_TO_INT
#    if defined(__ANDROID__)
#        define STRING_TO_INT(x) std::atoi(x)
#    else
#        define STRING_TO_INT(x) std::stoi(x)
#    endif
#endif


#endif // QSERIALTERMINAL_GLOBALDEFINITIONS_H
