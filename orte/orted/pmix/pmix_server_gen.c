/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2011 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2006-2013 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2009      Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2011      Oak Ridge National Labs.  All rights reserved.
 * Copyright (c) 2013-2019 Intel, Inc.  All rights reserved.
 * Copyright (c) 2014-2017 Mellanox Technologies, Inc.
 *                         All rights reserved.
 * Copyright (c) 2014-2019 Research Organization for Information Science
 *                         and Technology (RIST).  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include "orte_config.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "opal/util/argv.h"
#include "opal/util/output.h"
#include "opal/dss/dss.h"
#include "opal/hwloc/hwloc-internal.h"
#include "opal/mca/pstat/pstat.h"

#include "orte/mca/errmgr/errmgr.h"
#include "orte/mca/iof/iof.h"
#include "orte/mca/rmaps/rmaps_types.h"
#include "orte/mca/schizo/schizo.h"
#include "orte/mca/state/state.h"
#include "orte/util/name_fns.h"
#include "orte/util/show_help.h"
#include "orte/util/threads.h"
#include "orte/runtime/orte_globals.h"
#include "orte/mca/rml/rml.h"
#include "orte/mca/plm/plm.h"
#include "orte/mca/plm/base/plm_private.h"

#include "pmix_server_internal.h"

static void _client_conn(int sd, short args, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd = (orte_pmix_server_op_caddy_t*)cbdata;
    orte_job_t *jdata;
    orte_proc_t *p, *ptr;
    int i;

    ORTE_ACQUIRE_OBJECT(cd);

    if (NULL != cd->server_object) {
        /* we were passed back the orte_proc_t */
        p = (orte_proc_t*)cd->server_object;
    } else {
        /* find the named process */
        p = NULL;
        if (NULL == (jdata = orte_get_job_data_object(cd->proc.jobid))) {
            return;
        }
        for (i=0; i < jdata->procs->size; i++) {
            if (NULL == (ptr = (orte_proc_t*)opal_pointer_array_get_item(jdata->procs, i))) {
                continue;
            }
            if (cd->proc.jobid != ptr->name.jobid) {
                continue;
            }
            if (cd->proc.vpid == ptr->name.vpid) {
                p = ptr;
                break;
            }
        }
    }
    if (NULL != p) {
        ORTE_FLAG_SET(p, ORTE_PROC_FLAG_REG);
        ORTE_ACTIVATE_PROC_STATE(&p->name, ORTE_PROC_STATE_REGISTERED);
    }
    if (NULL != cd->cbfunc) {
        cd->cbfunc(PMIX_SUCCESS, cd->cbdata);
    }
    OBJ_RELEASE(cd);
}

pmix_status_t pmix_server_client_connected_fn(const pmix_proc_t *proc, void* server_object,
                                              pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    orte_process_name_t name;
    int rc;

    OPAL_PMIX_CONVERT_PROCT(rc, &name, proc);
    if (OPAL_SUCCESS != rc) {
        return PMIX_ERR_BAD_PARAM;
    }

    /* need to thread-shift this request as we are going
     * to access our global list of registered events */
    ORTE_PMIX_THREADSHIFT(&name, server_object, OPAL_SUCCESS, NULL,
                          NULL, 0, _client_conn, cbfunc, cbdata);
    return ORTE_SUCCESS;
}

static void _client_finalized(int sd, short args, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd = (orte_pmix_server_op_caddy_t*)cbdata;
    orte_job_t *jdata;
    orte_proc_t *p, *ptr;
    int i;

    ORTE_ACQUIRE_OBJECT(cd);

    if (NULL != cd->server_object) {
        /* we were passed back the orte_proc_t */
        p = (orte_proc_t*)cd->server_object;
    } else {
        /* find the named process */
        p = NULL;
        if (NULL == (jdata = orte_get_job_data_object(cd->proc.jobid))) {
            /* this tool was not started by us and we have
             * no job record for it - this shouldn't happen,
             * so let's error log it */
            ORTE_ERROR_LOG(ORTE_ERR_NOT_FOUND);
            /* ensure they don't hang */
            goto release;
        }
        for (i=0; i < jdata->procs->size; i++) {
            if (NULL == (ptr = (orte_proc_t*)opal_pointer_array_get_item(jdata->procs, i))) {
                continue;
            }
            if (cd->proc.jobid != ptr->name.jobid) {
                continue;
            }
            if (cd->proc.vpid == ptr->name.vpid) {
                p = ptr;
                break;
            }
        }
        /* if we came thru this code path, then this client must be an
         * independent tool that connected to us - i.e., it wasn't
         * something we spawned. For accounting purposes, we have to
         * ensure the job complete procedure is run - otherwise, slots
         * and other resources won't correctly be released */
        ORTE_FLAG_SET(p, ORTE_PROC_FLAG_IOF_COMPLETE);
        ORTE_FLAG_SET(p, ORTE_PROC_FLAG_WAITPID);
        ORTE_ACTIVATE_PROC_STATE(&cd->proc, ORTE_PROC_STATE_TERMINATED);
    }
    if (NULL != p) {
        ORTE_FLAG_SET(p, ORTE_PROC_FLAG_HAS_DEREG);
    }

  release:
    /* release the caller */
    if (NULL != cd->cbfunc) {
        cd->cbfunc(PMIX_SUCCESS, cd->cbdata);
    }
    OBJ_RELEASE(cd);
}

pmix_status_t pmix_server_client_finalized_fn(const pmix_proc_t *proc, void* server_object,
                                              pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    orte_process_name_t name;
    int rc;

    OPAL_PMIX_CONVERT_PROCT(rc, &name, proc);
    if (OPAL_SUCCESS != rc) {
        return PMIX_ERR_BAD_PARAM;
    }

    /* need to thread-shift this request as we are going
     * to access our global list of registered events */
    ORTE_PMIX_THREADSHIFT(&name, server_object, OPAL_SUCCESS, NULL,
                          NULL, 0, _client_finalized, cbfunc, cbdata);
    return ORTE_SUCCESS;

}

