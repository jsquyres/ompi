dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
dnl                         University Research and Technology
dnl                         Corporation.  All rights reserved.
dnl Copyright (c) 2004-2005 The University of Tennessee and The University
dnl                         of Tennessee Research Foundation.  All rights
dnl                         reserved.
dnl Copyright (c) 2004-2018 High Performance Computing Center Stuttgart,
dnl                         University of Stuttgart.  All rights reserved.
dnl Copyright (c) 2004-2006 The Regents of the University of California.
dnl                         All rights reserved.
dnl Copyright (c) 2018      University of Houston. All rights reserved.
dnl $COPYRIGHT$
dnl
dnl Additional copyrights may follow
dnl
dnl $HEADER$
dnl

dnl
dnl OMPI_CHECK_PYTHON3()
dnl --------------------------------------------------------
dnl Find Python >= v3 if we can (not an error if we can't).
dnl Output:
dnl - Sets and AC_SUBSTs PYTHONV3 (path to Python 3 executable)
dnl - AM_CONDITIONAL for HAVE_PYTHONV3
dnl
AC_DEFUN([OMPI_CHECK_PYTHON3],[
    AC_CACHE_CHECK([for Python >=v3], [ac_cv_path_PYTHONV3],
       [file=conftest.$$.py
        rm -f $file
        cat > $file <<EOF
import sys

happy = False
try:
    if sys.version_info.major>= 3:
        happy = True
except:
    pass

if happy:
    exit(0)
else:
    exit(1)
EOF
        AC_PATH_PROGS_FEATURE_CHECK([PYTHONV3], [python3 python],
            [[$ac_path_PYTHONV3 $file && ac_cv_path_PYTHONV3=$ac_path_PYTHONV3 ac_path_PYTHONV3_found=:]])
        ])
    AC_SUBST([PYTHONV3], [$ac_cv_path_PYTHONV3])
    AM_CONDITIONAL(HAVE_PYTHONV3, [test -n "$ac_cv_path_PYTHONV3"])
])
