#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <bpf_helpers.h>

#define DEBUG 1
#ifdef  DEBUG
/* Only use this for debug output. Notice output from bpf_trace_printk()
 * end-up in /sys/kernel/debug/tracing/trace_pipe
 * 
 * TODO navarrothiago Insert "[Debug] " tag in define.
 */
#define bpf_debug(fmt, ...)						\
		({							\
			char ____fmt[] = fmt;				\
			bpf_trace_printk(____fmt, sizeof(____fmt),	\
				     ##__VA_ARGS__);			\
		})
#else
#define bpf_debug(fmt, ...) { } while (0)
#endif


static void red () {
  bpf_debug("\033[1;31m");
}

static void green () {
  bpf_debug("\033[0;32m");
}

static void yellow() {
  bpf_debug("\033[1;33m");
}

static void reset () {
  bpf_debug("\033[0m");
}

#endif // __LOGGER_H__