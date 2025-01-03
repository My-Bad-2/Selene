#include <array>

#include <kernel/arch/arch.hpp>
#include <log.hpp>

namespace log {
namespace {
constexpr uint8_t MAX_CALLBACKS = 32;

struct Callback {
  log_fn func;
  FILE *stream;
  int level;
};

struct {
  void *udata;
  int level;
  bool quiet;
  struct Callback callbacks[MAX_CALLBACKS];
} logger_conf;

std::array<const char *, 7> level_strings = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "PANIC",
};

std::array<const char *, 7> level_colors = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m", "\x1b[35m",
};

void stdout_callback(struct LogEvent *event) {
  fprintf(event->stream, "%s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", level_colors[event->level],
          level_strings[event->level], event->file, event->line);

  vfprintf(event->stream, event->fmt, event->ap);
  fprintf(event->stream, "\n");
}

void stream_callback(struct LogEvent *event) {
  fprintf(event->stream, "%s%-5s %s:%d: ", event->file, event->line);

  vfprintf(event->stream, event->fmt, event->ap);
  fprintf(event->stream, "\n");
}

void init_log_event(LogEvent *event, FILE *stream) { event->stream = stream; }
}  // namespace

void set_level(int level) { logger_conf.level = level; }

void set_quiet(bool enable) { logger_conf.quiet = enable; }

int add_callback(log_fn func, FILE *stream, int level) {
  for (auto &callback : logger_conf.callbacks) {
    if (!callback.func) {
      callback = {
          .func = func,
          .stream = stream,
          .level = level,
      };

      return 0;
    }
  }

  return -1;
}

int add_stream(FILE *stream, int level) { return add_callback(stream_callback, stream, level); }

/**
 * @details If the log level meets the threshold, the message is processed by registered callbacks
 * and/or written to streams.
 */
void log(int level, const char *file, int line, const char *fmt, ...) {
  LogEvent event = {
      .ap = {},
      .fmt = fmt,
      .file = file,
      .stream = nullptr,
      .line = line,
      .level = level,
  };

  if (!logger_conf.quiet && level >= logger_conf.level) {
    init_log_event(&event, stderr);
    va_start(event.ap, fmt);
    stdout_callback(&event);
    va_end(event.ap);
  }

  for (int i = 0; i < MAX_CALLBACKS && logger_conf.callbacks[i].func; i++) {
    struct Callback *callback = &logger_conf.callbacks[i];

    if (level >= callback->level) {
      init_log_event(&event, callback->stream);
      va_start(event.ap, fmt);
      callback->func(&event);
      va_end(event.ap);
    }
  }

  if (level == LOG_PANIC) {
    arch_halt(false);
  }
}
}  // namespace log