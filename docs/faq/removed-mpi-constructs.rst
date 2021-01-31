Removed MPI Constructs
======================

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

Why does my MPI application fail to compile, complaining that various MPI APIs/symbols are undefined?
-----------------------------------------------------------------------------------------------------

Starting with v4.0.0, Open MPI |mdash| by default |mdash| removes the
prototypes from ``mpi.h`` for MPI symbols that were deprecated in 1996
in the MPI-2.0 standard, and finally removed from the MPI-3.0 standard
(2012).

Specifically, the following symbols (specified in the MPI
language-neutral names) are no longer prototyped in ``mpi.h`` by
default:

.. error:: **JMS Update table with man page links**

.. list-table::
    :header-rows: 1

    * - Removed symbol (click to go to the corresponding FAQ item)
      - Replaced with (click to go to the corresponding man page)
      - Deprecated
      - Removed

    * - :ref:`MPI_ADDRESS <label-mpi-address>`
      - MPI_GET_ADDRESS
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_ERRHANDLER_CREATE <label-mpi-errhandler-create>`
      - MPI_COMM_CREATE_ERRHANDLER
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_ERRHANDLER_GET <label-mpi-errhandler-get>`
      - MPI_COMM_ERRHANDLER_GET
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_ERRHANDLER_SET <label-mpi-errhandler-set>`
      - MPI_COMM_ERRHANDLER_SET
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_TYPE_EXTENT <label-mpi-type-extent>`
      - MPI_TYPE_GET_EXTENT
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_TYPE_HINDEXED <label-mpi-type-hindexed>`
      - MPI_TYPE_CREATE_HINDEXED
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_TYPE_HVECTOR <label-mpi-type-hvector>`
      - MPI_TYPE_CREATE_HVECTOR
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_TYPE_LB <label-mpi-type-lb>`
      - MPI_TYPE_GET_EXTENT
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_TYPE_STRUCT <label-mpi-type-struct>`
      - MPI_TYPE_CREATE_STRUCT
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_TYPE_UB <label-mpi-type-ub>`
      - MPI_TYPE_GET_EXTENT
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_LB <label-mpi-lb-ub>`
      - MPI_TYPE_CREATE_RESIZED
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_UB <label-mpi-lb-ub>`
      - MPI_TYPE_CREATE_RESIZED
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_COMBINED_HINDEXED_INTEGER <label-mpi-combiner-fortran-integers>`
      - MPI_COMBINER_HINDEXED
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_COMBINED_HVECTOR_INTEGER <label-mpi-combiner-fortran-integers>`
      - MPI_COMBINER_HVECTOR
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_COMBINED_STRUCT_INTEGER <label-mpi-combiner-fortran-integers>`
      - MPI_COMBINER_STRUCT
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

    * - :ref:`MPI_HANDLER_FUNCTION <label-mpi-handler-function>`
      - MPI_COMM_ERRHANDLER_FUNCTION
      - MPI-2.0 (1996)
      - MPI-3.0 (2012)

Although these symbols are no longer prototyped in ``mpi.h``, *they are
still present in the MPI library in Open MPI v4.0.x*. This enables
legacy MPI applications to *link and run* successfully with Open MPI
v4.0.x, even though they will fail to *compile*.

.. warning:: The Open MPI team **strongly** encourages all
   MPI application developers to stop using these constructs that were
   first deprecated over 20 years ago, and finally removed from the MPI
   specification in MPI-3.0 (in 2012).

The FAQ items in this category
show how to update your application to stop using these removed
symbols.

All that being said, if you are unable to immediately update your
application to stop using these removed MPI-1 symbols, you can
re-enable them in ``mpi.h`` by configuring Open MPI with the
``--enable-mpi1-compatibility`` flag.

.. note:: Future releases of Open MPI may
   remove these symbols altogether.

/////////////////////////////////////////////////////////////////////////

