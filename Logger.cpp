#include <bits/stdc++.h>
using namespace std;

/* =========================================================== */
/* ================== Logging System Design ================== */
/* =========================================================== */

enum LogLevel
{
    INFO,
    ERROR,
    DEBUG
};

class Logger {
    Logger *nextLogger = NULL;

public:
    Logger(Logger *nextLogger) {
        this->nextLogger = nextLogger;
    }
    
    virtual void log(LogLevel level, string msg) {
        if (nextLogger != NULL)
            nextLogger->log(level, msg);
    }
};

class InfoLogger: public Logger {
public:
    InfoLogger(Logger *nextLogger): Logger(nextLogger) {};
    void log(LogLevel level, string msg) {
        if (level == LogLevel::INFO) {
            cout << "INFO: " << msg << endl;
        } else {
            Logger::log(level, msg);
        }
    }
};

class DebugLogger: public Logger {
public:
    DebugLogger(Logger *nextLogger): Logger(nextLogger) {};
    void log(LogLevel level, string msg) {
        if (level == LogLevel::DEBUG) {
            cout << "DEBUG: " << msg << endl;
        } else {
            Logger::log(level, msg);
        }
    }
};

class ErrorLogger: public Logger {
public:
    ErrorLogger(Logger *nextLogger): Logger(nextLogger) {};
    void log(LogLevel level, string msg) {
        if (level == LogLevel::ERROR) {
            cout << "ERROR: " << msg << endl;
        } else {
            Logger::log(level, msg);
        }
    }
};

/* Driver function */
int main()
{
    Logger *logger = new InfoLogger(new DebugLogger(new ErrorLogger(NULL)));
    
    logger->log(LogLevel::ERROR, "something went wrong!");
    logger->log(LogLevel::INFO, "new day, new challenges");
    logger->log(LogLevel::DEBUG, "debugging is fun if you understand the code.");

    return 0;
}
