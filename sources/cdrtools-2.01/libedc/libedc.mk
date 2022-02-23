#ident @(#)libedc.mk	1.4 04/01/06 
###########################################################################
SRCROOT=	..
RULESDIR=	RULES
include		$(SRCROOT)/$(RULESDIR)/rules.top
###########################################################################

#.SEARCHLIST:	. $(ARCHDIR) stdio $(ARCHDIR)
#VPATH=		.:stdio:$(ARCHDIR)
INSDIR=		lib
#TARGETLIB=	oedc
TARGETLIB=	edc_ecc
CPPOPTS +=	-Iold
#
# Selectively increase the opimisation for libedc for better performance
#
# The code has been tested for correctness with this level of optimisation
# If your GCC creates defective code, you found a GCC bug that should
# be reported to the GCC people. As a workaround, you may remove the next
# lines to fall back to the standard optimisation level.
#
SUNPROCOPTOPT=	-fast -xarch=generic
GCCOPTOPT=	-O3  -fexpensive-optimizations
#
CFILES=		edc_ecc.c
HFILES=
#include		Targets
LIBS=		

###########################################################################
include		$(SRCROOT)/$(RULESDIR)/rules.lib
###########################################################################