Why on earth are you breaking the compilation of MPI applications?
------------------------------------------------------------------

The Open MPI developer community decided to take a first step
of removing the prototypes for these symbols from ``mpi.h`` starting
with the Open MPI v4.0.x series for the following reasons:

#. These symbols have been deprecated since *1996.*
   It's time to start raising awareness
   for developers who are inadvertently still using these removed
   symbols.
#. The MPI Forum removed these symbols from the MPI-3.0
   specification in 2012.  This is a sign that the Forum itself
   recognizes that these removed symbols are no longer needed.
#. Note that Open MPI *did not fully remove* these removed symbols:
   we just made it slightly more painful to get to them.  This is an
   attempt to raise awareness so that MPI application developers can
   update their applications (it's easy!).

In short: the only way to finally be able to remove these removed
symbols from Open MPI someday is to have a "grace period" where the
MPI application developers are a) made aware that they are using
removed symbols, and b) educated how to update their applications.

We, the Open MPI developers, recognize that your MPI application
failing to compile with Open MPI may be a nasty surprise.  We
apologize for that.

Our intent is simply to use this minor shock to raise awareness and
use it as an educational opportunity to show you how to update your
application (or direct your friendly neighborhood MPI application
developer to this FAQ) to stop using these removed MPI symbols.

Thank you!

/////////////////////////////////////////////////////////////////////////

Why am I getting deprecation warnings when compiling my MPI application?
------------------------------------------------------------------------

You are getting deprecation warnings because you are using
symbols / functions that are deprecated in MPI.  For example:

.. code-block:: sh
    :linenos:

    shell$ mpicc deprecated-example.c -c
    deprecated-example.c: In function 'foo':
    deprecated-example.c:6:5: warning: 'MPI_Attr_delete' is deprecated: MPI_Attr_delete was deprecated in MPI-2.0; use MPI_Comm_delete_attr instead [-Wdeprecated-declarations]
         MPI_Attr_delete(MPI_COMM_WORLD, 2);
         ^~~~~~~~~~~~~~~
    In file included from deprecated-example.c:2:
    /usr/local/openmpi/include/mpi.h:2601:20: note: declared here
     OMPI_DECLSPEC  int MPI_Attr_delete(MPI_Comm comm, int keyval)
                        ^~~~~~~~~~~~~~~

Note that the deprecation compiler warnings tells you how to upgrade
your code to avoid the deprecation warnings.  In this example, it
advises you to use ``MPI_Comm_delete_attr()`` instead of
``MPI_Attr_delete()``.

Also, note that when using ``--enable-mpi1-compatibility`` to re-enable
removed MPI-1 symbols you will still get compiler warnings when you use
the removed symbols.  For example:

.. code-block:: sh
    :linenos:

    shell$ mpicc deleted-example.c -c
    deleted-example.c: In function 'foo':
    deleted-example.c:8:5: warning: 'MPI_Address' is deprecated: MPI_Address was removed in MPI-3.0; use MPI_Get_address instead. [-Wdeleted-declarations]
         MPI_Address(buffer, &address);
         ^~~~~~~~~~~
    In file included from deleted-example.c:2:
    /usr/local/openmpi/include/mpi.h:2689:20: note: declared here
     OMPI_DECLSPEC  int MPI_Address(void *location, MPI_Aint *address)
                        ^~~~~~~~~~~

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-address:

How do I update my MPI application to stop using MPI_ADDRESS?
-------------------------------------------------------------

In C, the only thing that changed was the function name:
``MPI_Address()`` |rarrow| ``MPI_Get_address()``.  Nothing else needs
to change:

.. code-block:: c++
    :linenos:

    char buffer[30];
    MPI_Aint address;

    // Old way
    MPI_Address(buffer, &address);

    // New way
    MPI_Get_address(buffer, &address);

In Fortran, the type of the parameter changed from ``INTEGER``
$right_arrow ``INTEGER(KIND=MPI_ADDRESS_KIND)`` so that it can hold
larger values (e.g., 64 bit pointers):

