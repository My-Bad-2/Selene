/**
 * @file log.h
 *
 * @brief Provides a logging system for different severity levels with callback and stream support.
 *
 * This file defines a logging system that allows messages of varying severity levels to be logged
 * and output to different streams or processed by callback functions. The logging system supports
 * different log levels, such as trace, debug, info, warning, error, fatal, and panic, which
 * correspond to the severity of the message being logged.
 *
 * Key features:
 * - Various log levels (`LOG_TRACE`, `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`, `LOG_FATAL`,
 * `LOG_PANIC`).
 * - Macro functions for easy logging at different levels.
 * - Support for setting quiet mode and log level thresholds.
 * - Ability to add callback functions for log processing and output to multiple streams.
 * - The log system uses `va_list` for formatting messages with variable arguments.
 *
 * The logging system is designed to be flexible, allowing for both console-based output and more
 * complex logging mechanisms, such as logging to files or over the network (in-future).
 *
 * @note The log system supports varying verbosity based on the configured log level and allows for
 * fine-grained control over the log output.
 */
#ifndef LOG_H
#define LOG_H 1

#include <klibc/stdio.h>

/**
 * @brief Logs a trace-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_trace(...) log::log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a debug-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_debug(...) log::log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs an info-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_info(...) log::log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a warning-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_warn(...) log::log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs an error-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_error(...) log::log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a fatal-level message.
 * @param ... Format string and arguments for the message.
 */
#define log_fatal(...) log::log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Logs a panic-level message and halts the system.
 * @param ... Format string and arguments for the message.
 */
#define log_panic(...) log::log(LOG_PANIC, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @enum log_level
 * @brief Log levels indicating the severity of the log messages.
 */
enum LogLevel : uint8_t {
  LOG_TRACE,  ///< Trace-level messages for detailed debugging.
  LOG_DEBUG,  ///< Debug-level messages for general debugging.
  LOG_INFO,   ///< Informational messages.
  LOG_WARN,   ///< Warning messages indicating potential issues.
  LOG_ERROR,  ///< Error messages indicating failures.
  LOG_FATAL,  ///< Fatal errors requiring immediate attention.
  LOG_PANIC,  ///< Panic-level messages indicating a critical system failure.
};

/**
 * @struct log_event
 * @brief Represents a log event containing metadata and formatted message details.
 */
struct LogEvent {
  va_list ap;        ///< Variable argument list for formatting.
  const char *fmt;   ///< Format string.
  const char *file;  ///< Source file name.
  FILE *stream;      ///< Output stream.
  int line;          ///< Line number in the source file.
  int level;         ///< Log level.
};

/**
 * @brief Callback function type for log events.
 * @param event Pointer to the log_event containing log message details.
 */
using log_fn = void (*)(LogEvent *);

namespace log {
/**
 * @brief Enables or disables quiet mode for logging.
 * @param enable Set to true to enable quiet mode (suppress console output).
 */
void set_quiet(bool enable);

/**
 * @brief Sets the minimum log level for processing.
 * @param level Log level threshold. Events below this level will be ignored.
 */
void set_level(int level);

/**
 * @brief Adds a callback for handling log events.
 * @param func Callback function to process log events.
 * @param stream Stream to associate with the callback.
 * @param level Minimum log level to invoke the callback.
 * @return 0 on success, -1 if the callback limit is reached.
 */
int add_callback(log_fn func, FILE *stream, int level);

/**
 * @brief Adds a stream for log output.
 * @param stream Stream to write log messages to.
 * @param level Minimum log level for messages to be written to the stream.
 * @return 0 on success, -1 if the callback limit is reached.
 */
int add_stream(FILE *stream, int level);

/**
 * @brief Logs a message with the specified level and metadata.
 * @param level Log level of the message.
 * @param file Source file where the log message originated.
 * @param line Line number in the source file.
 * @param fmt Format string for the log message.
 * @param ... Variable arguments for the format string.
 */
void log(int level, const char *file, int line, const char *fmt, ...) __PRINTFLIKE(4, 5);
}  // namespace log

#endif  // LOG_H