/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2002 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Type_contigous */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Type_contiguous_x = PMPI_Type_contiguous_x
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Type_contiguous_x  MPI_Type_contiguous_x
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Type_contiguous_x as PMPI_Type_contiguous_x
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines.  You can use USE_WEAK_SYMBOLS to see if MPICH is
   using weak symbols to implement the MPI routines. */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Type_contiguous_x
#define MPI_Type_contiguous_x PMPI_Type_contiguous_x

#undef FUNCNAME
#define FUNCNAME MPIR_Type_contiguous_impl
#undef FCNAME
#define FCNAME MPIU_QUOTE(FUNCNAME)
int MPIR_Type_contiguous_x_impl(MPI_Aint count,
                                MPI_Datatype oldtype,
                                MPI_Datatype *newtype)
{
    int mpi_errno = MPI_SUCCESS;
    MPID_Datatype *new_dtp;
    MPI_Datatype new_handle;
    
    mpi_errno = MPID_Type_contiguous(count,
				     oldtype,
				     &new_handle);

    if (mpi_errno != MPI_SUCCESS) goto fn_fail;

    MPID_Datatype_get_ptr(new_handle, new_dtp);
    mpi_errno = MPID_Datatype_set_contents(new_dtp,
				           MPI_COMBINER_CONTIGUOUS,
				           0,
				           1,      /* nr_aints */
				           1,
				           NULL,
				           &count, /* aints */
				           &oldtype);

    if (mpi_errno != MPI_SUCCESS) goto fn_fail;

    MPIU_OBJ_PUBLISH_HANDLE(*newtype, new_handle);

 fn_exit:
    return mpi_errno;
 fn_fail:

    goto fn_exit;
}

#endif


#undef FUNCNAME
#define FUNCNAME MPI_Type_contiguous_x
#undef FCNAME
#define FCNAME MPIU_QUOTE(FUNCNAME)
/*@
    MPI_Type_contiguous_x - Creates a contiguous datatype

Input Parameters:
+ count - replication count (nonnegative integer) 
- oldtype - old datatype (handle) 

Output Parameters:
. newtype - new datatype (handle) 

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_EXHAUSTED
@*/
int MPIX_Type_contiguous_x(MPI_Count count,
			   MPI_Datatype oldtype,
			   MPI_Datatype *newtype)
{
    int mpi_errno = MPI_SUCCESS;
    MPID_MPI_STATE_DECL(MPID_STATE_MPI_TYPE_CONTIGUOUS); /* FIXME? */

    MPIR_ERRTEST_INITIALIZED_ORDIE();
    
    MPIU_THREAD_CS_ENTER(ALLFUNC,);
    MPID_MPI_FUNC_ENTER(MPID_STATE_MPI_TYPE_CONTIGUOUS); /* FIXME? */

#   ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPID_Datatype *datatype_ptr = NULL;

	    MPIR_ERRTEST_COUNT(count, mpi_errno);
            MPIR_ERRTEST_DATATYPE(oldtype, "datatype", mpi_errno);
	    
            if (HANDLE_GET_KIND(oldtype) != HANDLE_KIND_BUILTIN) {
                MPID_Datatype_get_ptr(oldtype, datatype_ptr);
                MPID_Datatype_valid_ptr(datatype_ptr, mpi_errno);
                if (mpi_errno != MPI_SUCCESS) goto fn_fail;
	    }
        }
        MPID_END_ERROR_CHECKS;
    }
#   endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ... */

    mpi_errno = MPIR_Type_contiguous_x_impl(count, oldtype, newtype);
    if (mpi_errno) MPIU_ERR_POP(mpi_errno);

    /* ... end of body of routine ... */
    
  fn_exit:
    MPID_MPI_FUNC_EXIT(MPID_STATE_MPI_TYPE_CONTIGUOUS); /* FIXME? */
    MPIU_THREAD_CS_EXIT(ALLFUNC,);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#   ifdef HAVE_ERROR_CHECKING
    {
    mpi_errno = MPIR_Err_create_code(
	mpi_errno, MPIR_ERR_RECOVERABLE, FCNAME, __LINE__, MPI_ERR_OTHER, "**mpix_type_contiguous_x",
	"**mpix_type_contiguous_x %d %D %p", count, oldtype, newtype);
    }
#   endif
    mpi_errno = MPIR_Err_return_comm( NULL, FCNAME, mpi_errno );
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}