.. code-block:: Fortran
    :linenos:

    USE mpi
    REAL buffer
    INTEGER ierror
    INTEGER old_address
    INTEGER(KIND = MPI_ADDRESS_KIND) new_address

    ! Old way
    CALL MPI_ADDRESS(buffer, old_address, ierror)

    ! New way
    CALL MPI_GET_ADDRESS(buffer, new_address, ierror)

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-errhandler-create:

How do I update my MPI application to stop using MPI_ERRHANDLER_CREATE?
-----------------------------------------------------------------------

In C, *effectively* the only thing that changed was the name
of the function: ``MPI_Errhandler_create()`` |rarrow|
``MPI_Comm_create_errhandler()``.

*Technically*, the type of the first parameter also changed
(``MPI_Handler_function`` |rarrow| ``MPI_Comm_errhandler_function``),
but most applications do not use this type directly and may not even
notice the change.

.. code-block:: c++
    :linenos:

    void my_errhandler_function(MPI_Comm *comm, int *code, ...)
    {
        // Do something useful to handle the error
    }

    void some_function(void)
    {
        MPI_Errhandler my_handler;

        // Old way
        MPI_Errhandler_create(my_errhandler_function, &my_handler);

        // New way
        MPI_Comm_create_errhandler(my_errhandler_function, &my_handler);
    }

In Fortran, only the subroutine name changed: ``MPI_ERRHANDLER_CREATE``
|rarrow| ``MPI_COMM_CREATE_ERRHANDLER``.

.. code-block:: Fortran
    :linenos:

    USE mpi
    EXTERNAL my_errhandler_function
    INTEGER ierror
    INTEGER my_handler

    ! Old way
    CALL MPI_ERRHANDLER_CREATE(my_errhandler_function, my_handler, ierror)

    ! Old way
    CALL MPI_COMM_CREATE_ERRHANDLER(my_errhandler_function, my_handler, ierror)

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-errhandler-get:

How do I update my MPI application to stop using MPI_ERRHANDLER_GET?
--------------------------------------------------------------------

In both C and Fortran, the only thing that changed with
regards to ``MPI_ERRHANDLER_GET`` is the name: ``MPI_ERRHANDLER_GET``
|rarrow| ``MPI_COMM_GET_ERRHANDLER``.

All parameter types stayed the same.

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-errhandler-set:

How do I update my MPI application to stop using MPI_ERRHANDLER_SET?
--------------------------------------------------------------------

In both C and Fortran, the only thing that changed with
regards to ``MPI_ERRHANDLER_SET`` is the name: ``MPI_ERRHANDLER_SET``
|rarrow| ``MPI_COMM_SET_ERRHANDLER``.

All parameter types stayed the same.

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-type-hindexed:

How do I update my MPI application to stop using MPI_TYPE_HINDEXED?
-------------------------------------------------------------------

In both C and Fortran, *effectively* the only change is the
name of the function: ``MPI_TYPE_HINDEXED`` |rarrow|
``MPI_TYPE_CREATE_HINDEXED``.

In C, the new function also has a ``const`` attribute on the two array
parameters, but most applications won't notice the difference.

All other parameter types stayed the same.

.. code-block:: c++
    :linenos:

    int count = 2;
    int block_lengths[] = { 1, 2 };
    MPI_Aint displacements[] = { 0, sizeof(int) };
    MPI_Datatype newtype;

    // Old way
    MPI_Type_hindexed(count, block_lengths, displacements, MPI_INT, &newtype);

    // New way
    MPI_Type_create_hindexed(count, block_lengths, displacements, MPI_INT, &newtype);

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-type-hvector:

How do I update my MPI application to stop using MPI_TYPE_HVECTOR?
------------------------------------------------------------------

