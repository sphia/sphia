
//
// remove-files.h
//
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//


#ifndef SPHIA_REMOVE_FILES_H
#define SPHIA_REMOVE_FILES_H 1

// Remove all files from the given path excluding hidden files(starting with "."),
// and any file ending with a extension in the given array. Returns -1 on error.
int
remove_files (const char *, char *[], int);

#endif

