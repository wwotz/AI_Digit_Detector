#ifndef DEBUG_H_
#define DEBUG_H_

int had_error_debug(void);
const char *pop_error_debug(void);
void push_error_debug(const char *err);
void flush_debug(void);

#endif // DEBUG_H_
