#
# Copyright (c) 2019-2020 Intel, Inc.  All rights reserved.
# Copyright (c) 2020      IBM Corporation.  All rights reserved.
# Copyright (c) 2020      Cisco Systems, Inc.  All rights reserved.
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

EXTRA_DIST = ompirun.in mpirun.sh

DISTCLEANFILES = ompirun

# mpirun, mpiexec, and oshrun are sym links.

# 1. If we have Python >=v3, they are a sym link to ompirun (a Python v3 script)
# 2. If we do not have Python >=v3, they are a sym link to mpirun.sh

if HAVE_PYTHONV3
dist_bin_SCRIPTS  = ompirun
# Make this go to the etc dir
dist_sysconf_DATA = ompirun.ini
SYM_LINK_TARGET   = ompirun
else
dist_bin_SCRIPTS  = mpirun.sh
SYM_LINK_TARGET   = mpirun.sh
endif

install-exec-hook:
	(cd $(DESTDIR)$(bindir); rm -f mpirun$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) mpirun$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f mpiexec$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) mpiexec$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f oshrun$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) oshrun$(EXEEXT))
	if test "$(SYM_LINK_TARGET)" = "ompirun"; then \
	    cd $(DESTDIR)$(bindir); \
	    rm -f ompirun-show$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) ompirun-show$(EXEEXT); \
	    rm -f ompirun-showonly$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) ompirun-showonly$(EXEEXT); \
	fi

uninstall-local:
	rm -f $(DESTDIR)$(bindir)/mpirun$(EXEEXT) \
		$(DESTDIR)$(bindir)/mpiexec$(EXEEXT)  \
		$(DESTDIR)$(bindir)/oshrun$(EXEEXT) \
		$(DESTDIR)$(bindir)/ompirun-show$(EXEEXT) \
		$(DESTDIR)$(bindir)/ompirun-showonly$(EXEEXT)
