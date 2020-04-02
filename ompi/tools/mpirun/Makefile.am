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
dist_pkgdata_DATA = ompirun.ini
SYM_LINK_TARGET   = ompirun
else
dist_bin_SCRIPTS  = mpirun.sh
SYM_LINK_TARGET   = mpirun.sh
endif

install-exec-hook:
	(cd $(DESTDIR)$(bindir); rm -f mpirun$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) mpirun$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f mpiexec$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) mpiexec$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f oshrun$(EXEEXT); $(LN_S) $(SYM_LINK_TARGET) oshrun$(EXEEXT))

# JMS to be removed
install-exec-hook-OLD:
	(cd $(DESTDIR)$(bindir); rm -f mpirun$(EXEEXT); $(LN_S) prte$(EXEEXT) mpirun$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f mpiexec$(EXEEXT); $(LN_S) prte$(EXEEXT) mpiexec$(EXEEXT))
	(cd $(DESTDIR)$(bindir); rm -f oshrun$(EXEEXT); $(LN_S) prte$(EXEEXT) oshrun$(EXEEXT))

uninstall-local:
	rm -f $(DESTDIR)$(bindir)/mpirun$(EXEEXT) \
		$(DESTDIR)$(bindir)/mpiexec$(EXEEXT)  \
		$(DESTDIR)$(bindir)/oshrun$(EXEEXT)
