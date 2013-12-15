
//
// file.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef SPHIA_FILE_H
#define SPHIA_FILE_H 1

// Remove all files from the given path excluding hidden files(starting with "."),
// and any file ending with a extension in the given array. Returns -1 on error.
int
remove_files (const char *, char *[], int);

// Check if a file exists, returns 1 if it does, and 0 if not.
int
file_exists (const char *);

#endif

