#!/bin/sh

fix_links() {
  pushd $1

  cd scgskeleton
  ln ../cdrecord/defaults.c defaults.c
  ln ../cdrecord/getnum.c getnum.c
  ln ../cdrecord/misc.c misc.c
  ln ../cdrecord/modes.c modes.c
  ln ../cdrecord/scsimmc.h scsimmc.h
  ln ../cdrecord/scsi_cdr.c scsi_cdr.c
  ln ../cdrecord/cd_misc.c cd_misc.c
  ln ../readcd/io.c io.c
  ln ../cdrecord/defaults.h defaults.h
  ln ../cdrecord/cdrecord.h cdrecord.h
  ln ../cdrecord/scsi_scan.c scsi_scan.c
  ln ../cdrecord/scsi_scan.h scsi_scan.h
  cd ../readcd
  ln ../cdrecord/scsi_scan.h scsi_scan.h
  ln ../cdrecord/scsi_scan.c scsi_scan.c
  ln ../cdrecord/movesect.h movesect.h
  ln ../cdrecord/movesect.c movesect.c
  ln ../cdrecord/scsi_mmc.c scsi_mmc.c
  ln ../cdrecord/getnum.c getnum.c
  ln ../cdrecord/defaults.c defaults.c
  ln ../cdrecord/scsi_cdr.c scsi_cdr.c
  ln ../cdrecord/modes.c modes.c
  ln ../cdrecord/misc.c misc.c
  ln ../cdrecord/cd_misc.c cd_misc.c
  cd ../scgcheck
  ln ../cdrecord/scsi_scan.c scsi_scan.c
  ln ../cdrecord/scsi_cdr.c scsi_cdr.c
  ln ../cdrecord/modes.c modes.c
  ln ../cdrecord/cd_misc.c cd_misc.c
  cd ../cdda2wav
  ln ../cdrecord/scsi_scan.h scsi_scan.h
  ln ../cdrecord/scsi_scan.c scsi_scan.c
  ln ../cdrecord/getnum.c getnum.c
  ln ../cdrecord/defaults.c defaults.c
  ln ../libparanoia/cdda_paranoia.h cdda_paranoia.h
  ln ../conf/aclocal.m4 aclocal.m4
  ln ../conf/acgeneral.m4 acgeneral.m4
  ln ../conf/config.guess config.guess
  ln ../conf/config.sub config.sub
  ln ../conf/install-sh install-sh
  ln ../cdrecord/cd_misc.c cd_misc.c
  ln ../cdrecord/modes.c modes.c
  ln ../cdrecord/scsi_cdr.c scsi_cdr.c
  cd ../mkisofs
  ln ../cdrecord/getnum.c getnum.c
  ln ../cdrecord/defaults.c defaults.c
  ln ../cdrecord/scsi_cdr.c scsi_cdr.c
  ln ../cdrecord/modes.c modes.c
  ln ../cdrecord/cd_misc.c cd_misc.c
  cd diag
  ln ../modes.c modes.c
  ln ../defaults.c defaults.c
  ln ../getnum.c getnum.c
  ln ../cd_misc.c cd_misc.c
  ln ../scsi_cdr.c scsi_cdr.c
  ln ../scsi.c scsi.c
  cd ../../include
  ln -s ../libscg/scg scg
  cd ../TEMPLATES
  ln Makefile.inc inc.mk
  ln Makefile.shl shl.mk
  ln Makefile.lib lib.mk
  ln Makefile.cmd cmd.mk
  cd ../DEFAULTS_ENG
  ln Defaults.dgux Defaults.dgux4
  ln Defaults.dgux Defaults.dgux3
  cd ../DEFAULTS
  ln Defaults.dgux Defaults.dgux4
  ln Defaults.dgux Defaults.dgux3
  cd ../RULES
  ln i586-linux-gcc.rul x86_64-linux-gcc.rul
  ln i586-linux-cc.rul x86_64-linux-cc.rul
  ln i486-cygwin32_nt-gcc.rul i386-cygwin32_nt-gcc.rul
  ln i486-cygwin32_nt-cc.rul i386-cygwin32_nt-cc.rul
  ln i386-ms-dos-gcc.rul i786-ms-dos-gcc.rul
  ln i386-ms-dos-gcc.rul i686-ms-dos-gcc.rul
  ln i386-ms-dos-gcc.rul i586-ms-dos-gcc.rul
  ln i386-ms-dos-gcc.rul i486-ms-dos-gcc.rul
  ln i386-unixware-gcc.rul pentium-pro-unixware-gcc.rul
  ln i386-unixware-cc.rul pentium-pro-unixware-cc.rul
  ln i386-unixware-gcc.rul i586-unixware-gcc.rul
  ln i386-unixware-cc.rul i586-unixware-cc.rul
  ln i386-unixware-gcc.rul i486-unixware-gcc.rul
  ln i386-unixware-cc.rul i486-unixware-cc.rul
  ln i386-unixware-gcc.rul pentium-iii-unixware-gcc.rul
  ln i386-unixware-cc.rul pentium-iii-unixware-cc.rul
  ln i386-netbsd-gcc.rul macppc-netbsd-gcc.rul
  ln i386-netbsd-cc.rul macppc-netbsd-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-831-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-831-hp-ux-cc.rul
  ln i386-bsd-os3-gcc.rul sparc-bsd-os3-gcc.rul
  ln i386-bsd-os3-cc.rul sparc-bsd-os3-cc.rul
  ln i386-bsd-os-cc.rul sparc-bsd-os-cc.rul
  ln i486-cygwin32_nt-gcc.rul i786-cygwin32_nt-gcc.rul
  ln i486-cygwin32_nt-cc.rul i786-cygwin32_nt-cc.rul
  ln i386-freebsd-gcc.rul sparc64-freebsd-gcc.rul
  ln i386-freebsd-cc.rul sparc64-freebsd-cc.rul
  ln i586-linux-gcc.rul ia64-linux-gcc.rul
  ln i586-linux-cc.rul ia64-linux-cc.rul
  ln os-cygwin32_nt.id os-cygwin_nt-5.1.id
  ln i586-linux-gcc.rul parisc64-linux-gcc.rul
  ln i586-linux-cc.rul parisc64-linux-cc.rul
  ln i586-linux-gcc.rul parisc-linux-gcc.rul
  ln i586-linux-cc.rul parisc-linux-cc.rul
  ln i586-linux-gcc.rul s390-linux-gcc.rul
  ln i586-linux-cc.rul s390-linux-cc.rul
  ln ip22-irix-gcc.rul ip17-irix-gcc.rul
  ln ip22-irix-cc.rul ip17-irix-cc.rul
  ln os-unixware.id os-openunix.id
  ln i586-linux-gcc.rul mipsel-linux-gcc.rul
  ln i586-linux-cc.rul mipsel-linux-cc.rul
  ln i586-linux-gcc.rul mips-linux-gcc.rul
  ln i586-linux-cc.rul mips-linux-cc.rul
  ln os-cygwin32_nt.id os-cygwin_me-4.90.id
  ln ip22-irix-gcc.rul ip30-irix-gcc.rul
  ln ip22-irix-gcc.rul ip28-irix-gcc.rul
  ln ip22-irix-gcc.rul ip27-irix-gcc.rul
  ln ip22-irix-gcc.rul ip20-irix-gcc.rul
  ln ip22-irix-gcc.rul ip32-irix-gcc.rul
  ln os-cygwin32_nt.id os-cygwin_nt-5.0.id
  ln os-mingw32_nt.id os-mingw32_nt-6.0.id
  ln os-mingw32_nt.id os-mingw32_nt-6.1.id
  ln os-mingw32_nt.id os-mingw32_nt-6.2.id
  ln os-cygwin32_nt.id os-cygwin_98-4.10.id
  ln 9000-725-hp-ux-gcc.rul 9000-800-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-800-hp-ux-cc.rul
  ln ip22-irix-cc.rul ip27-irix-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-785-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-785-hp-ux-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-778-hp-ux-gcc.rul
  ln power-macintosh-rhapsody-gcc.rul power-macintosh-mac-os10-gcc.rul
  ln power-macintosh-rhapsody-cc.rul power-macintosh-mac-os10-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-820-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-820-hp-ux-cc.rul
  ln i386-netbsd-gcc.rul alpha-netbsd-gcc.rul
  ln i386-netbsd-cc.rul alpha-netbsd-cc.rul
  ln i386-netbsd-gcc.rul amiga-netbsd-gcc.rul
  ln i386-netbsd-cc.rul amiga-netbsd-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-899-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-899-hp-ux-cc.rul
  ln 9000-725-hp-ux-cc.rul 9000-778-hp-ux-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-777-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-777-hp-ux-cc.rul
  ln i586-linux-gcc.rul sparc64-linux-gcc.rul
  ln i586-linux-cc.rul sparc64-linux-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-782-hp-ux-gcc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-780-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-782-hp-ux-cc.rul
  ln 9000-725-hp-ux-cc.rul 9000-780-hp-ux-cc.rul
  ln os-cygwin32_nt.id os-cygwin_98-4.0.id
  ln os-cygwin32_nt.id os-cygwin_95-4.0.id
  ln ip22-irix-cc.rul ip28-irix-cc.rul
  ln ip22-irix-cc.rul ip20-irix-cc.rul
  ln i586-linux-gcc.rul armv4l-linux-gcc.rul
  ln i586-linux-cc.rul armv4l-linux-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-712-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-712-hp-ux-cc.rul
  ln hppa-nextstep-cc.rul sparc-nextstep-gcc.rul
  ln hppa-nextstep-cc.rul m68k-nextstep-gcc.rul
  ln hppa-nextstep-cc.rul i386-nextstep-gcc.rul
  ln hppa-nextstep-cc.rul sparc-nextstep-cc.rul
  ln hppa-nextstep-cc.rul m68k-nextstep-cc.rul
  ln os-irix.id os-irix64.id
  ln hppa-nextstep-cc.rul i386-nextstep-cc.rul
  ln os-cygwin32_nt.id os-cygwin_nt-4.0.id
  ln i386-netbsd-gcc.rul mac68k-netbsd-gcc.rul
  ln i386-netbsd-cc.rul mac68k-netbsd-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-743-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-743-hp-ux-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-715-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-715-hp-ux-cc.rul
  ln i486-cygwin32_nt-gcc.rul i686-cygwin32_nt-gcc.rul
  ln i486-cygwin32_nt-gcc.rul i586-cygwin32_nt-gcc.rul
  ln i486-cygwin32_nt-cc.rul i686-cygwin32_nt-cc.rul
  ln i486-cygwin32_nt-cc.rul i586-cygwin32_nt-cc.rul
  ln i386-bsd-os-gcc.rul sparc-bsd-os-gcc.rul
  ln i586-linux-gcc.rul ppc-linux-gcc.rul
  ln i586-linux-cc.rul ppc-linux-cc.rul
  ln i586-linux-gcc.rul m68k-linux-gcc.rul
  ln i586-linux-cc.rul m68k-linux-cc.rul
  ln ip22-irix-cc.rul ip32-irix-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-755-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-755-hp-ux-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-735-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-735-hp-ux-cc.rul
  ln 9000-725-hp-ux-gcc.rul 9000-710-hp-ux-gcc.rul
  ln 9000-725-hp-ux-cc.rul 9000-710-hp-ux-cc.rul
  ln ip22-irix-cc.rul ip30-irix-cc.rul
  ln i386-netbsd-gcc.rul sparc-netbsd-gcc.rul
  ln i386-netbsd-cc.rul sparc-netbsd-cc.rul
  ln sun4-sunos5-gcc.rul sun4L-sunos5-gcc.rul
  ln sun4-sunos5-cc.rul sun4L-sunos5-cc.rul
  ln sun4-sunos5-gcc.rul sun4e-sunos5-gcc.rul
  ln sun4-sunos5-cc.rul sun4e-sunos5-cc.rul
  ln sun4-sunos5-gcc.rul sun4d-sunos5-gcc.rul
  ln sun4-sunos5-cc.rul sun4d-sunos5-cc.rul
  ln sun4-sunos5-gcc.rul sun4u-sunos5-gcc.rul
  ln sun4-sunos5-cc.rul sun4u-sunos5-cc.rul
  ln sun4-sunos5-gcc.rul sun4m-sunos5-gcc.rul
  ln sun4-sunos5-cc.rul sun4m-sunos5-cc.rul
  ln sun4-sunos4-gcc.rul sun4m-sunos4-gcc.rul
  ln sun4-sunos4-cc.rul sun4m-sunos4-cc.rul
  ln sun4-sunos5-gcc.rul sun4c-sunos5-gcc.rul
  ln sun4-sunos5-cc.rul sun4c-sunos5-cc.rul
  ln sun4-sunos4-gcc.rul sun4c-sunos4-gcc.rul
  ln sun4-sunos4-cc.rul sun4c-sunos4-cc.rul
  ln i586-linux-gcc.rul sparc-linux-gcc.rul
  ln i586-linux-cc.rul sparc-linux-cc.rul
  ln r-make.obj r-gmake.obj
  ln r-smake.tag r-build.tag
  ln r-smake.obj r-build.obj
  ln r-smake.dep r-build.dep
  ln mk-.id mk-make.id
  ln mk-smake.id mk-build.id
  ln i586-linux-gcc.rul i686-linux-gcc.rul
  ln i586-linux-cc.rul i686-linux-cc.rul
  ln i586-linux-gcc.rul i486-linux-gcc.rul
  ln i586-linux-cc.rul i486-linux-cc.rul
  ln i586-linux-gcc.rul i386-linux-gcc.rul
  ln i586-linux-cc.rul i386-linux-cc.rul
  ln i586-linux-gcc.rul alpha-linux-gcc.rul
  ln i586-linux-cc.rul alpha-linux-cc.rul
  cd -

  popd
}
