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
 * Copyright (c) 2014      Mellanox Technologies, Inc.
 *                         All rights reserved.
 * Copyright (c) 2014-2016 Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
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
#include "opal/pmix/pmix-internal.h"

#include "orte/mca/errmgr/errmgr.h"
#include "orte/util/name_fns.h"
#include "orte/util/show_help.h"
#include "orte/util/threads.h"
#include "orte/runtime/orte_data_server.h"
#include "orte/runtime/orte_globals.h"
#include "orte/mca/rml/rml.h"
#include "orte/mca/rml/base/rml_contact.h"

#include "pmix_server_internal.h"

static int init_server(void)
{
    char *server;
    pmix_proc_t proc;
    pmix_value_t val;
    char input[1024], *filename;
    FILE *fp;
    int rc;
    pmix_status_t ret;

    /* only do this once */
    orte_pmix_server_globals.pubsub_init = true;

    /* if the universal server wasn't specified, then we use
     * our own HNP for that purpose */
    if (NULL == orte_data_server_uri) {
        orte_pmix_server_globals.server = *ORTE_PROC_MY_HNP;
    } else {
        if (0 == strncmp(orte_data_server_uri, "file", strlen("file")) ||
            0 == strncmp(orte_data_server_uri, "FILE", strlen("FILE"))) {
            /* it is a file - get the filename */
            filename = strchr(orte_data_server_uri, ':');
            if (NULL == filename) {
                /* filename is not correctly formatted */
                orte_show_help("help-orterun.txt", "orterun:ompi-server-filename-bad", true,
                               orte_basename, orte_data_server_uri);
                return ORTE_ERR_BAD_PARAM;
            }
            ++filename; /* space past the : */

            if (0 >= strlen(filename)) {
                /* they forgot to give us the name! */
                orte_show_help("help-orterun.txt", "orterun:ompi-server-filename-missing", true,
                               orte_basename, orte_data_server_uri);
                return ORTE_ERR_BAD_PARAM;
            }

            /* open the file and extract the uri */
            fp = fopen(filename, "r");
            if (NULL == fp) { /* can't find or read file! */
                orte_show_help("help-orterun.txt", "orterun:ompi-server-filename-access", true,
                               orte_basename, orte_data_server_uri);
                return ORTE_ERR_BAD_PARAM;
            }
            if (NULL == fgets(input, 1024, fp)) {
                /* something malformed about file */
                fclose(fp);
                orte_show_help("help-orterun.txt", "orterun:ompi-server-file-bad", true,
                               orte_basename, orte_data_server_uri,
                               orte_basename);
                return ORTE_ERR_BAD_PARAM;
            }
            fclose(fp);
            input[strlen(input)-1] = '\0';  /* remove newline */
            server = strdup(input);
        } else {
            server = strdup(orte_data_server_uri);
        }
        /* parse the URI to get the server's name */
        if (ORTE_SUCCESS != (rc = orte_rml_base_parse_uris(server, &orte_pmix_server_globals.server, NULL))) {
            ORTE_ERROR_LOG(rc);
            free(server);
            return rc;
        }
        /* setup our route to the server */
        OPAL_PMIX_CONVERT_NAME(&proc, &orte_pmix_server_globals.server);
        PMIX_VALUE_LOAD(&val, server, PMIX_STRING);
        if (PMIX_SUCCESS != (ret = PMIx_Store_internal(&proc, PMIX_PROC_URI, &val))) {
            PMIX_ERROR_LOG(ret);
            PMIX_VALUE_DESTRUCT(&val);
            return rc;
        }
        PMIX_VALUE_DESTRUCT(&val);

        /* check if we are to wait for the server to start - resolves
         * a race condition that can occur when the server is run
         * as a background job - e.g., in scripts
         */
        if (orte_pmix_server_globals.wait_for_server) {
            /* ping the server */
            struct timeval timeout;
            timeout.tv_sec = orte_pmix_server_globals.timeout;
            timeout.tv_usec = 0;
            if (ORTE_SUCCESS != (rc = orte_rml.ping(server, &timeout))) {
                /* try it one more time */
                if (ORTE_SUCCESS != (rc = orte_rml.ping(server, &timeout))) {
                    /* okay give up */
                    orte_show_help("help-orterun.txt", "orterun:server-not-found", true,
                                   orte_basename, server,
                                   (long)orte_pmix_server_globals.timeout,
                                   ORTE_ERROR_NAME(rc));
                    ORTE_UPDATE_EXIT_STATUS(ORTE_ERROR_DEFAULT_EXIT_CODE);
                    return rc;
                }
            }
        }
    }

    return ORTE_SUCCESS;
}

