#ifndef NOTIFY_H_
#define NOTIFY_H_

typedef void* notify_handle;

// Return nullptr if failed.
notify_handle notify_init();

// Returns success.
bool notify(notify_handle handle, bool is_english);

void notify_free();

#endif  // NOTIFY_H_
