/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2021 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2006 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2006 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2007-2008 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2015      Los Alamos National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2015-2019 Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * Copyright (c) 2017      IBM Corporation.  All rights reserved.
 * Copyright (c) 2017-2019 Intel, Inc.  All rights reserved.
 * Copyright (c) 2018      Amazon.com, Inc. or its affiliates.  All Rights reserved.
 * Copyright (c) 2018      Triad National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2019      Triad National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2024      Jeffrey M. Squyres.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "opal_config.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_SYSLOG_H
#    include <syslog.h>
#endif
#include <fcntl.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#    include <unistd.h>
#endif
#ifdef HAVE_SYS_PARAM_H
#    include <sys/param.h>
#endif

#include "opal/constants.h"
#include "opal/mca/threads/mutex.h"
#include "opal/runtime/opal.h"
#include "opal/util/opal_environ.h"
#include "opal/util/output.h"
#include "opal/util/printf.h"
#include "opal/util/string_copy.h"

/*
 * Private data
 */
static int verbose_stream = -1;
static opal_output_stream_t verbose;
static char *output_dir = NULL;
static char *output_prefix = NULL;

static opal_output_pmix_cleanup_fn_t output_pmix_cleanup_fn = NULL;

/*
 * Internal data structures and helpers for the generalized output
 * stream mechanism.
 */
typedef struct {
    bool ldi_used;
    bool ldi_enabled;
    int ldi_verbose_level;

    bool ldi_syslog;
    int ldi_syslog_priority;

    char *ldi_syslog_ident;
    char *ldi_prefix;
    int ldi_prefix_len;

    char *ldi_suffix;
    int ldi_suffix_len;

    bool ldi_stdout;
    bool ldi_stderr;

    bool ldi_file;
    bool ldi_file_want_append;
    char *ldi_file_suffix;
    int ldi_fd;
    int ldi_file_num_lines_lost;
    char *ldi_filename;
} output_desc_t;

/*
 * Private functions
 */
static void construct(opal_object_t *stream);
static void destruct(opal_object_t *stream);
static int do_open(int output_id, opal_output_stream_t *lds);
static int open_file(int i);
static void free_descriptor(int output_id);
static int make_string(char **no_newline_string, output_desc_t *ldi, const char *format,
                       va_list arglist);
static int output(int output_id, const char *format, va_list arglist);
static void opal_output_finalize(void);

#define OPAL_OUTPUT_MAX_STREAMS 64
#if defined(HAVE_SYSLOG)
#    define USE_SYSLOG 1
#else
#    define USE_SYSLOG 0
#endif

/* global state */
bool opal_output_redirected_to_syslog = false;
int opal_output_redirected_syslog_pri = -1;

/*
 * Local state
 */
static bool initialized = false;
static int default_stderr_fd = -1;
static output_desc_t info[OPAL_OUTPUT_MAX_STREAMS];
static char *temp_str = 0;
static size_t temp_str_len = 0;
static opal_mutex_t mutex;
#if defined(HAVE_SYSLOG)
static bool syslog_opened = false;
#endif
static char *redirect_syslog_ident = NULL;

OBJ_CLASS_INSTANCE(opal_output_stream_t, opal_object_t, construct, destruct);

/*
 * Setup the output stream infrastructure
 */
