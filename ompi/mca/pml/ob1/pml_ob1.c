/*
 * Copyright (c) 2004-2008 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2008 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdlib.h>
#include <string.h>

#include "ompi/class/ompi_bitmap.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/mca/pml/base/base.h"
#include "ompi/mca/btl/btl.h"
#include "ompi/mca/pml/base/base.h"
#include "ompi/mca/btl/base/base.h"
#include "pml_ob1.h"
#include "pml_ob1_component.h"
#include "pml_ob1_comm.h"
#include "pml_ob1_hdr.h"
#include "pml_ob1_recvfrag.h"
#include "pml_ob1_sendreq.h"
#include "pml_ob1_recvreq.h"
#include "pml_ob1_rdmafrag.h"
#include "ompi/mca/bml/base/base.h"
#include "orte/mca/errmgr/errmgr.h"
#include "orte/mca/grpcomm/grpcomm.h"
#include "orte/runtime/orte_globals.h"

#include "ompi/runtime/ompi_cr.h"
#include "ompi/runtime/ompi_module_exchange.h"
#include "orte/mca/rml/rml.h"

mca_pml_ob1_t mca_pml_ob1 = {
    {
        mca_pml_ob1_add_procs,
        mca_pml_ob1_del_procs,
        mca_pml_ob1_enable,
        mca_pml_ob1_progress,
        mca_pml_ob1_add_comm,
        mca_pml_ob1_del_comm,
        mca_pml_ob1_irecv_init,
        mca_pml_ob1_irecv,
        mca_pml_ob1_recv,
        mca_pml_ob1_isend_init,
        mca_pml_ob1_isend,
        mca_pml_ob1_send,
        mca_pml_ob1_iprobe,
        mca_pml_ob1_probe,
        mca_pml_ob1_start,
        mca_pml_ob1_dump,
        mca_pml_ob1_ft_event,
        32768,
        INT_MAX
    }
};


void mca_pml_ob1_error_handler( struct mca_btl_base_module_t* btl,
                                int32_t flags );

int mca_pml_ob1_enable(bool enable)
{
    if( false == enable ) return OMPI_SUCCESS;

    OBJ_CONSTRUCT(&mca_pml_ob1.lock, opal_mutex_t);

    /* fragments */
    OBJ_CONSTRUCT(&mca_pml_ob1.rdma_frags, ompi_free_list_t);
    ompi_free_list_init_new( &mca_pml_ob1.rdma_frags,
                         sizeof(mca_pml_ob1_rdma_frag_t),
                         CACHE_LINE_SIZE,
                         OBJ_CLASS(mca_pml_ob1_rdma_frag_t),
                         0,CACHE_LINE_SIZE,
                         mca_pml_ob1.free_list_num,
                         mca_pml_ob1.free_list_max,
                         mca_pml_ob1.free_list_inc,
                         NULL );
                                                                                                            
    OBJ_CONSTRUCT(&mca_pml_ob1.recv_frags, ompi_free_list_t);

    ompi_free_list_init_new( &mca_pml_ob1.recv_frags,
                         sizeof(mca_pml_ob1_recv_frag_t) + mca_pml_ob1.unexpected_limit,
                         CACHE_LINE_SIZE,
                         OBJ_CLASS(mca_pml_ob1_recv_frag_t),
                         0,CACHE_LINE_SIZE,
                         mca_pml_ob1.free_list_num,
                         mca_pml_ob1.free_list_max,
                         mca_pml_ob1.free_list_inc,
                         NULL );
                                                                                                            
    OBJ_CONSTRUCT(&mca_pml_ob1.pending_pckts, ompi_free_list_t);
    ompi_free_list_init_new( &mca_pml_ob1.pending_pckts,
                         sizeof(mca_pml_ob1_pckt_pending_t),
                         CACHE_LINE_SIZE,
                         OBJ_CLASS(mca_pml_ob1_pckt_pending_t),
                         0,CACHE_LINE_SIZE,
                         mca_pml_ob1.free_list_num,
                         mca_pml_ob1.free_list_max,
                         mca_pml_ob1.free_list_inc,
                         NULL );


    OBJ_CONSTRUCT(&mca_pml_ob1.buffers, ompi_free_list_t);
    OBJ_CONSTRUCT(&mca_pml_ob1.send_ranges, ompi_free_list_t);
    ompi_free_list_init_new( &mca_pml_ob1.send_ranges,
                         sizeof(mca_pml_ob1_send_range_t) +
                         (mca_pml_ob1.max_send_per_range - 1) * sizeof(mca_pml_ob1_com_btl_t),
                         CACHE_LINE_SIZE,
                         OBJ_CLASS(mca_pml_ob1_send_range_t),
                         0,CACHE_LINE_SIZE,
                         mca_pml_ob1.free_list_num,
                         mca_pml_ob1.free_list_max,
                         mca_pml_ob1.free_list_inc,
                         NULL );

    /* pending operations */
    OBJ_CONSTRUCT(&mca_pml_ob1.send_pending, opal_list_t);
    OBJ_CONSTRUCT(&mca_pml_ob1.recv_pending, opal_list_t);
    OBJ_CONSTRUCT(&mca_pml_ob1.pckt_pending, opal_list_t);
    OBJ_CONSTRUCT(&mca_pml_ob1.rdma_pending, opal_list_t);

    /**
     * If we get here this is the PML who get selected for the run. We
     * should get ownership for the send and receive requests list, and
     * initialize them with the size of our own requests.
     */
    ompi_free_list_init_new( &mca_pml_base_send_requests,
                         sizeof(mca_pml_ob1_send_request_t) +
                         (mca_pml_ob1.max_rdma_per_request - 1) *
                         sizeof(mca_pml_ob1_com_btl_t),
                         CACHE_LINE_SIZE,
                         OBJ_CLASS(mca_pml_ob1_send_request_t),
                         0,CACHE_LINE_SIZE,
                         mca_pml_ob1.free_list_num,
                         mca_pml_ob1.free_list_max,
                         mca_pml_ob1.free_list_inc,
                         NULL );

    ompi_free_list_init_new( &mca_pml_base_recv_requests,
                         sizeof(mca_pml_ob1_recv_request_t) +
                         (mca_pml_ob1.max_rdma_per_request - 1) *
                         sizeof(mca_pml_ob1_com_btl_t),
                         CACHE_LINE_SIZE,
                         OBJ_CLASS(mca_pml_ob1_recv_request_t),
                         0,CACHE_LINE_SIZE,
                         mca_pml_ob1.free_list_num,
                         mca_pml_ob1.free_list_max,
                         mca_pml_ob1.free_list_inc,
                         NULL );

    mca_pml_ob1.enabled = true;
    return OMPI_SUCCESS;
}

