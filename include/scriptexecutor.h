#ifndef QSERIALTERMINAL_SCRIPTEXECUTOR_H
#define QSERIALTERMINAL_SCRIPTEXECUTOR_H

#include <iostream>
#include <string>
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
                 const std::function<void(MainWindow *, DelayType, int)> &printDelayResult);
private:
    std::shared_ptr<ScriptReader> m_scriptReader;
};


#endif //QSERIALTERMINAL_SCRIPTEXECUTOR_H