bool opal_output_init(void)
{
    int i;
    const char *hostname;
    char *str;

    if (initialized) {
        return true;
    }

    str = getenv("OPAL_OUTPUT_STDERR_FD");
    if (NULL != str) {
        default_stderr_fd = atoi(str);
    }
    str = getenv("OPAL_OUTPUT_REDIRECT");
    if (NULL != str) {
        if (0 == strcasecmp(str, "syslog")) {
            opal_output_redirected_to_syslog = true;
        }
    }
    str = getenv("OPAL_OUTPUT_SYSLOG_PRI");
#ifdef HAVE_SYSLOG_H
    if (NULL != str) {
        if (0 == strcasecmp(str, "info")) {
            opal_output_redirected_syslog_pri = LOG_INFO;
        } else if (0 == strcasecmp(str, "error")) {
            opal_output_redirected_syslog_pri = LOG_ERR;
        } else if (0 == strcasecmp(str, "warn")) {
            opal_output_redirected_syslog_pri = LOG_WARNING;
        } else {
            opal_output_redirected_syslog_pri = LOG_ERR;
        }
    } else {
        opal_output_redirected_syslog_pri = LOG_ERR;
    }
#endif /* HAVE_SYSLOG_H */
    str = getenv("OPAL_OUTPUT_SYSLOG_IDENT");
    if (NULL != str) {
        redirect_syslog_ident = strdup(str);
    }

    OBJ_CONSTRUCT(&verbose, opal_output_stream_t);
    if (opal_output_redirected_to_syslog) {
        verbose.lds_want_syslog = true;
        verbose.lds_syslog_priority = opal_output_redirected_syslog_pri;
        if (NULL != str) {
            verbose.lds_syslog_ident = strdup(redirect_syslog_ident);
        }
        verbose.lds_want_stderr = false;
        verbose.lds_want_stdout = false;
    } else {
        str = getenv("OPAL_OUTPUT_INTERNAL_TO_STDOUT");
        if (NULL != str && str[0] == '1') {
            verbose.lds_want_stdout = true;
        } else {
            verbose.lds_want_stderr = true;
        }
    }
    hostname = opal_gethostname();
    opal_asprintf(&verbose.lds_prefix, "[%s:%05d] ", hostname, getpid());

    for (i = 0; i < OPAL_OUTPUT_MAX_STREAMS; ++i) {
        info[i].ldi_used = false;
        info[i].ldi_enabled = false;

        info[i].ldi_syslog = opal_output_redirected_to_syslog;
        info[i].ldi_file = false;
        info[i].ldi_file_suffix = NULL;
        info[i].ldi_file_want_append = false;
        info[i].ldi_fd = -1;
        info[i].ldi_filename = NULL;
        info[i].ldi_file_num_lines_lost = 0;
    }

    /* Initialize the mutex that protects the output */

    OBJ_CONSTRUCT(&mutex, opal_mutex_t);
    initialized = true;

    /* Set some defaults */

    opal_asprintf(&output_prefix, "output-pid%d-", getpid());
    output_dir = strdup(opal_tmp_directory());

    /* Open the default verbose stream */
    verbose_stream = opal_output_open(&verbose);

    /* make sure opal output is cleaned up on finalize */
    opal_finalize_register_cleanup(opal_output_finalize);

    return true;
}

/*
 * Open a stream
 */
int opal_output_open(opal_output_stream_t *lds)
{
    return do_open(-1, lds);
}

/*
 * Reset the parameters on a stream
 */
int opal_output_reopen(int output_id, opal_output_stream_t *lds)
{
    return do_open(output_id, lds);
}

/*
 * Enable and disable output streams
 */
bool opal_output_switch(int output_id, bool enable)
{
    bool ret = false;

    /* Setup */

    if (!initialized) {
        opal_output_init();
    }

    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS) {
        ret = info[output_id].ldi_enabled;
        info[output_id].ldi_enabled = enable;
    }

    return ret;
}

/*
 * Reopen all the streams; used during checkpoint/restart.
 */