static void execute(int sd, short args, void *cbdata)
{
    pmix_server_req_t *req = (pmix_server_req_t*)cbdata;
    int rc;
    opal_buffer_t *xfer;
    orte_process_name_t *target;

    ORTE_ACQUIRE_OBJECT(req);

    if (!orte_pmix_server_globals.pubsub_init) {
        /* we need to initialize our connection to the server */
        if (ORTE_SUCCESS != (rc = init_server())) {
            orte_show_help("help-orted.txt", "noserver", true,
                           (NULL == orte_data_server_uri) ?
                           "NULL" : orte_data_server_uri);
            goto callback;
        }
    }

    /* add this request to our tracker hotel */
    if (OPAL_SUCCESS != (rc = opal_hotel_checkin(&orte_pmix_server_globals.reqs, req, &req->room_num))) {
        orte_show_help("help-orted.txt", "noroom", true, req->operation, orte_pmix_server_globals.num_rooms);
        goto callback;
    }

    /* setup the xfer */
    xfer = OBJ_NEW(opal_buffer_t);
    /* pack the room number */
    if (OPAL_SUCCESS != (rc = opal_dss.pack(xfer, &req->room_num, 1, OPAL_INT))) {
        ORTE_ERROR_LOG(rc);
        OBJ_RELEASE(xfer);
        goto callback;
    }
    opal_dss.copy_payload(xfer, &req->msg);

    /* if the range is SESSION, then set the target to the global server */
    if (PMIX_RANGE_SESSION == req->range) {
        opal_output_verbose(1, orte_pmix_server_globals.output,
                            "%s orted:pmix:server range SESSION",
                            ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));
        target = &orte_pmix_server_globals.server;
    } else if (PMIX_RANGE_LOCAL == req->range) {
        /* if the range is local, send it to myself */
        opal_output_verbose(1, orte_pmix_server_globals.output,
                            "%s orted:pmix:server range LOCAL",
                            ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));
        target = ORTE_PROC_MY_NAME;
    } else {
        opal_output_verbose(1, orte_pmix_server_globals.output,
                            "%s orted:pmix:server range GLOBAL",
                            ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));
        target = ORTE_PROC_MY_HNP;
    }

    /* send the request to the target */
    rc = orte_rml.send_buffer_nb(target, xfer,
                                 ORTE_RML_TAG_DATA_SERVER,
                                 orte_rml_send_callback, NULL);
    if (ORTE_SUCCESS == rc) {
        return;
    }

  callback:
    /* execute the callback to avoid having the client hang */
    if (NULL != req->opcbfunc) {
        req->opcbfunc(rc, req->cbdata);
    } else if (NULL != req->lkcbfunc) {
        req->lkcbfunc(rc, NULL, 0, req->cbdata);
    }
    opal_hotel_checkout(&orte_pmix_server_globals.reqs, req->room_num);
    OBJ_RELEASE(req);
}