int mca_pml_ob1_add_comm(ompi_communicator_t* comm)
{
    /* allocate pml specific comm data */
    mca_pml_ob1_comm_t* pml_comm = OBJ_NEW(mca_pml_ob1_comm_t);
    int i;

    if (NULL == pml_comm) {
        return OMPI_ERR_OUT_OF_RESOURCE;
    }
    mca_pml_ob1_comm_init_size(pml_comm, comm->c_remote_group->grp_proc_count);
    comm->c_pml_comm = pml_comm;

    for( i = 0; i < comm->c_remote_group->grp_proc_count; i++ ) {
        pml_comm->procs[i].ompi_proc = ompi_group_peer_lookup(comm->c_remote_group,i);
    }
    return OMPI_SUCCESS;
}

int mca_pml_ob1_del_comm(ompi_communicator_t* comm)
{
    OBJ_RELEASE(comm->c_pml_comm);
    comm->c_pml_comm = NULL;
    return OMPI_SUCCESS;
}


/*
 *   For each proc setup a datastructure that indicates the PTLs
 *   that can be used to reach the destination.
 *
 */

int mca_pml_ob1_add_procs(ompi_proc_t** procs, size_t nprocs)
{
    ompi_bitmap_t reachable;
    struct mca_bml_base_endpoint_t ** bml_endpoints = NULL;
    int rc;
    size_t i;

    if(nprocs == 0)
        return OMPI_SUCCESS;

    /* we don't have any endpoint data we need to cache on the
       ompi_proc_t, so set proc_pml to NULL */
    for (i = 0 ; i < nprocs ; ++i) {
        procs[i]->proc_pml = NULL;
    }

    OBJ_CONSTRUCT(&reachable, ompi_bitmap_t);
    rc = ompi_bitmap_init(&reachable, (int)nprocs);
    if(OMPI_SUCCESS != rc)
        return rc;

    /*
     * JJH: Disable this in FT enabled builds since
     * we use a wrapper PML. It will cause this check to 
     * return failure as all processes will return the wrapper PML
     * component in use instead of the wrapped PML component underneath.
     */
#if OPAL_ENABLE_FT == 0
    /* make sure remote procs are using the same PML as us */
    if (OMPI_SUCCESS != (rc = mca_pml_base_pml_check_selected("ob1",
                                                              procs,
                                                              nprocs))) {
        return rc;
    }
#endif

    bml_endpoints = (struct mca_bml_base_endpoint_t **) malloc ( nprocs *
		     sizeof(struct mca_bml_base_endpoint_t*));
    if ( NULL == bml_endpoints ) {
        rc = OMPI_ERR_OUT_OF_RESOURCE;
        goto cleanup_and_return;
    }
   
    rc = mca_bml.bml_add_procs( nprocs,
                                procs,
                                bml_endpoints,
                                &reachable );
    if(OMPI_SUCCESS != rc)
        goto cleanup_and_return;

    rc = mca_bml.bml_register( MCA_BTL_TAG_PML,
                               mca_pml_ob1_recv_frag_callback,
                               NULL );
    if(OMPI_SUCCESS != rc)
        goto cleanup_and_return;
    
    /* register error handlers */
    rc = mca_bml.bml_register_error(mca_pml_ob1_error_handler);
    if(OMPI_SUCCESS != rc)
        goto cleanup_and_return;
    
  cleanup_and_return:
    if( NULL != bml_endpoints) {
        free ( bml_endpoints);
    }
    OBJ_DESTRUCT(&reachable);

    return rc;
}