void opal_output_reopen_all(void)
{
    char *str;
    const char *hostname;

    str = getenv("OPAL_OUTPUT_STDERR_FD");
    if (NULL != str) {
        default_stderr_fd = atoi(str);
    } else {
        default_stderr_fd = -1;
    }

    hostname = opal_gethostname();
    if (NULL != verbose.lds_prefix) {
        free(verbose.lds_prefix);
        verbose.lds_prefix = NULL;
    }
    opal_asprintf(&verbose.lds_prefix, "[%s:%05d] ", hostname, getpid());
#if 0
    int i;
    opal_output_stream_t lds;

    for (i = 0; i < OPAL_OUTPUT_MAX_STREAMS; ++i) {

        /* scan till we find ldi_used == 0, which is the end-marker */

        if (!info[i].ldi_used) {
            break;
        }

        /*
         * set this to zero to ensure that opal_output_open will
         * return this same index as the output stream id
         */
        info[i].ldi_used = false;

#    if USE_SYSLOG
        lds.lds_want_syslog = info[i].ldi_syslog;
        lds.lds_syslog_priority = info[i].ldi_syslog_priority;
        lds.lds_syslog_ident = info[i].ldi_syslog_ident;
#    else
        lds.lds_want_syslog = false;
#    endif
        lds.lds_prefix = info[i].ldi_prefix;
        lds.lds_suffix = info[i].ldi_suffix;
        lds.lds_want_stdout = info[i].ldi_stdout;
        lds.lds_want_stderr = info[i].ldi_stderr;
        lds.lds_want_file = (-1 == info[i].ldi_fd) ? false : true;
        /* open all streams in append mode */
        lds.lds_want_file_append = true;
        lds.lds_file_suffix = info[i].ldi_file_suffix;

        /*
         * call opal_output_open to open the stream. The return value
         * is guaranteed to be i.  So we can ignore it.
         */
        opal_output_open(&lds);
    }
#endif
}

/*
 * Close a stream
 */
void opal_output_close(int output_id)
{
    int i;

    /* Setup */

    if (!initialized || output_id < 0) {
        return;
    }

    /* If it's valid, used, enabled, and has an open file descriptor,
     * free the resources associated with the descriptor */

    OPAL_THREAD_LOCK(&mutex);
    if (output_id < OPAL_OUTPUT_MAX_STREAMS && info[output_id].ldi_used
        && info[output_id].ldi_enabled) {
        free_descriptor(output_id);

        /* If no one has the syslog open, we should close it */

        for (i = 0; i < OPAL_OUTPUT_MAX_STREAMS; ++i) {
            if (info[i].ldi_used && info[i].ldi_syslog) {
                break;
            }
        }

#if defined(HAVE_SYSLOG) && defined(HAVE_SYSLOG_H)
        if (i >= OPAL_OUTPUT_MAX_STREAMS && syslog_opened) {
            closelog();
        }
#endif
    }

    OPAL_THREAD_UNLOCK(&mutex);
}

/*
 * Main function to send output to a stream
 */
void opal_output(int output_id, const char *format, ...)
{
    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS) {
        va_list arglist;
        va_start(arglist, format);
        output(output_id, format, arglist);
        va_end(arglist);
    }
}

/*
 * Check whether the verbose level is high enough for the given stream
 */
bool opal_output_check_verbosity(int level, int output_id)
{
    return (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS
            && info[output_id].ldi_verbose_level >= level);
}

/*
 * Send a message to a stream if the verbose level is high enough
 */
void opal_output_vverbose(int level, int output_id, const char *format, va_list arglist)
{
    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS
        && info[output_id].ldi_verbose_level >= level) {
        output(output_id, format, arglist);
    }
}

/*
 * Send a message to a string if the verbose level is high enough
 */
char *opal_output_string(int level, int output_id, const char *format, ...)
{
    int rc;
    char *ret = NULL;

    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS
        && info[output_id].ldi_verbose_level >= level) {
        va_list arglist;
        va_start(arglist, format);
        rc = make_string(&ret, &info[output_id], format, arglist);
        va_end(arglist);
        if (OPAL_SUCCESS != rc) {
            ret = NULL;
        }
    }

    return ret;
}

/*
 * Send a message to a string if the verbose level is high enough
 */
char *opal_output_vstring(int level, int output_id, const char *format, va_list arglist)
{
    int rc;
    char *ret = NULL;

    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS
        && info[output_id].ldi_verbose_level >= level) {
        rc = make_string(&ret, &info[output_id], format, arglist);
        if (OPAL_SUCCESS != rc) {
            ret = NULL;
        }
    }

    return ret;
}