static void _client_abort(int sd, short args, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd = (orte_pmix_server_op_caddy_t*)cbdata;
    orte_job_t *jdata;
    orte_proc_t *p, *ptr;
    int i;

    ORTE_ACQUIRE_OBJECT(cd);

    if (NULL != cd->server_object) {
        p = (orte_proc_t*)cd->server_object;
    } else {
        /* find the named process */
        p = NULL;
        if (NULL == (jdata = orte_get_job_data_object(cd->proc.jobid))) {
            return;
        }
        for (i=0; i < jdata->procs->size; i++) {
            if (NULL == (ptr = (orte_proc_t*)opal_pointer_array_get_item(jdata->procs, i))) {
                continue;
            }
            if (cd->proc.jobid != ptr->name.jobid) {
                continue;
            }
            if (cd->proc.vpid == ptr->name.vpid) {
                p = ptr;
                break;
            }
        }
    }
    if (NULL != p) {
        p->exit_code = cd->status;
        ORTE_ACTIVATE_PROC_STATE(&p->name, ORTE_PROC_STATE_CALLED_ABORT);
    }

    /* release the caller */
    if (NULL != cd->cbfunc) {
        cd->cbfunc(OPAL_SUCCESS, cd->cbdata);
    }
    OBJ_RELEASE(cd);
}

pmix_status_t pmix_server_abort_fn(const pmix_proc_t *proc, void *server_object,
                                   int status, const char msg[],
                                   pmix_proc_t procs[], size_t nprocs,
                                   pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    orte_process_name_t name;
    int rc;

    OPAL_PMIX_CONVERT_PROCT(rc, &name, proc);
    if (OPAL_SUCCESS != rc) {
        return PMIX_ERR_BAD_PARAM;
    }

    /* need to thread-shift this request as we are going
     * to access our global list of registered events */
    ORTE_PMIX_THREADSHIFT(&name, server_object, status, msg,
                          procs, nprocs, _client_abort, cbfunc, cbdata);
    return ORTE_SUCCESS;
}

static void _register_events(int sd, short args, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd = (orte_pmix_server_op_caddy_t*)cbdata;

    ORTE_ACQUIRE_OBJECT(cd);

    /* need to implement this */

    if (NULL != cd->cbfunc) {
        cd->cbfunc(ORTE_SUCCESS, cd->cbdata);
    }
    OBJ_RELEASE(cd);
}

/* hook for the local PMIX server to pass event registrations
 * up to us - we will assume the responsibility for providing
 * notifications for registered events */
pmix_status_t pmix_server_register_events_fn(pmix_status_t *codes, size_t ncodes,
                                             const pmix_info_t info[], size_t ninfo,
                                             pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd;

    /* need to thread-shift this request as we are going
     * to access our global list of registered events */
    cd = OBJ_NEW(orte_pmix_server_op_caddy_t);
    cd->codes = codes;
    cd->ncodes = ncodes;
    cd->info = (pmix_info_t*)info;
    cd->ninfo = ninfo;
    cd->cbfunc = cbfunc;
    cd->cbdata = cbdata;
    opal_event_set(orte_event_base, &(cd->ev), -1,
                   OPAL_EV_WRITE, _register_events, cd);
    opal_event_set_priority(&(cd->ev), ORTE_MSG_PRI);
    ORTE_POST_OBJECT(cd);
    opal_event_active(&(cd->ev), OPAL_EV_WRITE, 1);
    return PMIX_SUCCESS;
}

static void _deregister_events(int sd, short args, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd = (orte_pmix_server_op_caddy_t*)cbdata;

    ORTE_ACQUIRE_OBJECT(cd);

    /* need to implement this */
    if (NULL != cd->cbfunc) {
        cd->cbfunc(ORTE_SUCCESS, cd->cbdata);
    }
    OBJ_RELEASE(cd);
}
/* hook for the local PMIX server to pass event deregistrations
 * up to us */
pmix_status_t pmix_server_deregister_events_fn(pmix_status_t *codes, size_t ncodes,
                                               pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd;

    /* need to thread-shift this request as we are going
     * to access our global list of registered events */
    cd = OBJ_NEW(orte_pmix_server_op_caddy_t);
    cd->codes = codes;
    cd->ncodes = ncodes;
    cd->cbfunc = cbfunc;
    cd->cbdata = cbdata;
    opal_event_set(orte_event_base, &(cd->ev), -1,
                   OPAL_EV_WRITE, _deregister_events, cd);
    opal_event_set_priority(&(cd->ev), ORTE_MSG_PRI);
    ORTE_POST_OBJECT(cd);
    opal_event_active(&(cd->ev), OPAL_EV_WRITE, 1);
    return ORTE_SUCCESS;
}

static void _notify_release(int status, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd = (orte_pmix_server_op_caddy_t*)cbdata;

    ORTE_ACQUIRE_OBJECT(cd);

    if (NULL != cd->info) {
        PMIX_INFO_FREE(cd->info, cd->ninfo);
    }
    OBJ_RELEASE(cd);
}

/* someone has sent us an event that we need to distribute
 * to our local clients */
