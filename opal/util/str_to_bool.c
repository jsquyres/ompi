/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2007 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2007-2018 Cisco Systems, Inc.  All rights reserved
 * Copyright (c) 2009      Sun Microsystems, Inc.  All rights reserved.
 * Copyright (c) 2012-2017 Los Alamos National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2017-2018 IBM Corporation. All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "opal_config.h"

#include <string.h>
#include <ctype.h>

#include "opal/constants.h"
#include "opal/util/str2bool.h"


int opal_str_to_bool(const char *str, bool *result)
{
    int ret = OPAL_SUCCESS;
    size_t len;

    /* Skip leading whitespace */
    str += strspn(str, " \t\n\v\f\r");
    len = strcspn(str, " \t\n\v\f\r");

    /* Empty string? */
    if (*str == '\0') {
        return OPAL_ERR_VALUE_OUT_OF_BOUNDS;
    }

    if (isdigit(*str)) {
        *result = (bool) atoi(str);
    } else if (0 == strncasecmp(str, "yes", len) ||
               0 == strncasecmp(str, "y", len) ||
               0 == strncasecmp(str, "true", len) ||
               0 == strncasecmp(str, "t", len) ||
               0 == strncasecmp(str, "enabled", len)) {
        *result = true;
    } else if (0 == strncasecmp(str, "no", len) ||
               0 == strncasecmp(str, "n", len) ||
               0 == strncasecmp(str, "false", len) ||
               0 == strncasecmp(str, "f", len) ||
               0 == strncasecmp(str, "disabled", len)) {
        *result = false;
    } else {
        ret = OPAL_ERR_VALUE_OUT_OF_BOUNDS;
    }
    
    return ret;
}
