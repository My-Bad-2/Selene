/**
 * @file log.h
 * @brief Lightweight logging library with support for callbacks, streams, and colored output.
 */
#ifndef LOG_H
#define LOG_H 1

#include <klibc/stdio.h>
#include <stdbool.h>

/**
 * @brief Logs a trace-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a debug-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs an info-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_info(...) log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a warning-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_warn(...) log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs an error-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a fatal-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a panic-level message and halts the system.
 * @param ... Format string and arguments for the message.
 */
#define log_panic(...) log_log(LOG_PANIC, __FILE__, __LINE__, __VA_ARGS__)

__CDECLS_BEGIN

/**
 * @enum LogLevel
 * @brief Log levels indicating the severity of the log messages.
 */
enum {
    LOG_TRACE,///< Trace-level messages for detailed debugging.
    LOG_DEBUG,///< Debug-level messages for general debugging.
    LOG_INFO, ///< Informational messages.
    LOG_WARN, ///< Warning messages indicating potential issues.
    LOG_ERROR,///< Error messages indicating failures.
    LOG_FATAL,///< Fatal errors requiring immediate attention.
    LOG_PANIC,///< Panic-level messages indicating a critical system failure.
};

/**
 * @struct LogEvent
 * @brief Represents a log event containing metadata and formatted message details.
 */
struct LogEvent
{
    va_list ap;      ///< Variable argument list for formatting.
    const char *fmt; ///< Format string.
    const char *file;///< Source file name.
    FILE *stream;    ///< Output stream.
    int line;        ///< Line number in the source file.
    int level;       ///< Log level.
};

/**
 * @brief Callback function type for log events.
 * @param event Pointer to the LogEvent containing log message details.
 */
typedef void (*log_fn)(struct LogEvent *);

/**
 * @brief Enables or disables quiet mode for logging.
 * @param enable Set to true to enable quiet mode (suppress console output).
 */
void log_set_quiet(bool enable);

/**
 * @brief Sets the minimum log level for processing.
 * @param level Log level threshold. Events below this level will be ignored.
 */
void log_set_level(int level);

/**
 * @brief Adds a callback for handling log events.
 * @param func Callback function to process log events.
 * @param stream Stream to associate with the callback.
 * @param level Minimum log level to invoke the callback.
 * @return 0 on success, -1 if the callback limit is reached.
 */
int log_add_callback(log_fn func, FILE *stream, int level);

/**
 * @brief Adds a stream for log output.
 * @param stream Stream to write log messages to.
 * @param level Minimum log level for messages to be written to the stream.
 * @return 0 on success, -1 if the callback limit is reached.
 */
int log_add_stream(FILE *stream, int level);

/**
 * @brief Logs a message with the specified level and metadata.
 * @param level Log level of the message.
 * @param file Source file where the log message originated.
 * @param line Line number in the source file.
 * @param fmt Format string for the log message.
 * @param ... Variable arguments for the format string.
 */
void log_log(int level, const char *file, int line, const char *fmt, ...) __PRINTFLIKE(4, 5);

__CDECLS_END

#endif// LOG_H