/*
 * iterate through each proc and notify any PTLs associated
 * with the proc that it is/has gone away
 */

int mca_pml_ob1_del_procs(ompi_proc_t** procs, size_t nprocs)
{
    return mca_bml.bml_del_procs(nprocs, procs);
}

/*
 * diagnostics
 */

int mca_pml_ob1_dump(struct ompi_communicator_t* comm, int verbose)
{
    struct mca_pml_comm_t* pml_comm = comm->c_pml_comm;
    int i;

    /* iterate through all procs on communicator */
    for( i = 0; i < (int)pml_comm->num_procs; i++ ) {
        mca_pml_ob1_comm_proc_t* proc = &pml_comm->procs[i];
        mca_bml_base_endpoint_t* ep = (mca_bml_base_endpoint_t*)proc->ompi_proc->proc_bml;
        size_t n;

        opal_output(0, "[Rank %d]\n", i);
        /* dump all receive queues */

        /* dump all btls */
        for(n=0; n<ep->btl_eager.arr_size; n++) {
            mca_bml_base_btl_t* bml_btl = &ep->btl_eager.bml_btls[n];
            bml_btl->btl->btl_dump(bml_btl->btl, bml_btl->btl_endpoint, verbose);
        }
    }
    return OMPI_SUCCESS;
}

static void mca_pml_ob1_fin_completion( mca_btl_base_module_t* btl,
                                        struct mca_btl_base_endpoint_t* ep,
                                        struct mca_btl_base_descriptor_t* des,
                                        int status )
{
    
    mca_bml_base_btl_t* bml_btl = (mca_bml_base_btl_t*) des->des_context; 

    /* check for pending requests */
    MCA_PML_OB1_PROGRESS_PENDING(bml_btl);
}

int mca_pml_ob1_send_fin( ompi_proc_t* proc,
                          mca_bml_base_btl_t* bml_btl,
                          void *hdr_des,
                          uint8_t order,
                          uint32_t status )
{
    mca_btl_base_descriptor_t* fin;
    mca_pml_ob1_fin_hdr_t* hdr;
    int rc;

    mca_bml_base_alloc(bml_btl, &fin, order, sizeof(mca_pml_ob1_fin_hdr_t),
                       MCA_BTL_DES_FLAGS_PRIORITY | MCA_BTL_DES_FLAGS_BTL_OWNERSHIP);

    if(NULL == fin) {
        MCA_PML_OB1_ADD_FIN_TO_PENDING(proc, hdr_des, bml_btl, order, status);
        return OMPI_ERR_OUT_OF_RESOURCE;
    }
    fin->des_cbfunc = mca_pml_ob1_fin_completion;
    fin->des_cbdata = NULL;

    /* fill in header */
    hdr = (mca_pml_ob1_fin_hdr_t*)fin->des_src->seg_addr.pval;
    hdr->hdr_common.hdr_flags = 0;
    hdr->hdr_common.hdr_type = MCA_PML_OB1_HDR_TYPE_FIN;
    hdr->hdr_des.pval = hdr_des;
    hdr->hdr_fail = status;

    ob1_hdr_hton(hdr, MCA_PML_OB1_HDR_TYPE_FIN, proc);

    /* queue request */
    rc = mca_bml_base_send(
                           bml_btl,
                           fin,
                           MCA_BTL_TAG_PML
                           );
    if(OMPI_SUCCESS != rc) {
        mca_bml_base_free(bml_btl, fin);
        MCA_PML_OB1_ADD_FIN_TO_PENDING(proc, hdr_des, bml_btl, order, status);
        return OMPI_ERR_OUT_OF_RESOURCE;
    }
    
    return OMPI_SUCCESS;
}