pmix_status_t pmix_server_publish_fn(const pmix_proc_t *proc,
                                     const pmix_info_t info[], size_t ninfo,
                                     pmix_op_cbfunc_t cbfunc, void *cbdata){
    pmix_server_req_t *req;
    pmix_status_t rc;
    int ret;
    uint8_t cmd = ORTE_PMIX_PUBLISH_CMD;
    pmix_data_buffer_t pbkt;
    pmix_byte_object_t pbo;
    opal_byte_object_t bo, *boptr;
    pmix_proc_t psender;
    size_t n;

    opal_output_verbose(1, orte_pmix_server_globals.output,
                        "%s orted:pmix:server PUBLISH",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));

    /* create the caddy */
    req = OBJ_NEW(pmix_server_req_t);
    opal_asprintf(&req->operation, "PUBLISH: %s:%d", __FILE__, __LINE__);
    req->opcbfunc = cbfunc;
    req->cbdata = cbdata;

    /* load the command */
    if (OPAL_SUCCESS != (ret = opal_dss.pack(&req->msg, &cmd, 1, OPAL_UINT8))) {
        ORTE_ERROR_LOG(ret);
        OBJ_RELEASE(req);
        return PMIX_ERR_PACK_FAILURE;
    }

    /* no help for it - need to search for range/persistence */
    for(n=0; n < ninfo; n++) {
        if (0 == strncmp(info[n].key, PMIX_RANGE, PMIX_MAX_KEYLEN)) {
            req->range = info[n].value.data.range;
        } else if (0 == strncmp(info[n].key, PMIX_TIMEOUT, PMIX_MAX_KEYLEN)) {
            req->timeout = info[n].value.data.integer;
        }
    }

    /* setup a pmix_data_buffer_t to hold the message */
    PMIX_DATA_BUFFER_CONSTRUCT(&pbkt);
    /* I will be sending it */
    OPAL_PMIX_CONVERT_NAME(&psender, ORTE_PROC_MY_NAME);

    /* pack the name of the publisher */
    if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, (pmix_proc_t*)proc, 1, PMIX_PROC))) {
        PMIX_ERROR_LOG(rc);
        OBJ_RELEASE(req);
        return rc;
    }

    /* pack the number of infos */
    if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, &ninfo, 1, PMIX_SIZE))) {
        PMIX_ERROR_LOG(rc);
        OBJ_RELEASE(req);
        return rc;
    }

    /* pack the infos */
    if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, (pmix_info_t*)info, ninfo, PMIX_INFO))) {
        PMIX_ERROR_LOG(rc);
        OBJ_RELEASE(req);
        return rc;
    }

    /* unload the pmix buffer */
    PMIX_DATA_BUFFER_UNLOAD(&pbkt, pbo.bytes, pbo.size);
    bo.bytes = (uint8_t*)pbo.bytes;
    bo.size = pbo.size;

    /* pack it into our msg */
    boptr = &bo;
    if (ORTE_SUCCESS != (ret = opal_dss.pack(&req->msg, &boptr, 1, OPAL_BYTE_OBJECT))) {
        ORTE_ERROR_LOG(ret);
        free(bo.bytes);
        return PMIX_ERR_PACK_FAILURE;
    }
    free(bo.bytes);

    /* thread-shift so we can store the tracker */
    opal_event_set(orte_event_base, &(req->ev),
                   -1, OPAL_EV_WRITE, execute, req);
    opal_event_set_priority(&(req->ev), ORTE_MSG_PRI);
    ORTE_POST_OBJECT(req);
    opal_event_active(&(req->ev), OPAL_EV_WRITE, 1);

    return OPAL_SUCCESS;

}