void pmix_server_notify(int status, orte_process_name_t* sender,
                        opal_buffer_t *buffer,
                        orte_rml_tag_t tg, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd;
    int cnt, rc;
    pmix_proc_t source, psender;
    opal_byte_object_t *boptr;
    pmix_data_buffer_t pbkt;
    pmix_data_range_t range = PMIX_RANGE_SESSION;
    pmix_status_t code, ret;
    size_t ninfo;

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s PRRTE Notification received from %s",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME),
                        ORTE_NAME_PRINT(sender));

    /* unpack the byte object payload */
    cnt = 1;
    if (ORTE_SUCCESS != (rc = opal_dss.unpack(buffer, &boptr, &cnt, OPAL_BYTE_OBJECT))) {
        ORTE_ERROR_LOG(rc);
        return;
    }

    /* load it into a pmix data buffer for processing */
    PMIX_DATA_BUFFER_LOAD(&pbkt, boptr->bytes, boptr->size);
    free(boptr);

    /* convert the sender */
    OPAL_PMIX_CONVERT_NAME(&psender, sender);

    /* unpack the status code */
    cnt = 1;
    if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, &code, &cnt, PMIX_STATUS))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        return;
    }

    /* unpack the source */
    cnt = 1;
    if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, &source, &cnt, PMIX_PROC))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        return;
    }

    /* unpack the range */
    cnt = 1;
    if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, &range, &cnt, PMIX_DATA_RANGE))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        return;
    }

    cd = OBJ_NEW(orte_pmix_server_op_caddy_t);

    /* unpack the #infos that were provided */
    cnt = 1;
    if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, &cd->ninfo, &cnt, PMIX_SIZE))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        OBJ_RELEASE(cd);
        return;
    }
    /* reserve a spot for an additional flag */
    ninfo = cd->ninfo + 1;
    /* create the space */
    PMIX_INFO_CREATE(cd->info, ninfo);

    if (0 < cd->ninfo) {
        /* unpack into it */
        cnt = cd->ninfo;
        if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, cd->info, &cnt, PMIX_INFO))) {
            PMIX_ERROR_LOG(ret);
            PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
            PMIX_INFO_FREE(cd->info, cd->ninfo);
            OBJ_RELEASE(cd);
            return;
        }
    }
    cd->ninfo = ninfo;
    PMIX_DATA_BUFFER_DESTRUCT(&pbkt);

    /* protect against infinite loops by marking that this notification was
     * passed down to the server by me */
    PMIX_INFO_LOAD(&cd->info[ninfo-1], "orte.notify.donotloop", NULL, PMIX_BOOL);

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s NOTIFYING PMIX SERVER OF STATUS %s SOURCE %s RANGE %s",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME), PMIx_Error_string(code), source.nspace, PMIx_Data_range_string(range));

    ret = PMIx_Notify_event(code, &source, range, cd->info, cd->ninfo, _notify_release, cd);
    if (PMIX_SUCCESS != ret) {
        if (PMIX_OPERATION_SUCCEEDED != ret) {
            PMIX_ERROR_LOG(ret);
        }
        if (NULL != cd->info) {
            PMIX_INFO_FREE(cd->info, cd->ninfo);
        }
        OBJ_RELEASE(cd);
    }
}

pmix_status_t pmix_server_notify_event(pmix_status_t code,
                                       const pmix_proc_t *source,
                                       pmix_data_range_t range,
                                       pmix_info_t info[], size_t ninfo,
                                       pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    opal_buffer_t *buf;
    int rc;
    orte_grpcomm_signature_t *sig;
    opal_byte_object_t *boptr, bo;
    pmix_byte_object_t pbo;
    pmix_data_buffer_t pbkt;
    pmix_proc_t psender;
    pmix_status_t ret;
    size_t n;

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s local process %s:%d generated event code %d range %s",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME),
                        source->nspace, source->rank, code, PMIx_Data_range_string(range));

    /* check to see if this is one we sent down */
    for (n=0; n < ninfo; n++) {
        if (0 == strcmp(info[n].key, "orte.notify.donotloop")) {
            /* yep - do not process */
            goto done;
        }
    }

    /* a local process has generated an event - we need to xcast it
     * to all the daemons so it can be passed down to their local
     * procs */
    PMIX_DATA_BUFFER_CONSTRUCT(&pbkt);
    /* convert the sender */
    OPAL_PMIX_CONVERT_NAME(&psender, ORTE_PROC_MY_NAME);

    /* pack the status code */
    if (PMIX_SUCCESS != (ret = PMIx_Data_pack(&psender, &pbkt, &code, 1, PMIX_STATUS))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        return ret;
    }
    /* pack the source */
    if (PMIX_SUCCESS != (ret = PMIx_Data_pack(&psender, &pbkt, (pmix_proc_t*)source, 1, PMIX_PROC))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        return ret;
    }
    /* pack the range */
    if (PMIX_SUCCESS != (ret = PMIx_Data_pack(&psender, &pbkt, &range, 1, PMIX_DATA_RANGE))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        return ret;
    }
    /* pack the number of infos */
    if (PMIX_SUCCESS != (ret = PMIx_Data_pack(&psender, &pbkt, &ninfo, 1, PMIX_SIZE))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        return ret;
    }
    if (0 < ninfo) {
        if (PMIX_SUCCESS != (ret = PMIx_Data_pack(&psender, &pbkt, info, ninfo, PMIX_INFO))) {
            PMIX_ERROR_LOG(ret);
            PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
            return ret;
        }
    }
    /* unload the pmix buffer */
    PMIX_DATA_BUFFER_UNLOAD(&pbkt, pbo.bytes, pbo.size);
    bo.bytes = (uint8_t*)pbo.bytes;
    bo.size = pbo.size;

    /* pack it into our reply */
    buf = OBJ_NEW(opal_buffer_t);
    boptr = &bo;
    if (ORTE_SUCCESS != (rc = opal_dss.pack(buf, &boptr, 1, OPAL_BYTE_OBJECT))) {
        ORTE_ERROR_LOG(rc);
        free(bo.bytes);
        OBJ_RELEASE(buf);
        return PMIX_ERR_PACK_FAILURE;
    }
    free(bo.bytes);

    /* goes to all daemons */
    sig = OBJ_NEW(orte_grpcomm_signature_t);
    if (NULL == sig) {
        OBJ_RELEASE(buf);
        return PMIX_ERR_NOMEM;
    }
    sig->signature = (orte_process_name_t*)malloc(sizeof(orte_process_name_t));
    if (NULL == sig->signature) {
        OBJ_RELEASE(buf);
        OBJ_RELEASE(sig);
        return PMIX_ERR_NOMEM;
    }
    sig->signature[0].jobid = ORTE_PROC_MY_NAME->jobid;
    sig->signature[0].vpid = ORTE_VPID_WILDCARD;
    sig->sz = 1;
    if (ORTE_SUCCESS != (rc = orte_grpcomm.xcast(sig, ORTE_RML_TAG_NOTIFICATION, buf))) {
        ORTE_ERROR_LOG(rc);
        OBJ_RELEASE(buf);
        OBJ_RELEASE(sig);
        return PMIX_ERROR;
    }
    OBJ_RELEASE(buf);
    /* maintain accounting */
    OBJ_RELEASE(sig);

  done:
    /* we do not need to execute a callback as we did this atomically */
    return PMIX_OPERATION_SUCCEEDED;
}