/*
 * Set the verbosity level of a stream
 */
void opal_output_set_verbosity(int output_id, int level)
{
    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS) {
        info[output_id].ldi_verbose_level = level;
    }
}

/*
 * Control where output flies will go
 */
void opal_output_set_output_file_info(const char *dir, const char *prefix, char **olddir,
                                      char **oldprefix)
{
    if (NULL != olddir) {
        *olddir = strdup(output_dir);
    }
    if (NULL != oldprefix) {
        *oldprefix = strdup(output_prefix);
    }

    if (NULL != dir) {
        free(output_dir);
        output_dir = strdup(dir);
    }
    if (NULL != prefix) {
        free(output_prefix);
        output_prefix = strdup(prefix);
    }
}

void opal_output_register_pmix_cleanup_fn(opal_output_pmix_cleanup_fn_t cleanup_fn)
{
    int n;

    OPAL_THREAD_LOCK(&mutex);
    /* Register any filenames setup prior to this call */
    for (n = 0; n < OPAL_OUTPUT_MAX_STREAMS; n++) {
        if (NULL != info[n].ldi_filename) {
            output_pmix_cleanup_fn(info[n].ldi_filename, false, true, false);
        }
    }
    /* Save the function pointer for later calls */
    output_pmix_cleanup_fn = cleanup_fn;
    OPAL_THREAD_UNLOCK(&mutex);
}

/*
 * Shut down the output stream system
 */
static void opal_output_finalize(void)
{
    if (initialized) {
        if (verbose_stream != -1) {
            opal_output_close(verbose_stream);
        }
        free(verbose.lds_prefix);
        verbose.lds_prefix = NULL;

        verbose_stream = -1;

        free(output_prefix);
        output_prefix = NULL;

        free(output_dir);
        output_dir = NULL;

        if (NULL != temp_str) {
            free(temp_str);
            temp_str = NULL;
            temp_str_len = 0;
        }
        OBJ_DESTRUCT(&verbose);
        OBJ_DESTRUCT(&mutex);
    }

    initialized = false;
}

/************************************************************************/

/*
 * Constructor
 */
static void construct(opal_object_t *obj)
{
    opal_output_stream_t *stream = (opal_output_stream_t *) obj;

    stream->lds_verbose_level = 0;
    stream->lds_syslog_priority = 0;
    stream->lds_syslog_ident = NULL;
    stream->lds_prefix = NULL;
    stream->lds_suffix = NULL;
    stream->lds_is_debugging = false;
    stream->lds_want_syslog = false;
    stream->lds_want_stdout = false;
    stream->lds_want_stderr = false;
    stream->lds_want_file = false;
    stream->lds_want_file_append = false;
    stream->lds_file_suffix = NULL;
}
static void destruct(opal_object_t *obj)
{
    opal_output_stream_t *stream = (opal_output_stream_t *) obj;

    if (NULL != stream->lds_file_suffix) {
        free(stream->lds_file_suffix);
        stream->lds_file_suffix = NULL;
    }
}

/*
 * Back-end of open() and reopen().  Necessary to have it as a
 * back-end function so that we can do the thread locking properly
 * (especially upon reopen).
 */
