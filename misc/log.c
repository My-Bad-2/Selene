#include <log.h>

#include <kernel/arch/arch.h>

#define MAX_CALLBACKS 32

struct callback {
  log_fn func;
  FILE *stream;
  int level;
};

static struct {
  void *udata;
  int level;
  bool quiet;
  struct callback callbacks[MAX_CALLBACKS];
} logger_conf;

static const char *level_strings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "PANIC"};

static const char *level_colors[] = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m", "\x1b[35m",
};

static void stdout_callback(struct log_event *event) {
  fprintf(event->stream, "%s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", level_colors[event->level],
          level_strings[event->level], event->file, event->line);

  vfprintf(event->stream, event->fmt, event->ap);
  fprintf(event->stream, "\n");
}

static void stream_callback(struct log_event *event) {
  fprintf(event->stream, "%s%-5s %s:%d: ", event->file, event->line);

  vfprintf(event->stream, event->fmt, event->ap);
  fprintf(event->stream, "\n");
}

void log_set_level(int level) { logger_conf.level = level; }

void log_set_quiet(bool enable) { logger_conf.quiet = enable; }

int log_add_callback(log_fn func, FILE *stream, int level) {
  for (int i = 0; i < MAX_CALLBACKS; i++) {
    if (!logger_conf.callbacks[i].func) {
      logger_conf.callbacks[i] = (struct callback){func, stream, level};

      return 0;
    }
  }

  return -1;
}

int log_add_stream(FILE *stream, int level) {
  return log_add_callback(stream_callback, stream, level);
}

static void init_log_event(struct log_event *event, FILE *stream) { event->stream = stream; }

/**
 * @details If the log level meets the threshold, the message is processed by registered callbacks
 * and/or written to streams.
 */
void log_log(int level, const char *file, int line, const char *fmt, ...) {
  struct log_event event = {
      .fmt = fmt,
      .file = file,
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
    struct callback *callback = &logger_conf.callbacks[i];

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