static void _toolconn(int sd, short args, void *cbdata)
{
    pmix_server_req_t *cd = (pmix_server_req_t*)cbdata;
    orte_job_t *jdata = NULL, *jptr;
    int rc, i;
    uint32_t u32;
    size_t n;
    pmix_proc_t pname;
    opal_buffer_t *buf;
    orte_plm_cmd_flag_t command = ORTE_PLM_ALLOC_JOBID_CMD;
    pmix_status_t xrc;
    void *nptr;

    ORTE_ACQUIRE_OBJECT(cd);

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s TOOL CONNECTION PROCESSING",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));

    /* check for directives */
    if (NULL != cd->info) {
        for (n=0; n < cd->ninfo; n++) {
            if (PMIX_CHECK_KEY(&cd->info[n], PMIX_EVENT_SILENT_TERMINATION)) {
                cd->flag = PMIX_INFO_TRUE(&cd->info[n]);
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_VERSION_INFO)) {
                /* we ignore this for now */
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_USERID)) {
                PMIX_VALUE_GET_NUMBER(xrc, &cd->info[n].value, cd->uid, uid_t);
                if (PMIX_SUCCESS != xrc) {
                    if (NULL != cd->toolcbfunc) {
                        cd->toolcbfunc(xrc, NULL, cd->cbdata);
                    }
                    OBJ_RELEASE(cd);
                    return;
                }
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_GRPID)) {
                PMIX_VALUE_GET_NUMBER(xrc, &cd->info[n].value, cd->gid, gid_t);
                if (PMIX_SUCCESS != xrc) {
                    if (NULL != cd->toolcbfunc) {
                        cd->toolcbfunc(xrc, NULL, cd->cbdata);
                    }
                    OBJ_RELEASE(cd);
                    return;
                }
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_NSPACE)) {
                 OPAL_PMIX_CONVERT_NSPACE(rc, &cd->target.jobid, cd->info[n].value.data.string);
                 if (ORTE_SUCCESS != rc) {
                    ORTE_ERROR_LOG(rc);
                 }
             } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_RANK)) {
                OPAL_PMIX_CONVERT_RANK(cd->target.vpid, cd->info[n].value.data.rank);
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_HOSTNAME)) {
                cd->operation = strdup(cd->info[n].value.data.string);
#ifdef PMIX_CMD_LINE
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_CMD_LINE)) {
                cd->cmdline = strdup(cd->info[n].value.data.string);
#endif
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_LAUNCHER)) {
                cd->launcher = PMIX_INFO_TRUE(&cd->info[n]);
            } else if (PMIX_CHECK_KEY(&cd->info[n], PMIX_PROC_PID)) {
                PMIX_VALUE_GET_NUMBER(xrc, &cd->info[n].value, cd->pid, pid_t);
                if (PMIX_SUCCESS != xrc) {
                    if (NULL != cd->toolcbfunc) {
                        cd->toolcbfunc(xrc, NULL, cd->cbdata);
                    }
                    OBJ_RELEASE(cd);
                    return;
                }
            }
        }
    }

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s TOOL CONNECTION FROM UID %d GID %d",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME), cd->uid, cd->gid);

    /* if we are not the HNP or master, and the tool doesn't
     * already have a name (i.e., we didn't spawn it), then
     * there is nothing we can currently do.
     * Eventually, when we switch to nspace instead of an
     * integer jobid, we'll just locally assign this value */
    if (ORTE_JOBID_INVALID == cd->target.jobid ||
        ORTE_VPID_INVALID == cd->target.vpid) {
       /* if we are the HNP, we can directly assign the jobid */
        if (ORTE_PROC_IS_HNP || ORTE_PROC_IS_MASTER) {
            jdata = OBJ_NEW(orte_job_t);
            rc = orte_plm_base_create_jobid(jdata);
            if (ORTE_SUCCESS != rc) {
                OBJ_RELEASE(jdata);
                if (NULL != cd->toolcbfunc) {
                    OPAL_PMIX_CONVERT_NAME(&pname, &cd->target);
                    cd->toolcbfunc(PMIX_ERROR, &pname, cd->cbdata);
                }
                OBJ_RELEASE(cd);
                return;
            }
            cd->target.jobid = jdata->jobid;
            cd->target.vpid = 0;
            orte_pmix_server_tool_conn_complete(jdata, cd);
        } else {
            if (OPAL_SUCCESS != (rc = opal_hotel_checkin(&orte_pmix_server_globals.reqs, cd, &cd->room_num))) {
                orte_show_help("help-orted.txt", "noroom", true, cd->operation, orte_pmix_server_globals.num_rooms);
                goto callback;
            }
            /* we need to send this to the HNP for a jobid */
            buf = OBJ_NEW(opal_buffer_t);
            opal_dss.pack(buf, &command, 1, ORTE_PLM_CMD);
            opal_dss.pack(buf, &cd->room_num, 1, OPAL_INT);
            /* send it to the HNP for processing - might be myself! */
            if (ORTE_SUCCESS != (rc = orte_rml.send_buffer_nb(ORTE_PROC_MY_HNP, buf,
                                                              ORTE_RML_TAG_PLM,
                                                              orte_rml_send_callback, NULL))) {
                ORTE_ERROR_LOG(rc);
                xrc = opal_pmix_convert_rc(rc);
                opal_hotel_checkout_and_return_occupant(&orte_pmix_server_globals.reqs, cd->room_num, (void**)&cd);
                OBJ_RELEASE(buf);
                if (NULL != cd->toolcbfunc) {
                    cd->toolcbfunc(xrc, NULL, cd->cbdata);
                }
                OBJ_RELEASE(cd);
            }
            return;
        }
    } else {
        /* we may have spawned this job, so check to see if we
         * already have a job object for it */
        jdata = NULL;
        i = opal_hash_table_get_first_key_uint32(orte_job_data, &u32, (void **)&jptr, &nptr);
        while (OPAL_SUCCESS == i) {
            if (cd->target.jobid == jptr->jobid) {
                jdata = jptr;
                /* flag that this job is a tool */
                ORTE_FLAG_SET(jdata, ORTE_JOB_FLAG_TOOL);
                break;
            }
            i = opal_hash_table_get_next_key_uint32(orte_job_data, &u32, (void **)&jptr, nptr, &nptr);
        }
        if (NULL == jdata) {
            jdata = OBJ_NEW(orte_job_t);
            jdata->jobid = cd->target.jobid;
            orte_pmix_server_tool_conn_complete(jdata, cd);
        }
    }
    rc = ORTE_SUCCESS;

  callback:
    if (NULL != cd->toolcbfunc) {
        OPAL_PMIX_CONVERT_NAME(&pname, &cd->target);
        xrc = opal_pmix_convert_rc(rc);
        cd->toolcbfunc(xrc, &pname, cd->cbdata);
    }
    OBJ_RELEASE(cd);
}