void mca_pml_ob1_process_pending_packets(mca_bml_base_btl_t* bml_btl)
{
    mca_pml_ob1_pckt_pending_t *pckt;
    int32_t i, rc, s = (int32_t)opal_list_get_size(&mca_pml_ob1.pckt_pending);

    for(i = 0; i < s; i++) {
        mca_bml_base_btl_t *send_dst = NULL;
        OPAL_THREAD_LOCK(&mca_pml_ob1.lock);
        pckt = (mca_pml_ob1_pckt_pending_t*)
            opal_list_remove_first(&mca_pml_ob1.pckt_pending);
        OPAL_THREAD_UNLOCK(&mca_pml_ob1.lock);
        if(NULL == pckt)
            break;
        if(pckt->bml_btl != NULL && 
                pckt->bml_btl->btl == bml_btl->btl) {
            send_dst = pckt->bml_btl;
        } else {
            send_dst = mca_bml_base_btl_array_find(
                    &pckt->proc->proc_bml->btl_eager, bml_btl->btl);
        }
        if(NULL == send_dst) {
            OPAL_THREAD_LOCK(&mca_pml_ob1.lock);
            opal_list_append(&mca_pml_ob1.pckt_pending,
                    (opal_list_item_t*)pckt);
            OPAL_THREAD_UNLOCK(&mca_pml_ob1.lock);
            continue;
        }

        switch(pckt->hdr.hdr_common.hdr_type) {
            case MCA_PML_OB1_HDR_TYPE_ACK:
                rc = mca_pml_ob1_recv_request_ack_send_btl(pckt->proc,
                        send_dst,
                        pckt->hdr.hdr_ack.hdr_src_req.lval,
                        pckt->hdr.hdr_ack.hdr_dst_req.pval,
                        pckt->hdr.hdr_ack.hdr_send_offset,
                        pckt->hdr.hdr_common.hdr_flags & MCA_PML_OB1_HDR_FLAGS_NORDMA);
                MCA_PML_OB1_PCKT_PENDING_RETURN(pckt);
                if(OMPI_ERR_OUT_OF_RESOURCE == rc) {
                    MCA_PML_OB1_ADD_ACK_TO_PENDING(pckt->proc,
                            pckt->hdr.hdr_ack.hdr_src_req.lval,
                            pckt->hdr.hdr_ack.hdr_dst_req.pval,
                            pckt->hdr.hdr_ack.hdr_send_offset);
                    return;
                }
                break;
            case MCA_PML_OB1_HDR_TYPE_FIN:
                rc = mca_pml_ob1_send_fin(pckt->proc, send_dst,
                                          pckt->hdr.hdr_fin.hdr_des.pval,
                                          pckt->order,
                                          pckt->hdr.hdr_fin.hdr_fail);
                MCA_PML_OB1_PCKT_PENDING_RETURN(pckt);
                if(OMPI_ERR_OUT_OF_RESOURCE == rc)
                     return;
                break;
            default:
                opal_output(0, "[%s:%d] wrong header type\n",
                        __FILE__, __LINE__);
                break;
        }
    }
}

void mca_pml_ob1_process_pending_rdma(void)
{
    mca_pml_ob1_rdma_frag_t* frag;
    int32_t i, rc, s = (int32_t)opal_list_get_size(&mca_pml_ob1.rdma_pending);

    for(i = 0; i < s; i++) {
        OPAL_THREAD_LOCK(&mca_pml_ob1.lock);
        frag = (mca_pml_ob1_rdma_frag_t*)
            opal_list_remove_first(&mca_pml_ob1.rdma_pending);
        OPAL_THREAD_UNLOCK(&mca_pml_ob1.lock);
        if(NULL == frag)
            break;
        if(frag->rdma_state == MCA_PML_OB1_RDMA_PUT) {
            frag->retries++;
            rc = mca_pml_ob1_send_request_put_frag(frag);
        } else {
            rc = mca_pml_ob1_recv_request_get_frag(frag);
        }
        if(OMPI_ERR_OUT_OF_RESOURCE == rc)
            break;
    }
}


