#ifndef _EXECINFO_H
#define _EXECINFO_H 1

#include "features.h"

__BEGIN_DECLS

/* Store up to SIZE return address of the current program state in
   ARRAY and return the exact number of values stored.  */
extern int backtrace (void **__array, int __size);


/* Return names of functions from the backtrace list in ARRAY in a newly
   malloc()ed memory block.  */
extern char **backtrace_symbols (void *__const *__array, int __size);


/* This function is similar to backtrace_symbols() but it writes the result
   immediately to a file.  */
extern void backtrace_symbols_fd (void *__const *__array, int __size, int __fd);


__END_DECLS

#endif /* execinfo.h  */