static int do_open(int output_id, opal_output_stream_t *lds)
{
    int i;
    bool redirect_to_file = false;
    char *str, *sfx;

    /* Setup */

    if (!initialized) {
        opal_output_init();
    }

    /* Bozo check */

    if (output_id >= OPAL_OUTPUT_MAX_STREAMS ||
        output_id < -1) {
        return OPAL_ERR_BAD_PARAM;
    }

    str = getenv("OPAL_OUTPUT_REDIRECT");
    if (NULL != str && 0 == strcasecmp(str, "file")) {
        redirect_to_file = true;
    }
    sfx = getenv("OPAL_OUTPUT_SUFFIX");

    /* If output_id == -1, find an available stream, or return
     * OPAL_ERROR */

    if (-1 == output_id) {
        OPAL_THREAD_LOCK(&mutex);
        for (i = 0; i < OPAL_OUTPUT_MAX_STREAMS; ++i) {
            if (!info[i].ldi_used) {
                break;
            }
        }
        if (i >= OPAL_OUTPUT_MAX_STREAMS) {
            OPAL_THREAD_UNLOCK(&mutex);
            return OPAL_ERR_OUT_OF_RESOURCE;
        }
    }

    /* Otherwise, we're reopening, so we need to free all previous
     * resources, close files, etc. */

    else {
        free_descriptor(output_id);
        i = output_id;
    }

    /* Special case: if we got NULL for lds, then just use the default
     * verbose */

    if (NULL == lds) {
        lds = &verbose;
    }

    /* Got a stream -- now initialize it and open relevant outputs */

    info[i].ldi_used = true;
    if (-1 == output_id) {
        OPAL_THREAD_UNLOCK(&mutex);
    }
    info[i].ldi_enabled = lds->lds_is_debugging ? (bool) OPAL_ENABLE_DEBUG : true;
    info[i].ldi_verbose_level = lds->lds_verbose_level;

#if USE_SYSLOG
#    if defined(HAVE_SYSLOG) && defined(HAVE_SYSLOG_H)
    if (opal_output_redirected_to_syslog) {
        info[i].ldi_syslog = true;
        info[i].ldi_syslog_priority = opal_output_redirected_syslog_pri;
        if (NULL != redirect_syslog_ident) {
            info[i].ldi_syslog_ident = strdup(redirect_syslog_ident);
            openlog(redirect_syslog_ident, LOG_PID, LOG_USER);
        } else {
            info[i].ldi_syslog_ident = NULL;
            openlog("opal", LOG_PID, LOG_USER);
        }
        syslog_opened = true;
    } else {
#    endif
        info[i].ldi_syslog = lds->lds_want_syslog;
        if (lds->lds_want_syslog) {

#    if defined(HAVE_SYSLOG) && defined(HAVE_SYSLOG_H)
            if (NULL != lds->lds_syslog_ident) {
                info[i].ldi_syslog_ident = strdup(lds->lds_syslog_ident);
                openlog(lds->lds_syslog_ident, LOG_PID, LOG_USER);
            } else {
                info[i].ldi_syslog_ident = NULL;
                openlog("opal", LOG_PID, LOG_USER);
            }
#    endif
            syslog_opened = true;
            info[i].ldi_syslog_priority = lds->lds_syslog_priority;
        }

#    if defined(HAVE_SYSLOG) && defined(HAVE_SYSLOG_H)
    }
#    endif

#else
    info[i].ldi_syslog = false;
#endif

    if (NULL != lds->lds_prefix) {
        info[i].ldi_prefix = strdup(lds->lds_prefix);
        info[i].ldi_prefix_len = (int) strlen(lds->lds_prefix);
    } else {
        info[i].ldi_prefix = NULL;
        info[i].ldi_prefix_len = 0;
    }

    if (NULL != lds->lds_suffix) {
        info[i].ldi_suffix = strdup(lds->lds_suffix);
        info[i].ldi_suffix_len = (int) strlen(lds->lds_suffix);
    } else {
        info[i].ldi_suffix = NULL;
        info[i].ldi_suffix_len = 0;
    }

    if (opal_output_redirected_to_syslog) {
        /* since all is redirected to syslog, ensure
         * we don't duplicate the output to the std places
         */
        info[i].ldi_stdout = false;
        info[i].ldi_stderr = false;
        info[i].ldi_file = false;
        info[i].ldi_fd = -1;
    } else {
        /* since we aren't redirecting to syslog, use what was
         * given to us
         */
        if (NULL != str && redirect_to_file) {
            info[i].ldi_stdout = false;
            info[i].ldi_stderr = false;
            info[i].ldi_file = true;
        } else {
            info[i].ldi_stdout = lds->lds_want_stdout;
            info[i].ldi_stderr = lds->lds_want_stderr;

            info[i].ldi_fd = -1;
            info[i].ldi_file = lds->lds_want_file;
        }
        if (NULL != sfx) {
            info[i].ldi_file_suffix = strdup(sfx);
        } else {
            info[i].ldi_file_suffix = (NULL == lds->lds_file_suffix) ? NULL
                                                                     : strdup(lds->lds_file_suffix);
        }
        info[i].ldi_file_want_append = lds->lds_want_file_append;
        info[i].ldi_file_num_lines_lost = 0;
    }

    /* Special case: output_id == 0 == verbose_stream
     * This is the verbose stream, so update the internal 'verbose_stream'
     * to match the parameters set in the info[i]
     */
    if (verbose_stream == i) {
        verbose.lds_want_syslog = info[i].ldi_syslog;
        verbose.lds_syslog_priority = info[i].ldi_syslog_priority;
        verbose.lds_syslog_ident = info[i].ldi_syslog_ident;
        verbose.lds_want_stdout = info[i].ldi_stdout;
        verbose.lds_want_stderr = info[i].ldi_stderr;
    }

    /* Don't open a file in the session directory now -- do that lazily
     * so that if there's no output, we don't have an empty file */

    return i;
}

