#define _GNU_SOURCE
#include <string.h>
#include <stdarg.h>	// handle variable args in open
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>	// for setting an error code
#include <dlfcn.h>	// for dynamic loading


// interpose the open call
// open takes an optional third argument - the permissions for creating a file
// we don't care what it's set to but will have to handle the variable # of args
//	int open(const char *pathname, int flags)
//	int open(const char *pathname, int flags, mode_t mode)
//
int open(const char *pathname, int flags, ...)
{
	// your code here
}

// interpose the readdir call
// This isn't the system call but is used by many programs that need to read directories, 
// like find and ls.

struct dirent *readdir(DIR *dirp)
{
	// your code here
}
