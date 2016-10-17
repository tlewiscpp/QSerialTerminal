#ifndef QSERIALTERMINAL_SCRIPTEXECUTOR_H
#define QSERIALTERMINAL_SCRIPTEXECUTOR_H

#include <QApplication>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <functional>
#include <cstdlib>
#include <serialport.h>
#include <generalutilities.h>
#include "mainwindow.h"
#include "qserialterminalstrings.h"
#include "scriptreader.h"

class MainWindow;

class ScriptExecutor
{
public:
    ScriptExecutor(const std::string &scriptFilePath);
    void setScriptFilePath(const std::string &scriptFilePath);
    std::shared_ptr<ScriptReader> scriptReader() const;
    void execute(MainWindow *mainWindow, std::shared_ptr<SerialPort> serialPort,
                 const std::function<void(MainWindow *, const std::string &)> &printRxResult,
                 const std::function<void(MainWindow *, const std::string &)> &printTxResult,
                 const std::function<void(MainWindow *, DelayType, int)> &printDelayResult,
                 const std::function<void(MainWindow *, FlushType)> &printFlushResult,
                 const std::function<void(MainWindow *, LoopType, int, int)> &printLoopResult);
private:
    std::shared_ptr<ScriptReader> m_scriptReader;

    void doLoop(MainWindow *mainWindow, std::shared_ptr<SerialPort> serialPort,
                 const std::function<void(MainWindow *, const std::__cxx11::string &)> &printRxResult,
                 const std::function<void(MainWindow *, const std::__cxx11::string &)> &printTxResult,
                 const std::function<void(MainWindow *, DelayType, int)> &printDelayResult,
                 const std::function<void(MainWindow *, FlushType)> &printFlushResult,
                 const std::function<void(MainWindow *, LoopType, int, int)> &printLoopResult,
                 const std::vector<SerialCommand> &commands,
                 int loopCount);

    void delaySecondsWithUpdate(int howLong, QApplication *update);
    void delayMillisecondsWithUpdate(int howLong, QApplication *update);
    void delayMicrosecondsWithUpdate(int howLong, QApplication *update);
};




#endif //QSERIALTERMINAL_SCRIPTEXECUTOR_H
