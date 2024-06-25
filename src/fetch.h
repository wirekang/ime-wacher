#ifndef FETCH_H_
#define FETCH_H_

enum class FetchResult {
  failed,
  english,
  non_english,
};

// Returns success.
bool fetch_init();

FetchResult fetch();

void fetch_free();

#endif  // FETCH_H_
