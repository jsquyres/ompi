Quick start
===========

In many cases, Open MPI can be built and installed by simply
indicating the installation directory on the command line:

.. code-block:: sh
   :linenos:

   shell$ tar xf openmpi-<version>.tar.bz2
   shell$ cd openmpi-<version>
   shell$ ./configure --prefix=<path> |& tee config.out

   # ...lots of output...

   shell$ make -j 8 |& tee make.out

   # ...lots of output...

   shell$ make install |& tee install.out

   # ...lots of output...

Note that there are many, many configuration options to the
``./configure`` step.  Some of them may be needed for your particular
environmnet; see :doc:`the installation section <installation>` for
desciptions of the options available.

If your installation prefix path is not writable by a regular user,
you may need to use sudo or su to run the ``make install`` step.  For
example:

.. code-block:: sh
   :linenos:

   shell$ sudo make install |& tee install.out
   [sudo] password for jsquyres: <enter your password here>

   # ...lots of output...

Finally, note that VPATH builds are fully supported.  For example:

.. code-block:: sh
   :linenos:

   shell$ tar xf openmpi-<version>.tar.bz2
   shell$ cd openmpi-<version>
   shell$ mkdir build
   shell$ cd build
   shell$ ../configure --prefix=<path> |& tee config.out
   # ...etc.