In both C and Fortran, the only change is the
name of the function: ``MPI_TYPE_HVECTOR`` |rarrow|
``MPI_TYPE_CREATE_HVECTOR``.

All parameter types stayed the same.

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-type-struct:

How do I update my MPI application to stop using MPI_TYPE_STRUCT?
-----------------------------------------------------------------

In both C and Fortran, *effectively* the only change is the
name of the function: ``MPI_TYPE_STRUCT`` |rarrow|
``MPI_TYPE_CREATE_STRUCT``.

In C, the new function also has a ``const`` attribute on the three array
parameters, but most applications won't notice the difference.

All other parameter types stayed the same.

.. code-block:: c++
    :linenos:

    int count = 2;
    int block_lengths[] = { 1, 2 };
    MPI_Aint displacements[] = { 0, sizeof(int) };
    MPI_Datatype datatypes[] = { MPI_INT, MPI_DOUBLE };
    MPI_Datatype newtype;

    // Old way
    MPI_Type_struct(count, block_lengths, displacements, datatypes, &newtype);

    // New way
    MPI_Type_create_struct(count, block_lengths, displacements, datatypes, &newtype);

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-type-extent:

How do I update my MPI application to stop using MPI_TYPE_EXTENT?
-----------------------------------------------------------------

In both C and Fortran, the ``MPI_TYPE_EXTENT`` function is
superseded by the slightly-different ``MPI_TYPE_GET_EXTENT`` function:
the new function also returns the lower bound.

.. code-block:: c++
    :linenos:

    MPI_Aint lb;
    MPI_Aint extent;

    // Old way
    MPI_Type_extent(MPI_INT, &extent);

    // New way
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-type-lb:

How do I update my MPI application to stop using MPI_TYPE_LB?
-------------------------------------------------------------

In both C and Fortran, the ``MPI_TYPE_LB`` function is
superseded by the slightly-different ``MPI_TYPE_GET_EXTENT`` function:
the new function also returns the extent.

.. code-block:: c++
    :linenos:

    MPI_Aint lb;
    MPI_Aint extent;

    // Old way
    MPI_Type_lb(MPI_INT, &lb);

    // New way
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-type-ub:

How do I update my MPI application to stop using MPI_TYPE_UB?
-------------------------------------------------------------

In both C and Fortran, the ``MPI_TYPE_UB`` function is
superseded by the slightly-different ``MPI_TYPE_GET_EXTENT`` function:
the new function returns the lower bound and the extent, which can be
used to compute the upper bound.

.. code-block:: c++
    :linenos:

    MPI_Aint lb, ub;
    MPI_Aint extent;

    // Old way
    MPI_Type_ub(MPI_INT, &ub);

    // New way
    MPI_Type_get_extent(MPI_INT, &lb, &extent);
    ub = lb + extent

Note the ``ub`` calculation after calling ``MPI_Type_get_extent()``.

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-lb-ub:

How do I update my MPI application to stop using MPI_LB / MPI_UB?
-----------------------------------------------------------------

The ``MPI_LB`` and ``MPI_UB`` positional markers were fully
replaced with ``MPI_TYPE_CREATE_RESIZED`` in MPI-2.0.

Prior to MPI-2.0, ``MPI_UB`` and ``MPI_LB`` were intended to be used as
input to ``MPI_TYPE_STRUCT`` (which, itself, has been deprecated and
renamed to ``MPI_TYPE_CREATE_STRUCT``).  The same end effect can now be
achieved with ``MPI_TYPE_CREATE_RESIZED``.
For example, using the old method:

.. code-block:: c++
    :linenos:

    int count = 3;
    int block_lengths[] = { 1, 1, 1 };
    MPI_Aint displacements[] = { -2, 0, 10 };
    MPI_Datatype datatypes[] = { MPI_LB, MPI_INT, MPI_UB };
    MPI_Datatype newtype;

    MPI_Type_struct(count, block_lengths, displacements, datatypes, &newtype);
    MPI_Type_commit(&newtype);

    MPI_Aint ub, lb, extent;
    MPI_Type_lb(newtype, &lb);
    MPI_Type_ub(newtype, &ub);
    MPI_Type_extent(newtype, &extent);
    printf("OLD: LB=%d, UB=%d, extent=%d\n",
           lb, ub, extent);