static int open_file(int i)
{
    int flags;
    int n;

    /* first check to see if this file is already open
     * on someone else's stream - if so, we don't want
     * to open it twice
     */
    for (n = 0; n < OPAL_OUTPUT_MAX_STREAMS; n++) {
        if (i == n) {
            continue;
        }
        if (!info[n].ldi_used) {
            continue;
        }
        if (!info[n].ldi_file) {
            continue;
        }
        if (NULL != info[i].ldi_file_suffix && NULL != info[n].ldi_file_suffix) {
            if (0 != strcmp(info[i].ldi_file_suffix, info[n].ldi_file_suffix)) {
                break;
            }
        }
        if (NULL == info[i].ldi_file_suffix && NULL != info[n].ldi_file_suffix) {
            break;
        }
        if (NULL != info[i].ldi_file_suffix && NULL == info[n].ldi_file_suffix) {
            break;
        }
        if (info[n].ldi_fd < 0) {
            break;
        }
        info[i].ldi_fd = info[n].ldi_fd;
        return OPAL_SUCCESS;
    }

    /* Setup the filename and open flags */

    if (NULL != output_dir) {
        if (NULL != info[i].ldi_filename) {
            free(info[i].ldi_filename);
        }
        info[i].ldi_filename = (char *) malloc(OPAL_PATH_MAX);
        if (NULL == info[i].ldi_filename) {
            return OPAL_ERR_OUT_OF_RESOURCE;
        }
        opal_string_copy(info[i].ldi_filename, output_dir, OPAL_PATH_MAX);
        strcat(info[i].ldi_filename, "/");
        if (NULL != output_prefix) {
            strcat(info[i].ldi_filename, output_prefix);
        }
        if (info[i].ldi_file_suffix != NULL) {
            strcat(info[i].ldi_filename, info[i].ldi_file_suffix);
        } else {
            info[i].ldi_file_suffix = NULL;
            strcat(info[i].ldi_filename, "output.txt");
        }
        flags = O_CREAT | O_RDWR;
        if (!info[i].ldi_file_want_append) {
            flags |= O_TRUNC;
        }

        /* Actually open the file */
        info[i].ldi_fd = open(info[i].ldi_filename, flags, 0644);
        if (-1 == info[i].ldi_fd) {
            info[i].ldi_used = false;
            /* release the filename in all error cases */
            free(info[i].ldi_filename);
            info[i].ldi_filename = NULL;
            return OPAL_ERR_IN_ERRNO;
        }

        /* Make the file be close-on-exec to prevent child inheritance
         * problems */
        if (-1 == fcntl(info[i].ldi_fd, F_SETFD, 1)) {
            /* release the filename in all error cases */
            free(info[i].ldi_filename);
            info[i].ldi_filename = NULL;
            return OPAL_ERR_IN_ERRNO;
        }

        /* register it to be ignored */
        if (NULL != output_pmix_cleanup_fn) {
            output_pmix_cleanup_fn(info[i].ldi_filename, false, true, false);
        }
    }

    /* Return successfully even if the session dir did not exist yet;
     * we'll try opening it later */

    return OPAL_SUCCESS;
}