void orte_pmix_server_tool_conn_complete(orte_job_t *jdata,
                                         pmix_server_req_t *req)
{
    orte_app_context_t *app;
    orte_proc_t *proc;
    orte_node_t *node, *nptr;
    int i;

    /* flag that this job is a tool */
    ORTE_FLAG_SET(jdata, ORTE_JOB_FLAG_TOOL);
    if (req->launcher) {
        /* flag that it is also a launcher */
        ORTE_FLAG_SET(jdata, ORTE_JOB_FLAG_LAUNCHER);
    }
    /* store it away */
    opal_hash_table_set_value_uint32(orte_job_data, jdata->jobid, jdata);

    /* must create a map for it (even though it has no
     * info in it) so that the job info will be picked
     * up in subsequent pidmaps or other daemons won't
     * know how to route
     */
    jdata->map = OBJ_NEW(orte_job_map_t);

    /* setup an app_context for the singleton */
    app = OBJ_NEW(orte_app_context_t);
    if (NULL == req->cmdline) {
        app->app = strdup("tool");
        opal_argv_append_nosize(&app->argv, "tool");
    } else {
        app->argv = opal_argv_split(req->cmdline, ' ');
        app->app = strdup(app->argv[0]);
    }
    app->num_procs = 1;
    opal_pointer_array_add(jdata->apps, app);
    jdata->num_apps = 1;

    /* setup a proc object for the singleton - since we
     * -must- be the HNP, and therefore we stored our
     * node on the global node pool, and since the singleton
     * -must- be on the same node as us, indicate that
     */
    proc = OBJ_NEW(orte_proc_t);
    proc->name.jobid = jdata->jobid;
    proc->name.vpid = req->target.vpid;
    proc->pid = req->pid;
    proc->parent = ORTE_PROC_MY_NAME->vpid;
    ORTE_FLAG_SET(proc, ORTE_PROC_FLAG_ALIVE);
    ORTE_FLAG_SET(proc, ORTE_PROC_FLAG_TOOL);
    proc->state = ORTE_PROC_STATE_RUNNING;
    /* set the trivial */
    proc->local_rank = 0;
    proc->node_rank = 0;
    proc->app_rank = 0;
    proc->app_idx = 0;
    if (NULL == req->operation) {
        /* it is on my node */
        node = (orte_node_t*)opal_pointer_array_get_item(orte_node_pool, 0);
        ORTE_FLAG_SET(proc, ORTE_PROC_FLAG_LOCAL);
    } else {
        /* we need to locate it */
        node = NULL;
        for (i=0; i < orte_node_pool->size; i++) {
            if (NULL == (nptr = (orte_node_t*)opal_pointer_array_get_item(orte_node_pool, i))) {
                continue;
            }
            if (0 == strcmp(req->operation, nptr->name)) {
                node = nptr;
                break;
            }
        }
        if (NULL == node) {
            /* not in our allocation - which is still okay */
            node = OBJ_NEW(orte_node_t);
            node->name = strdup(req->operation);
            ORTE_FLAG_SET(node, ORTE_NODE_NON_USABLE);
            opal_pointer_array_add(orte_node_pool, node);
        }
    }
    proc->node = node;
    OBJ_RETAIN(node);  /* keep accounting straight */
    opal_pointer_array_add(jdata->procs, proc);
    jdata->num_procs = 1;
    /* add the node to the job map */
    OBJ_RETAIN(node);
    opal_pointer_array_add(jdata->map->nodes, node);
    jdata->map->num_nodes++;
    /* and it obviously is on the node - note that
     * we do _not_ increment the #procs on the node
     * as the tool doesn't count against the slot
     * allocation */
    OBJ_RETAIN(proc);
    opal_pointer_array_add(node->procs, proc);
    /* if they indicated a preference for termination, set it */
    if (req->flag) {
        orte_set_attribute(&jdata->attributes, ORTE_JOB_SILENT_TERMINATION,
                           ORTE_ATTR_GLOBAL, NULL, OPAL_BOOL);
    }
}