If we run the above, we get an output of:

.. code-block::
    :linenos:

    OLD: LB=-2, UB=10, extent=12

The ``MPI_TYPE_RESIZED`` function allows us to take any arbitrary
datatype and set the lower bound and extent directly (which indirectly
sets the upper bound), without needing to setup the arrays and
computing the displacements necessary to invoke
``MPI_TYPE_CREATE_STRUCT``.

Aside from the printf statement, the following example is exactly
equivalent to the prior example (see <a
href=\"#mpi-1-mpi-type-ub\">this FAQ entry</a> for a mapping of
``MPI_TYPE_UB`` to ``MPI_TYPE_GET_EXTENT``):

.. code-block:: c++
    :linenos:

    MPI_Datatype newtype;

    MPI_Type_create_resized(MPI_INT, -2, 12, &newtype);
    MPI_Type_commit(&newtype);

    MPI_Aint ub, lb, extent;
    MPI_Type_get_extent(newtype, &lb, &extent);
    ub = lb + extent;
    printf("NEW: LB=%d, UB=%d, extent=%d\n",
           lb, ub, extent);

If we run the above, we get an output of:

.. code-block::
    :linenos:

    NEW: LB=-2, UB=10, extent=12

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-combiner-fortran-integers:

How do I update my MPI application to stop using MPI_COMBINER_HINDEXED_INTEGER, MPI_COMBINER_HVECTOR_INTEGER, and MPI_COMBINER_STRUCT_INTEGER?
----------------------------------------------------------------------------------------------------------------------------------------------

The ``MPI_COMBINER_HINDEXED_INTEGER``,
``MPI_COMBINER_HVECTOR_INTEGER``, and ``MPI_COMBINER_STRUCT_INTEGER``
constants could previously be returned from ``MPI_TYPE_GET_ENVELOPE``.

Starting with MPI-3.0, these values will never be returned.  Instead,
they will just return the same names, but without the ``_INTEGER``
suffix.  Specifically:

* ``MPI_COMBINER_HINDEXED_INTEGER`` |rarrow| ``MPI_COMBINER_HINDEXED``
* ``MPI_COMBINER_HVECTOR_INTEGER`` |rarrow| ``MPI_COMBINER_HVECTOR``
* ``MPI_COMBINER_STRUCT_INTEGER`` |rarrow| ``MPI_COMBINER_STRUCT``

If your Fortran code is using any of the ``_INTEGER``-suffixed names,
you can just delete the ``_INTEGER`` suffix.

/////////////////////////////////////////////////////////////////////////

.. _label-mpi-handler-function:

How do I update my MPI application to stop using MPI_Handler_function?
----------------------------------------------------------------------

The ``MPI_Handler_function`` C type is only used in the
deprecated/removed function ``MPI_Errhandler_create()``, as described <a
href=\"#mpi-1-mpi-errhandler-create\">in this FAQ entry</a>.

Most MPI applications likely won't use this type at all.  But if they
do, they can simply use the new, exactly-equivalent type name (i.e.,
the return type, number, and type of parameters didn't change):
``MPI_Comm_errhandler_function``.

.. code-block:: c++
    :linenos:

    void my_errhandler_function(MPI_Comm *comm, int *code, ...)
    {
        // Do something useful to handle the error
    }

    void some_function(void)
    {
        // Old way
        MPI_Handler_function *old_ptr = my_errhandler_function;

        // New way
        MPI_Comm_errhandler_function *new_ptr = my_errhandler_function;
    }

The ``MPI_Handler_function`` type isn't used at all in the Fortran
bindings.
