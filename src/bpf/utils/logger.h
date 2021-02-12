#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <bpf_helpers.h>

// #define DEBUG 1
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


#endif // __LOGGER_H__
