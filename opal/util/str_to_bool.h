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

#ifndef OPAL_STR_TO_BOOL_H
#define OPAL_STR_TO_BOOL_H

/**
 * Convert a string to a boolean value.
 *
 * @param str The string to be converted
 *
 *   The string is converted in the following manner:
 *
 *   - Whitespace at the beginning and end of the string is ignored
 *   - If the string value is digits, the return value is "(bool)
 *     atoi(value)"
 *   - If the string value is (case-insensitive) "yes", "y", "true",
 *     "t", or "enabled", the result is true
 *   - If the string value is (case-insensitive) "no", "n", "false",
 *     "f", or "disabled", the result is false
 *   - All other values (including a blank string) will return
 *     OPAL_ERR_VALUE_OUT_OF_BOUNDS
 */
int opal_str_to_bool(const char *str, bool *result);

#endif // OPAL_STR_TO_BOOL_H
