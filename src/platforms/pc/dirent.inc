/*-------------------------------------------------------------------------
 *
 * dirent.c
 *	  opendir/readdir/closedir for win32/msvc
 *
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/port/dirent.c
 *
 *-------------------------------------------------------------------------
 */

#define DT_DIR 4
#define DT_REG 8
#define DT_LNK 16

#include "tkc/types_def.h"

#ifdef __GNUC__

#include "errno.h"

#endif

struct dirent {
  long d_ino;
  uint8_t d_type;
  unsigned short d_reclen;
  unsigned short d_namlen;
  wchar_t d_name[MAX_PATH];
};

typedef struct DIR DIR;

DIR* opendir(const wchar_t*);
struct dirent* readdir(DIR*);
int closedir(DIR*);

struct DIR {
  wchar_t* dirname;
  struct dirent ret; /* Used to return to caller */
  HANDLE handle;
};

DIR* opendir(const wchar_t* dirname) {
  DWORD attr;
  DIR* d;

  /* Make sure it is a directory */
  attr = GetFileAttributesW(dirname);
  if (attr == INVALID_FILE_ATTRIBUTES) {
    errno = ENOENT;
    return NULL;
  }
  if ((attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
    errno = ENOTDIR;
    return NULL;
  }

  d = malloc(sizeof(DIR));
  if (!d) {
    errno = ENOMEM;
    return NULL;
  }
  d->dirname = malloc(wcslen(dirname) * sizeof(wchar_t) + 8);
  if (!d->dirname) {
    errno = ENOMEM;
    free(d);
    return NULL;
  }
  wcscpy(d->dirname, dirname);
  if (d->dirname[wcslen(d->dirname) - 1] != '/' && d->dirname[wcslen(d->dirname) - 1] != '\\')
    wcscat(d->dirname, L"\\"); /* Append backslash if not already
                                * there */
  wcscat(d->dirname, L"*");    /* Search for entries named anything */
  d->handle = INVALID_HANDLE_VALUE;
  d->ret.d_ino = 0;    /* no inodes on win32 */
  d->ret.d_reclen = 0; /* not used on win32 */

  return d;
}

void rewinddir(DIR* d) {
}

struct dirent* readdir(DIR* d) {
  WIN32_FIND_DATAW fd;

  if (d->handle == INVALID_HANDLE_VALUE) {
    d->handle = FindFirstFileW(d->dirname, &fd);
    if (d->handle == INVALID_HANDLE_VALUE) {
      errno = ENOENT;
      return NULL;
    }
  } else {
    if (!FindNextFileW(d->handle, &fd)) {
      if (GetLastError() == ERROR_NO_MORE_FILES) {
        /* No more files, force errno=0 (unlike mingw) */
        errno = 0;
        return NULL;
      }
      return NULL;
    }
  }
  wcscpy(d->ret.d_name, fd.cFileName); /* Both strings are MAX_PATH
                                        * long */
  d->ret.d_namlen = wcslen(d->ret.d_name);

  d->ret.d_type = 0;
  if ((fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) ||
      (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)) {
    d->ret.d_type |= DT_REG;
  }

  if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    d->ret.d_type |= DT_DIR;
  }

  return &d->ret;
}

int closedir(DIR* d) {
  int ret = 0;

  if (d->handle != INVALID_HANDLE_VALUE) ret = !FindClose(d->handle);
  free(d->dirname);
  free(d);

  return ret;
}