pmix_status_t pmix_server_lookup_fn(const pmix_proc_t *proc, char **keys,
                                    const pmix_info_t info[], size_t ninfo,
                                    pmix_lookup_cbfunc_t cbfunc, void *cbdata)
{
    pmix_server_req_t *req;
    int ret;
    uint8_t cmd = ORTE_PMIX_LOOKUP_CMD;
    size_t m, n;
    pmix_data_buffer_t pbkt;
    pmix_byte_object_t pbo;
    opal_byte_object_t bo, *boptr;
    pmix_proc_t psender;
    pmix_status_t rc;

    if (NULL == keys || 0 == opal_argv_count(keys)) {
        return PMIX_ERR_BAD_PARAM;
    }

    /* create the caddy */
    req = OBJ_NEW(pmix_server_req_t);
    opal_asprintf(&req->operation, "LOOKUP: %s:%d", __FILE__, __LINE__);
    req->lkcbfunc = cbfunc;
    req->cbdata = cbdata;

    /* load the command */
    if (OPAL_SUCCESS != (ret = opal_dss.pack(&req->msg, &cmd, 1, OPAL_UINT8))) {
        ORTE_ERROR_LOG(ret);
        OBJ_RELEASE(req);
        return PMIX_ERR_PACK_FAILURE;
    }

    /* no help for it - need to search for range and timeout */
   for(n=0; n < ninfo; n++) {
        if (0 == strncmp(info[n].key, PMIX_RANGE, PMIX_MAX_KEYLEN)) {
            req->range = info[n].value.data.range;
        } else if (0 == strncmp(info[n].key, PMIX_TIMEOUT, PMIX_MAX_KEYLEN)) {
            req->timeout = info[n].value.data.integer;
        }
    }

    /* setup a pmix_data_buffer_t to hold the message */
    PMIX_DATA_BUFFER_CONSTRUCT(&pbkt);
    /* I will be sending it */
    OPAL_PMIX_CONVERT_NAME(&psender, ORTE_PROC_MY_NAME);

    /* pack the name of the requestor */
    if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, (pmix_proc_t*)proc, 1, PMIX_PROC))) {
        PMIX_ERROR_LOG(rc);
        OBJ_RELEASE(req);
        return rc;
    }

    /* pack the number of keys */
    n = opal_argv_count(keys);
    if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, &n, 1, PMIX_SIZE))) {
        PMIX_ERROR_LOG(rc);
        OBJ_RELEASE(req);
        return rc;
    }
    /* pack the keys */
    for (m=0; NULL != keys[m]; m++) {
        if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, &keys[m], 1, PMIX_STRING))) {
            PMIX_ERROR_LOG(rc);
            OBJ_RELEASE(req);
            return rc;
        }
    }

    /* pack the number of infos */
    if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, &ninfo, 1, PMIX_SIZE))) {
        PMIX_ERROR_LOG(rc);
        OBJ_RELEASE(req);
        return rc;
    }

    /* pack the infos */
    if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, (pmix_info_t*)info, ninfo, PMIX_INFO))) {
        PMIX_ERROR_LOG(rc);
        OBJ_RELEASE(req);
        return rc;
    }

    /* unload the pmix buffer */
    PMIX_DATA_BUFFER_UNLOAD(&pbkt, pbo.bytes, pbo.size);
    bo.bytes = (uint8_t*)pbo.bytes;
    bo.size = pbo.size;

    /* pack it into our msg */
    boptr = &bo;
    if (ORTE_SUCCESS != (ret = opal_dss.pack(&req->msg, &boptr, 1, OPAL_BYTE_OBJECT))) {
        ORTE_ERROR_LOG(ret);
        free(bo.bytes);
        return PMIX_ERR_PACK_FAILURE;
    }
    free(bo.bytes);

    /* thread-shift so we can store the tracker */
    opal_event_set(orte_event_base, &(req->ev),
                   -1, OPAL_EV_WRITE, execute, req);
    opal_event_set_priority(&(req->ev), ORTE_MSG_PRI);
    ORTE_POST_OBJECT(req);
    opal_event_active(&(req->ev), OPAL_EV_WRITE, 1);

    return OPAL_SUCCESS;
}

