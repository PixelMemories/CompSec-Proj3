#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>  // for dynamic loading
#include <errno.h>  // for setting an error code
#include <stdarg.h>  // handle variable args in open
#include <fcntl.h>

/* Typedef for the original readdir function */
typedef struct dirent *(*orig_readdir_type)(DIR *);

static orig_readdir_type real_readdir = NULL;

/* Globals to store hidden file names */
static int hidden_initialized = 0;
static char **hidden_files = NULL;
static int hidden_count = 0;

/* Function to initialize the list of hidden files */
void init_hidden(void) {
    if (hidden_initialized)
        return;

    char *env = getenv("HIDDEN");
    if (env) {
        /* Duplicate the string since strtok modifies it */
        char *env_copy = strdup(env);
        char *token = strtok(env_copy, ":");
        while (token != NULL) {
            hidden_files = realloc(hidden_files, sizeof(char*) * (hidden_count + 1));
            if (hidden_files == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            hidden_files[hidden_count] = strdup(token);
            hidden_count++;
            token = strtok(NULL, ":");
        }
        free(env_copy);
    }
    hidden_initialized = 1;
}

// interpose the readdir call
// This isn't the system call but is used by many programs that need to read directories, 
// like find and ls.


/* Replacement readdir function */
struct dirent *readdir(DIR *dirp) {
    if (!real_readdir)
        real_readdir = (orig_readdir_type)dlsym(RTLD_NEXT, "readdir");

    if (!hidden_initialized)
        init_hidden();

    struct dirent *entry;
    while ((entry = real_readdir(dirp)) != NULL) {
        int hide = 0;
        for (int i = 0; i < hidden_count; i++) {
            if (strstr(entry->d_name, hidden_files[i]) != NULL) {
                hide = 1;
                break;
            }
        }
        if (!hide)
            return entry;
        /* Otherwise, continue looping to skip hidden entries */
    }
    return NULL;
}


/* Typedef for the original open function */
typedef int (*orig_open_type)(const char *pathname, int flags, ...);
static orig_open_type real_open = NULL;

/* Globals to store blocked suffixes */
static int blocked_initialized = 0;
static char **blocked_suffixes = NULL;
static int blocked_count = 0;

/* Function to initialize the list of blocked suffixes */
void init_blocked(void) {
    if (blocked_initialized)
        return;

    char *env = getenv("BLOCKED");
    if (env) {
        char *env_copy = strdup(env);
        char *token = strtok(env_copy, ":");
        while (token != NULL) {
            blocked_suffixes = realloc(blocked_suffixes, sizeof(char*) * (blocked_count + 1));
            if (blocked_suffixes == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            blocked_suffixes[blocked_count] = strdup(token);
            blocked_count++;
            token = strtok(NULL, ":");
        }
        free(env_copy);
    }
    blocked_initialized = 1;
}

// interpose the open call
// open takes an optional third argument - the permissions for creating a file
// we don't care what it's set to but will have to handle the variable # of args
//	int open(const char *pathname, int flags)
//	int open(const char *pathname, int flags, mode_t mode)
//

/* Replacement open function */
int open(const char *pathname, int flags, ...) {
    if (!real_open)
        real_open = (orig_open_type)dlsym(RTLD_NEXT, "open");

    if (!blocked_initialized)
        init_blocked();

    /* Check the pathname for any blocked suffix */
    for (int i = 0; i < blocked_count; i++) {
        size_t path_len = strlen(pathname);
        size_t suffix_len = strlen(blocked_suffixes[i]);
        if (path_len >= suffix_len) {
            if (strcmp(pathname + path_len - suffix_len, blocked_suffixes[i]) == 0) {
                errno = EACCES;
                return -1;
            }
        }
    }

    /* Forward the call using variable arguments */
    int fd;
    va_list args;
    va_start(args, flags);
    if (flags & O_CREAT) {
        mode_t mode = va_arg(args, mode_t);
        fd = real_open(pathname, flags, mode);
    } else {
        fd = real_open(pathname, flags);
    }
    va_end(args);
    return fd;
}