void pmix_tool_connected_fn(pmix_info_t *info, size_t ninfo,
                            pmix_tool_connection_cbfunc_t cbfunc,
                            void *cbdata)
{
    pmix_server_req_t *cd;

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s TOOL CONNECTION REQUEST RECVD",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));

    /* need to threadshift this request */
    cd = OBJ_NEW(pmix_server_req_t);
    cd->info = info;
    cd->ninfo = ninfo;
    cd->toolcbfunc = cbfunc;
    cd->cbdata = cbdata;

    opal_event_set(orte_event_base, &(cd->ev), -1,
                   OPAL_EV_WRITE, _toolconn, cd);
    opal_event_set_priority(&(cd->ev), ORTE_MSG_PRI);
    ORTE_POST_OBJECT(cd);
    opal_event_active(&(cd->ev), OPAL_EV_WRITE, 1);

}

static void lgcbfn(int sd, short args, void *cbdata)
{
    orte_pmix_server_op_caddy_t *cd = (orte_pmix_server_op_caddy_t*)cbdata;

    if (NULL != cd->cbfunc) {
        cd->cbfunc(cd->status, cd->cbdata);
    }
    OBJ_RELEASE(cd);
}

void pmix_server_log_fn(const pmix_proc_t *client,
                        const pmix_info_t data[], size_t ndata,
                        const pmix_info_t directives[], size_t ndirs,
                        pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    size_t n, cnt;
    opal_buffer_t *buf;
    opal_byte_object_t bo, *boptr;
    int rc = ORTE_SUCCESS;
    pmix_data_buffer_t pbuf;
    pmix_byte_object_t pbo;
    pmix_proc_t psender;
    pmix_status_t ret;

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s logging info",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));

    /* if we are the one that passed it down, then we don't pass it back */
    for (n=0; n < ndirs; n++) {
        if (PMIX_CHECK_KEY(&directives[n], "orte.log.noloop")) {
            if (PMIX_INFO_TRUE(&directives[n])) {
                rc = PMIX_SUCCESS;
                goto done;
            }
        }
    }

    PMIX_DATA_BUFFER_CONSTRUCT(&pbuf);
    OPAL_PMIX_CONVERT_NAME(&psender, ORTE_PROC_MY_NAME);
    cnt = 0;

    for (n=0; n < ndata; n++) {
        if (0 == strncmp(data[n].key, ORTE_PMIX_SHOW_HELP, PMIX_MAX_KEYLEN)) {
            /* pull out the blob */
            if (PMIX_BYTE_OBJECT != data[n].value.type) {
                continue;
            }
            buf = OBJ_NEW(opal_buffer_t);
            opal_dss.load(buf, data[n].value.data.bo.bytes, data[n].value.data.bo.size);
            if (ORTE_SUCCESS != (rc = orte_rml.send_buffer_nb(ORTE_PROC_MY_HNP, buf,
                                                              ORTE_RML_TAG_SHOW_HELP,
                                                              orte_rml_send_callback, NULL))) {
                ORTE_ERROR_LOG(rc);
                buf->base_ptr = NULL;
                OBJ_RELEASE(buf);
            }
        } else {
            /* ship this to our HNP/MASTER for processing, even if that is us */
            ret = PMIx_Data_pack(&psender, &pbuf, (pmix_info_t*)&data[n], 1, PMIX_INFO);
            if (PMIX_SUCCESS != ret) {
                PMIX_ERROR_LOG(ret);
            }
            ++cnt;
        }
    }
    if (0 < cnt) {
        buf = OBJ_NEW(opal_buffer_t);
        opal_dss.pack(buf, &cnt, 1, OPAL_SIZE);
        PMIX_DATA_BUFFER_UNLOAD(&pbuf, pbo.bytes, pbo.size);
        bo.bytes = (uint8_t*)pbo.bytes;
        bo.size = pbo.size;
        boptr = &bo;
        opal_dss.pack(buf, &boptr, 1, OPAL_BYTE_OBJECT);
        free(bo.bytes);
        rc = orte_rml.send_buffer_nb(ORTE_PROC_MY_HNP, buf,
                                     ORTE_RML_TAG_LOGGING,
                                     orte_rml_send_callback, NULL);
        if (ORTE_SUCCESS != rc) {
            ORTE_ERROR_LOG(rc);
            OBJ_RELEASE(buf);
        }
    }

  done:
    /* we cannot directly execute the callback here
     * as it would threadlock - so shift to somewhere
     * safe */
    ORTE_PMIX_THREADSHIFT(ORTE_NAME_WILDCARD, NULL, rc,
                          NULL, NULL, 0, lgcbfn,
                          cbfunc, cbdata);
}