pmix_status_t pmix_server_unpublish_fn(const pmix_proc_t *proc, char **keys,
                                       const pmix_info_t info[], size_t ninfo,
                                       pmix_op_cbfunc_t cbfunc, void *cbdata)
{
    pmix_server_req_t *req;
    int ret;
    uint8_t cmd = ORTE_PMIX_UNPUBLISH_CMD;
    pmix_data_buffer_t pbkt;
    pmix_byte_object_t pbo;
    opal_byte_object_t bo, *boptr;
    pmix_proc_t psender;
    size_t m, n;
    pmix_status_t rc;

    /* create the caddy */
    req = OBJ_NEW(pmix_server_req_t);
    opal_asprintf(&req->operation, "UNPUBLISH: %s:%d", __FILE__, __LINE__);
    req->opcbfunc = cbfunc;
    req->cbdata = cbdata;

    /* load the command */
    if (OPAL_SUCCESS != (ret = opal_dss.pack(&req->msg, &cmd, 1, OPAL_UINT8))) {
        ORTE_ERROR_LOG(ret);
        OBJ_RELEASE(req);
        return PMIX_ERR_PACK_FAILURE;
    }

     /* no help for it - need to search for range and timeout */
    for(n=0; n < ninfo; n++) {
         if (0 == strncmp(info[n].key, PMIX_RANGE, PMIX_MAX_KEYLEN)) {
             req->range = info[n].value.data.range;
         } else if (0 == strncmp(info[n].key, PMIX_TIMEOUT, PMIX_MAX_KEYLEN)) {
             req->timeout = info[n].value.data.integer;
         }
     }

     /* setup a pmix_data_buffer_t to hold the message */
     PMIX_DATA_BUFFER_CONSTRUCT(&pbkt);
     /* I will be sending it */
     OPAL_PMIX_CONVERT_NAME(&psender, ORTE_PROC_MY_NAME);

     /* pack the name of the requestor */
     if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, (pmix_proc_t*)proc, 1, PMIX_PROC))) {
         PMIX_ERROR_LOG(rc);
         OBJ_RELEASE(req);
         return rc;
     }

     /* pack the number of keys */
     n = opal_argv_count(keys);
     if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, &n, 1, PMIX_SIZE))) {
         PMIX_ERROR_LOG(rc);
         OBJ_RELEASE(req);
         return rc;
     }
     /* pack the keys */
     for (m=0; m < n; m++) {
         if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, &keys[m], 1, PMIX_STRING))) {
             PMIX_ERROR_LOG(rc);
             OBJ_RELEASE(req);
             return rc;
         }
     }

     /* pack the number of infos */
     if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, &ninfo, 1, PMIX_SIZE))) {
         PMIX_ERROR_LOG(rc);
         OBJ_RELEASE(req);
         return rc;
     }

     /* pack the infos */
     if (PMIX_SUCCESS != (rc = PMIx_Data_pack(&psender, &pbkt, (pmix_info_t*)info, ninfo, PMIX_INFO))) {
         PMIX_ERROR_LOG(rc);
         OBJ_RELEASE(req);
         return rc;
     }

     /* unload the pmix buffer */
     PMIX_DATA_BUFFER_UNLOAD(&pbkt, pbo.bytes, pbo.size);
     bo.bytes = (uint8_t*)pbo.bytes;
     bo.size = pbo.size;

     /* pack it into our msg */
     boptr = &bo;
     if (ORTE_SUCCESS != (ret = opal_dss.pack(&req->msg, &boptr, 1, OPAL_BYTE_OBJECT))) {
         ORTE_ERROR_LOG(ret);
         free(bo.bytes);
         return PMIX_ERR_PACK_FAILURE;
     }
     free(bo.bytes);

    /* thread-shift so we can store the tracker */
    opal_event_set(orte_event_base, &(req->ev),
                   -1, OPAL_EV_WRITE, execute, req);
    opal_event_set_priority(&(req->ev), ORTE_MSG_PRI);
    ORTE_POST_OBJECT(req);
    opal_event_active(&(req->ev), OPAL_EV_WRITE, 1);

    return OPAL_SUCCESS;
}

