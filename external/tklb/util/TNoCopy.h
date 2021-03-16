#ifndef TKLB_NO_COPY

/**
 * A macro to disable all kinds of implicit copy mechanisms
 */
#define TKLB_NO_COPY(name) \
name(const name&) = delete; \
name(const name*) = delete; \
name(name&&) = delete; \
name& operator= (const name&) = delete; \
name& operator= (name&&) = delete;

#endif // TKLB_NO_COPY