pmix_status_t pmix_server_job_ctrl_fn(const pmix_proc_t *requestor,
                                      const pmix_proc_t targets[], size_t ntargets,
                                      const pmix_info_t directives[], size_t ndirs,
                                      pmix_info_cbfunc_t cbfunc, void *cbdata)
{
    int rc, j;
    size_t m, n;
    orte_proc_t *proc;
    opal_pointer_array_t parray, *ptrarray;
    orte_process_name_t name;
    opal_buffer_t *cmd;
    orte_daemon_cmd_flag_t cmmnd = ORTE_DAEMON_HALT_VM_CMD;
    orte_grpcomm_signature_t *sig;

    opal_output_verbose(2, orte_pmix_server_globals.output,
                        "%s job control request from %s:%d",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME),
                        requestor->nspace, requestor->rank);

    for (m=0; m < ndirs; m++) {
        if (0 == strncmp(directives[m].key, PMIX_JOB_CTRL_KILL, PMIX_MAX_KEYLEN)) {
            /* convert the list of targets to a pointer array */
            if (NULL == targets) {
                ptrarray = NULL;
            } else {
                OBJ_CONSTRUCT(&parray, opal_pointer_array_t);
                for (n=0; n < ntargets; n++) {
                    OPAL_PMIX_CONVERT_PROCT(rc, &name, &targets[n]);
                    if (OPAL_SUCCESS != rc) {
                        ORTE_ERROR_LOG(rc);
                        return PMIX_ERR_BAD_PARAM;
                    }
                    if (ORTE_VPID_WILDCARD == name.vpid) {
                        /* create an object */
                        proc = OBJ_NEW(orte_proc_t);
                        proc->name.jobid = name.jobid;
                        proc->name.vpid = ORTE_VPID_WILDCARD;
                    } else {
                        /* get the proc object for this proc */
                        if (NULL == (proc = orte_get_proc_object(&name))) {
                            ORTE_ERROR_LOG(ORTE_ERR_NOT_FOUND);
                            continue;
                        }
                        OBJ_RETAIN(proc);
                    }
                    opal_pointer_array_add(&parray, proc);
                }
                ptrarray = &parray;
            }
            if (ORTE_SUCCESS != (rc = orte_plm.terminate_procs(ptrarray))) {
                ORTE_ERROR_LOG(rc);
            }
            if (NULL != ptrarray) {
                /* cleanup the array */
                for (j=0; j < parray.size; j++) {
                    if (NULL != (proc = (orte_proc_t*)opal_pointer_array_get_item(&parray, j))) {
                        OBJ_RELEASE(proc);
                    }
                }
                OBJ_DESTRUCT(&parray);
            }
            continue;
        } else if (0 == strncmp(directives[m].key, PMIX_JOB_CTRL_TERMINATE, PMIX_MAX_KEYLEN)) {
            if (NULL == targets) {
                /* terminate the daemons and all running jobs */
                cmd = OBJ_NEW(opal_buffer_t);
                /* pack the command */
                if (ORTE_SUCCESS != (rc = opal_dss.pack(cmd, &cmmnd, 1, ORTE_DAEMON_CMD))) {
                    ORTE_ERROR_LOG(rc);
                    OBJ_RELEASE(cmd);
                    return rc;
                }
                /* goes to all daemons */
                sig = OBJ_NEW(orte_grpcomm_signature_t);
                sig->signature = (orte_process_name_t*)malloc(sizeof(orte_process_name_t));
                sig->signature[0].jobid = ORTE_PROC_MY_NAME->jobid;
                sig->signature[0].vpid = ORTE_VPID_WILDCARD;
                if (ORTE_SUCCESS != (rc = orte_grpcomm.xcast(sig, ORTE_RML_TAG_DAEMON, cmd))) {
                    ORTE_ERROR_LOG(rc);
                }
                OBJ_RELEASE(cmd);
                OBJ_RELEASE(sig);
            }
        }
    }

    return PMIX_OPERATION_SUCCEEDED;
}

#if PMIX_NUMERIC_VERSION >= 0x00040000
static void relcb(void *cbdata)
{
    orte_pmix_mdx_caddy_t *cd=(orte_pmix_mdx_caddy_t*)cbdata;

    if (NULL != cd->info) {
        PMIX_INFO_FREE(cd->info, cd->ninfo);
    }
    OBJ_RELEASE(cd);
}
static void group_release(int status, opal_buffer_t *buf, void *cbdata)
{
    orte_pmix_mdx_caddy_t *cd = (orte_pmix_mdx_caddy_t*)cbdata;
    int32_t cnt;
    int rc;
    pmix_status_t ret;
    size_t cid, n;
    pmix_byte_object_t bo;
    int32_t byused;

    ORTE_ACQUIRE_OBJECT(cd);

    if (ORTE_SUCCESS != status) {
        rc = status;
        goto complete;
    }

    if (1 == cd->mode) {
        /* a context id was requested, get it */
        cnt = 1;
        rc = opal_dss.unpack(buf, &cid, &cnt, OPAL_SIZE);
        /* error if they didn't return it */
        if (ORTE_SUCCESS != rc) {
            ORTE_ERROR_LOG(rc);
            goto complete;
        }
        cd->ninfo++;
    }
    /* if anything is left in the buffer, then it is
     * modex data that needs to be stored */
    PMIX_BYTE_OBJECT_CONSTRUCT(&bo);
    byused = buf->bytes_used - (buf->unpack_ptr - buf->base_ptr);
    if (0 < byused) {
        bo.bytes = buf->unpack_ptr;
        bo.size = byused;
    }
    if (NULL != bo.bytes && 0 < bo.size) {
        cd->ninfo++;
    }

    if (0 < cd->ninfo) {
        PMIX_INFO_CREATE(cd->info, cd->ninfo);
        n = 0;
        if (1 == cd->mode) {
            PMIX_INFO_LOAD(&cd->info[n], PMIX_GROUP_CONTEXT_ID, &cid, PMIX_SIZE);
            ++n;
        }
        if (NULL != bo.bytes && 0 < bo.size) {
            PMIX_INFO_LOAD(&cd->info[n], PMIX_GROUP_ENDPT_DATA, &bo, PMIX_BYTE_OBJECT);
        }
    }

  complete:
    ret = opal_pmix_convert_rc(rc);
    /* return to the local procs in the collective */
    if (NULL != cd->infocbfunc) {
        cd->infocbfunc(ret, cd->info, cd->ninfo, cd->cbdata, relcb, cd);
    } else {
        if (NULL != cd->info) {
            PMIX_INFO_FREE(cd->info, cd->ninfo);
        }
        OBJ_RELEASE(cd);
    }
}

