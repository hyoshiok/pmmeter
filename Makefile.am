
EXTRA_DIST = \
	README.md \
	AUTHORS \
	COPYING \
	$(TESTS)

dist_man_MANS = \
	numatop.8

AM_CFLAGS = -fPIC -O2 -g -Wall -W -Wformat-security -D_FORTIFY_SOURCE=2 -fno-common -std=gnu99
ACLOCAL_AMFLAGS = -I m4

noinst_LTLIBRARIES = libnumatop.la
libnumatop_la_SOURCES = \
	common/include/os/linux/perf_event.h \
	common/include/os/map.h \
	common/include/os/node.h \
	common/include/os/os_cmd.h \
	common/include/os/os_page.h \
	common/include/os/os_perf.h \
	common/include/os/os_types.h \
	common/include/os/os_util.h \
	common/include/os/os_win.h \
	common/include/os/pfwrapper.h \
	common/include/os/plat.h \
	common/include/os/sym.h \
	common/include/cmd.h \
	common/include/disp.h \
	common/include/lwp.h \
	common/include/page.h \
	common/include/perf.h \
	common/include/proc.h \
	common/include/reg.h \
	common/include/types.h \
	common/include/ui_perf_map.h \
	common/include/util.h \
	common/include/win.h \
	common/os/map.c \
	common/os/node.c \
	common/os/os_cmd.c \
	common/os/os_page.c \
	common/os/os_perf.c \
	common/os/os_util.c \
	common/os/os_win.c \
	common/os/pfwrapper.c \
	common/os/plat.c \
	common/os/sym.c \
	common/cmd.c \
	common/disp.c \
	common/lwp.c \
	common/page.c \
	common/perf.c \
	common/proc.c \
	common/reg.c \
	common/ui_perf_map.c \
	common/util.c \
	common/win.c

if CPU_INTEL
libnumatop_la_SOURCES += \
	intel/include/bdw.h \
	intel/include/nhm.h \
	intel/include/skl.h \
	intel/include/snb.h \
	intel/include/types.h \
	intel/include/util.h \
	intel/include/wsm.h \
	intel/bdw.c \
	intel/nhm.c \
	intel/plat.c \
	intel/skl.c \
	intel/snb.c \
	intel/ui_perf_map.c \
	intel/util.c \
	intel/wsm.c
endif

bin_PROGRAMS = numatop
noinst_PROGRAMS = pmmeter

numatop_CFLAGS = $(NCURSES_CFLAGS)
numatop_LDADD = libnumatop.la $(NCURSES_LIBS)
numatop_SOURCES = common/numatop.c

pmmeter_CFLAGS = $(NCURSES_CFLAGS)
pmmeter_LDADD = libnumatop.la $(NCURSES_LIBS)
pmmeter_SOURCES = \
	src/include/util.h \
	src/pmmeter.c

if CPU_INTEL
pmmeter_SOURCES += \
	src/intel/util.c
endif

TESTS = test/mgen.01.sh test/mgen.02.sh