/*
 * Free all the resources associated with a descriptor.
 */
static void free_descriptor(int output_id)
{
    output_desc_t *ldi;

    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS && info[output_id].ldi_used
        && info[output_id].ldi_enabled) {
        ldi = &info[output_id];

        if (-1 != ldi->ldi_fd) {
            close(ldi->ldi_fd);
        }
        if (NULL != ldi->ldi_filename) {
            free(ldi->ldi_filename);
            ldi->ldi_filename = NULL;
        }
        ldi->ldi_used = false;

        /* If we strduped a prefix, suffix, or syslog ident, free it */

        if (NULL != ldi->ldi_prefix) {
            free(ldi->ldi_prefix);
        }
        ldi->ldi_prefix = NULL;

        if (NULL != ldi->ldi_suffix) {
            free(ldi->ldi_suffix);
        }
        ldi->ldi_suffix = NULL;

        if (NULL != ldi->ldi_file_suffix) {
            free(ldi->ldi_file_suffix);
        }
        ldi->ldi_file_suffix = NULL;

        if (NULL != ldi->ldi_syslog_ident) {
            free(ldi->ldi_syslog_ident);
        }
        ldi->ldi_syslog_ident = NULL;
    }
}

static int make_string(char **no_newline_string, output_desc_t *ldi, const char *format,
                       va_list arglist)
{
    size_t len, total_len;
    bool want_newline = false;

    /* Make the formatted string */

    opal_vasprintf(no_newline_string, format, arglist);
    total_len = len = strlen(*no_newline_string);
    if ('\n' != (*no_newline_string)[len - 1]) {
        want_newline = true;
        ++total_len;
    } else if (NULL != ldi->ldi_suffix) {
        /* if we have a suffix, then we don't want a
         * newline to appear before it
         */
        (*no_newline_string)[len - 1] = '\0';
        want_newline = true; /* add newline to end after suffix */
        /* total_len won't change since we just moved the newline
         * to appear after the suffix
         */
    }
    if (NULL != ldi->ldi_prefix) {
        total_len += strlen(ldi->ldi_prefix);
    }
    if (NULL != ldi->ldi_suffix) {
        total_len += strlen(ldi->ldi_suffix);
    }
    if (temp_str_len < total_len + want_newline) {
        if (NULL != temp_str) {
            free(temp_str);
        }
        temp_str = (char *) malloc(total_len * 2);
        if (NULL == temp_str) {
            return OPAL_ERR_OUT_OF_RESOURCE;
        }
        temp_str_len = total_len * 2;
    }
    if (NULL != ldi->ldi_prefix && NULL != ldi->ldi_suffix) {
        if (want_newline) {
            snprintf(temp_str, temp_str_len, "%s%s%s\n", ldi->ldi_prefix, *no_newline_string,
                     ldi->ldi_suffix);
        } else {
            snprintf(temp_str, temp_str_len, "%s%s%s", ldi->ldi_prefix, *no_newline_string,
                     ldi->ldi_suffix);
        }
    } else if (NULL != ldi->ldi_prefix) {
        if (want_newline) {
            snprintf(temp_str, temp_str_len, "%s%s\n", ldi->ldi_prefix, *no_newline_string);
        } else {
            snprintf(temp_str, temp_str_len, "%s%s", ldi->ldi_prefix, *no_newline_string);
        }
    } else if (NULL != ldi->ldi_suffix) {
        if (want_newline) {
            snprintf(temp_str, temp_str_len, "%s%s\n", *no_newline_string, ldi->ldi_suffix);
        } else {
            snprintf(temp_str, temp_str_len, "%s%s", *no_newline_string, ldi->ldi_suffix);
        }
    } else {
        if (want_newline) {
            snprintf(temp_str, temp_str_len, "%s\n", *no_newline_string);
        } else {
            snprintf(temp_str, temp_str_len, "%s", *no_newline_string);
        }
    }

    return OPAL_SUCCESS;
}