void pmix_server_keyval_client(int status, orte_process_name_t* sender,
                               opal_buffer_t *buffer,
                               orte_rml_tag_t tg, void *cbdata)
{
    uint8_t command;
    int rc, room_num = -1;
    int32_t cnt;
    pmix_server_req_t *req=NULL;
    opal_byte_object_t *boptr;
    pmix_data_buffer_t pbkt;
    pmix_status_t ret, rt = PMIX_SUCCESS;
    pmix_proc_t psender;
    pmix_info_t info;
    pmix_pdata_t *pdata = NULL;
    size_t n, npdata = 0;

    opal_output_verbose(1, orte_pmix_server_globals.output,
                        "%s recvd lookup data return",
                        ORTE_NAME_PRINT(ORTE_PROC_MY_NAME));

    /* unpack the room number of the request tracker */
    cnt = 1;
    if (OPAL_SUCCESS != (rc = opal_dss.unpack(buffer, &room_num, &cnt, OPAL_INT))) {
        ORTE_ERROR_LOG(rc);
        ret = PMIX_ERR_UNPACK_FAILURE;
        goto release;
    }

    /* unpack the command */
    cnt = 1;
    if (ORTE_SUCCESS != (rc = opal_dss.unpack(buffer, &command, &cnt, OPAL_UINT8))) {
        ORTE_ERROR_LOG(rc);
        return;
    }

    /* unpack the return status */
    cnt = 1;
    if (OPAL_SUCCESS != (rc = opal_dss.unpack(buffer, &status, &cnt, OPAL_INT))) {
        ORTE_ERROR_LOG(rc);
        ret = PMIX_ERR_UNPACK_FAILURE;
        goto release;
    }

    if (ORTE_ERR_NOT_FOUND == status) {
        ret = PMIX_ERR_NOT_FOUND;
        goto release;
    } else if (ORTE_ERR_PARTIAL_SUCCESS == status) {
        rt = PMIX_QUERY_PARTIAL_SUCCESS;
    } else {
        ret = PMIX_SUCCESS;
    }
    if (ORTE_PMIX_UNPUBLISH_CMD == command) {
        /* nothing else will be included */
        goto release;
    }

    /* unpack the byte object payload */
    cnt = 1;
    rc = opal_dss.unpack(buffer, &boptr, &cnt, OPAL_BYTE_OBJECT);
    /* there may not be anything returned here - e.g., a publish
     * command will not return any data if no matching pending
     * requests were found */
    if (PMIX_SUCCESS != rc) {
        if (PMIX_SUCCESS == ret) {
            ret = rt;
        }
        goto release;
    }

    /* load it into a pmix data buffer for processing */
    PMIX_DATA_BUFFER_LOAD(&pbkt, boptr->bytes, boptr->size);
    boptr->bytes = NULL;
    free(boptr);

    /* convert the sender */
    OPAL_PMIX_CONVERT_NAME(&psender, sender);

    /* unpack the number of data items */
    cnt = 1;
    if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, &npdata, &cnt, PMIX_SIZE))) {
        PMIX_ERROR_LOG(ret);
        PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
        goto release;
    }

    if (0 < npdata) {
        PMIX_PDATA_CREATE(pdata, npdata);
        for (n=0; n < npdata; n++) {
            PMIX_INFO_CONSTRUCT(&info);
            cnt = 1;
            if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, &pdata[n].proc, &cnt, PMIX_PROC))) {
                PMIX_ERROR_LOG(ret);
                PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
                goto release;
            }
            cnt = 1;
            if (PMIX_SUCCESS != (ret = PMIx_Data_unpack(&psender, &pbkt, &info, &cnt, PMIX_INFO))) {
                PMIX_ERROR_LOG(ret);
                PMIX_DATA_BUFFER_DESTRUCT(&pbkt);
                goto release;
            }
            (void)strncpy(pdata[n].key, info.key, PMIX_MAX_KEYLEN);
            pmix_value_xfer(&pdata[n].value, &info.value);
            PMIX_INFO_DESTRUCT(&info);
        }
    }
    if (PMIX_SUCCESS == ret) {
        ret = rt;
    }

  release:
    if (0 <= room_num) {
        /* retrieve the tracker */
        opal_hotel_checkout_and_return_occupant(&orte_pmix_server_globals.reqs, room_num, (void**)&req);
    }

    if (NULL != req) {
        /* pass down the response */
        if (NULL != req->opcbfunc) {
            req->opcbfunc(ret, req->cbdata);
        } else if (NULL != req->lkcbfunc) {
            req->lkcbfunc(ret, pdata, npdata, req->cbdata);
        } else {
            /* should not happen */
            ORTE_ERROR_LOG(ORTE_ERR_NOT_SUPPORTED);
        }

        /* cleanup */
        OBJ_RELEASE(req);
    }
    if (NULL != pdata) {
        PMIX_PDATA_FREE(pdata, npdata);
    }
}
