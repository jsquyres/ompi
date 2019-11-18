/*
 * Copyright (c) 2011-2013 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2018-2019 Intel, Inc.  All rights reserved.
 * Copyright (c) 2019      Research Organization for Information Science
 *                         and Technology (RIST).  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
/** @file:
 */

#ifndef ORTE_MCA_STATE_BASE_H
#define ORTE_MCA_STATE_BASE_H

/*
 * includes
 */
#include "orte_config.h"
#include "orte/constants.h"

#include "opal/class/opal_list.h"
#include "opal/util/printf.h"

#include "orte/mca/mca.h"
#include "orte/mca/state/state.h"


BEGIN_C_DECLS

/* select a component */
ORTE_DECLSPEC    int orte_state_base_select(void);

/* debug tools */
ORTE_DECLSPEC void orte_state_base_print_job_state_machine(void);

ORTE_DECLSPEC void orte_state_base_print_proc_state_machine(void);

ORTE_DECLSPEC extern int orte_state_base_parent_fd;

END_C_DECLS

#endif