pmix_status_t pmix_server_group_fn(pmix_group_operation_t op, char *gpid,
                                   const pmix_proc_t procs[], size_t nprocs,
                                   const pmix_info_t directives[], size_t ndirs,
                                   pmix_info_cbfunc_t cbfunc, void *cbdata)
{
    orte_pmix_mdx_caddy_t *cd;
    int rc;
    size_t i, mode = 0;
    pmix_server_pset_t *pset;
    bool fence = false;
#if PMIX_NUMERIC_VERSION >= 0x00040000
    opal_buffer_t bf;
    pmix_byte_object_t *bo = NULL;
#endif

    /* they are required to pass us an id */
    if (NULL == gpid) {
        return PMIX_ERR_BAD_PARAM;
    }

    /* check the directives */
    for (i=0; i < ndirs; i++) {
        /* see if they want a context id assigned */
        if (PMIX_CHECK_KEY(&directives[i], PMIX_GROUP_ASSIGN_CONTEXT_ID)) {
            if (PMIX_INFO_TRUE(&directives[i])) {
                mode = 1;
            }
        } else if (PMIX_CHECK_KEY(&directives[i], PMIX_EMBED_BARRIER)) {
            fence = PMIX_INFO_TRUE(&directives[i]);
#if PMIX_NUMERIC_VERSION >= 0x00040000
        } else if (PMIX_CHECK_KEY(&directives[i], PMIX_GROUP_ENDPT_DATA)) {
            bo = (pmix_byte_object_t*)&directives[i].value.data.bo;
#endif
        }
    }

    if (PMIX_GROUP_CONSTRUCT == op) {
        /* add it to our list of known process sets */
        pset = OBJ_NEW(pmix_server_pset_t);
        pset->name = strdup(gpid);
        pset->num_members = nprocs;
        PMIX_PROC_CREATE(pset->members, pset->num_members);
        memcpy(pset->members, procs, nprocs * sizeof(pmix_proc_t));
        opal_list_append(&orte_pmix_server_globals.psets, &pset->super);
    } else if (PMIX_GROUP_DESTRUCT == op) {
        /* find this process set on our list of groups */
        OPAL_LIST_FOREACH(pset, &orte_pmix_server_globals.psets, pmix_server_pset_t) {
            if (0 == strcmp(pset->name, gpid)) {
                opal_list_remove_item(&orte_pmix_server_globals.psets, &pset->super);
                OBJ_RELEASE(pset);
                break;
            }
        }
    }

    /* if they don't want us to do a fence and they don't want a
     * context id assigned, then we are done */
    if (!fence && 0 == mode) {
        return PMIX_OPERATION_SUCCEEDED;
    }

    cd = OBJ_NEW(orte_pmix_mdx_caddy_t);
    cd->infocbfunc = cbfunc;
    cd->cbdata = cbdata;
    cd->mode = mode;

   /* compute the signature of this collective */
    if (NULL != procs) {
        cd->sig = OBJ_NEW(orte_grpcomm_signature_t);
        cd->sig->sz = nprocs;
        cd->sig->signature = (orte_process_name_t*)malloc(cd->sig->sz * sizeof(orte_process_name_t));
        memset(cd->sig->signature, 0, cd->sig->sz * sizeof(orte_process_name_t));
        for (i=0; i < nprocs; i++) {
            OPAL_PMIX_CONVERT_PROCT(rc, &cd->sig->signature[i], &procs[i]);
            if (OPAL_SUCCESS != rc) {
                OPAL_ERROR_LOG(rc);
                OBJ_RELEASE(cd);
                return PMIX_ERR_BAD_PARAM;
            }
        }
    }
    cd->buf = OBJ_NEW(opal_buffer_t);
#if PMIX_NUMERIC_VERSION >= 0x00040000
    /* if they provided us with a data blob, send it along */
    if (NULL != bo) {
        /* We don't own the byte_object and so we have to
         * copy it here */
        OBJ_CONSTRUCT(&bf, opal_buffer_t);
        opal_dss.load(&bf, bo->bytes, bo->size);
        opal_dss.copy_payload(cd->buf, &bf);
        /* don't destruct bf! */
    }
#endif
    /* pass it to the global collective algorithm */
    if (ORTE_SUCCESS != (rc = orte_grpcomm.allgather(cd->sig, cd->buf, mode,
                                                     group_release, cd))) {
        ORTE_ERROR_LOG(rc);
        OBJ_RELEASE(cd);
        return PMIX_ERROR;
    }
    return PMIX_SUCCESS;
}
#endif

pmix_status_t pmix_server_iof_pull_fn(const pmix_proc_t procs[], size_t nprocs,
                                      const pmix_info_t directives[], size_t ndirs,
                                      pmix_iof_channel_t channels,
                                      pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    return PMIX_ERR_NOT_SUPPORTED;
}

pmix_status_t pmix_server_stdin_fn(const pmix_proc_t *source,
                                   const pmix_proc_t targets[], size_t ntargets,
                                   const pmix_info_t directives[], size_t ndirs,
                                   const pmix_byte_object_t *bo,
                                   pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    orte_process_name_t dst;
    int rc;

    OPAL_PMIX_CONVERT_PROCT(rc, &dst, &targets[0]);
    orte_iof.push_stdin(&dst, (uint8_t*)bo->bytes, bo->size);

#if PMIX_NUMERIC_VERSION >= 0x00040000
    if (NULL == bo->bytes || 0 == bo->size) {
        return PMIX_ERR_IOF_COMPLETE;
    }
#endif

    return PMIX_OPERATION_SUCCEEDED;
}