/*
 * Do the actual output.  Take a va_list so that we can be called from
 * multiple different places, even functions that took "..." as input
 * arguments.
 */
static int output(int output_id, const char *format, va_list arglist)
{
    int rc = OPAL_SUCCESS;
    char *str, *out = NULL;
    output_desc_t *ldi;

    /* Setup */

    if (!initialized) {
        opal_output_init();
    }

    /* If it's valid, used, and enabled, output */

    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS && info[output_id].ldi_used
        && info[output_id].ldi_enabled) {
        OPAL_THREAD_LOCK(&mutex);
        ldi = &info[output_id];

        /* Make the strings */
        if (OPAL_SUCCESS != (rc = make_string(&str, ldi, format, arglist))) {
            OPAL_THREAD_UNLOCK(&mutex);
            return rc;
        }

        /* Syslog output -- does not use the newline-appended string */
#if defined(HAVE_SYSLOG) && defined(HAVE_SYSLOG_H)
        if (ldi->ldi_syslog) {
            syslog(ldi->ldi_syslog_priority, "%s", str);
        }
#endif

        /* All others (stdout, stderr, file) use temp_str, potentially
           with a newline appended */

        out = temp_str;

        /* stdout output */
        if (ldi->ldi_stdout) {
            int tmp = opal_best_effort_write(fileno(stdout), out, strlen(out));
            if (OPAL_SUCCESS != tmp) {
                rc = tmp;
            }
            fflush(stdout);
        }

        /* stderr output */
        if (ldi->ldi_stderr) {
            int tmp = opal_best_effort_write((-1 == default_stderr_fd) ? fileno(stderr) : default_stderr_fd,
                                             out, strlen(out));
            if (OPAL_SUCCESS != tmp) {
                rc = tmp;
            }
            fflush(stderr);
        }

        /* File output -- first check to see if the file opening was
         * delayed.  If so, try to open it.  If we failed to open it,
         * then just discard (there are big warnings in the
         * opal_output.h docs about this!). */

        if (ldi->ldi_file) {
            if (ldi->ldi_fd == -1) {
                if (OPAL_SUCCESS != open_file(output_id)) {
                    ++ldi->ldi_file_num_lines_lost;
                } else if (ldi->ldi_file_num_lines_lost > 0) {
                    char buffer[BUFSIZ];
                    int tmp;
                    memset(buffer, 0, BUFSIZ);
                    snprintf(buffer, BUFSIZ - 1,
                             "[WARNING: %d lines lost because the Open MPI process session "
                             "directory did\n not exist when opal_output() was invoked]\n",
                             ldi->ldi_file_num_lines_lost);
                    tmp = opal_best_effort_write(ldi->ldi_fd, buffer, strlen(buffer));
                    if (OPAL_SUCCESS != tmp) {
                        rc = tmp;
                    }
                    ldi->ldi_file_num_lines_lost = 0;
                }
            }
            if (ldi->ldi_fd != -1) {
                int tmp = opal_best_effort_write(ldi->ldi_fd, out, strlen(out));
                if (OPAL_SUCCESS != tmp) {
                    rc = tmp;
                }
            }
        }
        OPAL_THREAD_UNLOCK(&mutex);
        free(str);
    }

    return rc;
}

int opal_output_get_verbosity(int output_id)
{
    if (output_id >= 0 && output_id < OPAL_OUTPUT_MAX_STREAMS && info[output_id].ldi_used) {
        return info[output_id].ldi_verbose_level;
    } else {
        return -1;
    }
}