void mca_pml_ob1_error_handler(
        struct mca_btl_base_module_t* btl,
        int32_t flags) { 
    orte_errmgr.abort(-1, NULL);
}

int mca_pml_ob1_ft_event( int state )
{
    ompi_proc_t** procs = NULL;
    size_t num_procs;
    int ret, p;

    if(OPAL_CRS_CHECKPOINT == state) {
        ;
    }
    else if(OPAL_CRS_CONTINUE == state) {
        ;
    }
    else if(OPAL_CRS_RESTART_PRE == state ) {
        /* Nothing here */
    }
    else if(OPAL_CRS_RESTART == state ) {
        /*
         * Get a list of processes
         */
        procs = ompi_proc_all(&num_procs);
        if(NULL == procs) {
            return OMPI_ERR_OUT_OF_RESOURCE;
        }

        /*
         * Clean out the modex information since it is invalid now.
         *    orte_grpcomm.purge_proc_attrs();
         * This happens at the ORTE level, so doing it again here will cause
         * some issues with socket caching.
         */


        /*
         * Refresh the proc structure, and publish our proc info in the modex.
         * NOTE: Do *not* call ompi_proc_finalize as there are many places in
         *       the code that point to indv. procs in this strucutre. For our
         *       needs here we only need to fix up the modex, bml and pml 
         *       references.
         */
        if (OMPI_SUCCESS != (ret = ompi_proc_refresh())) {
            opal_output(0,
                        "pml:ob1: ft_event(Restart): proc_refresh Failed %d",
                        ret);
            return ret;
        }
    }
    else if(OPAL_CRS_TERM == state ) {
        ;
    }
    else {
        ;
    }

    /* Call the BML
     * BML is expected to call ft_event in
     * - BTL(s)
     * - MPool(s)
     */
    if( OMPI_SUCCESS != (ret = mca_bml.bml_ft_event(state))) {
        opal_output(0, "pml:base: ft_event: BML ft_event function failed: %d\n",
                    ret);
    }
    
    if(OPAL_CRS_CHECKPOINT == state) {
        ;
    }
    else if(OPAL_CRS_CONTINUE == state) {
        ;
    }
    else if(OPAL_CRS_RESTART_PRE == state ) {
        /* Nothing here */
    }
    else if(OPAL_CRS_RESTART == state  ) {
        /*
         * Exchange the modex information once again.
         * BTLs will have republished their modex information.
         */
        if (OMPI_SUCCESS != (ret = orte_grpcomm.modex(NULL))) {
            opal_output(0,
                        "pml:ob1: ft_event(Restart): Failed orte_grpcomm.modex() = %d",
                        ret);
            return ret;
        }

        /*
         * Startup the PML stack now that the modex is running again
         * Add the new procs (BTLs redo modex recv's)
         */
        if( OMPI_SUCCESS != (ret = mca_pml_ob1_add_procs(procs, num_procs) ) ) {
            opal_output(0, "pml:ob1: fr_event(Restart): Failed in add_procs (%d)", ret);
            return ret;
        }

        /* Is this barrier necessary ? JJH */
        if (OMPI_SUCCESS != (ret = orte_grpcomm.barrier())) {
            opal_output(0, "pml:ob1: fr_event(Restart): Failed in orte_grpcomm.barrier (%d)", ret);
            return ret;
        }

        if( NULL != procs ) {
            for(p = 0; p < (int)num_procs; ++p) {
                OBJ_RELEASE(procs[p]);
            }
            free(procs);
            procs = NULL;
        }
    }
    else if(OPAL_CRS_TERM == state ) {
        ;
    }
    else {
        ;
    }

    return OMPI_SUCCESS;
}

int mca_pml_ob1_com_btl_comp(const void *v1, const void *v2)
{
    const mca_pml_ob1_com_btl_t *b1 = (const mca_pml_ob1_com_btl_t *) v1;
    const mca_pml_ob1_com_btl_t *b2 = (const mca_pml_ob1_com_btl_t *) v2;

    if(b1->bml_btl->btl_weight < b2->bml_btl->btl_weight)
        return 1;
    if(b1->bml_btl->btl_weight > b2->bml_btl->btl_weight)
        return -1;

    return 0;
}
