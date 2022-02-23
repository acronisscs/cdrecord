#!/bin/sh

# cd src/i686-w64-mingw32/cdrtools
# find . -lname '../*' -ls | awk '{$1=$2=$3=$4=$5=$6=$7=$8=$9=$10=""; print $0}' | sed 's/ -> / /' | awk '{ print ln $2,$1 }'

LINKS=(
'../conf/acgeneral.m4 cdda2wav/acgeneral.m4' \
'../conf/aclocal.m4 cdda2wav/aclocal.m4' \
'../libparanoia/cdda_paranoia.h cdda2wav/cdda_paranoia.h' \
'../cdrecord/cd_misc.c cdda2wav/cd_misc.c' \
'../conf/config.guess cdda2wav/config.guess' \
'../conf/config.sub cdda2wav/config.sub' \
'../cdrecord/defaults.c cdda2wav/defaults.c' \
'../cdrecord/getnum.c cdda2wav/getnum.c' \
'../conf/install-sh cdda2wav/install-sh' \
'../cdrecord/modes.c cdda2wav/modes.c' \
'../cdrecord/scsi_cdr.c cdda2wav/scsi_cdr.c' \
'../cdrecord/scsi_scan.c cdda2wav/scsi_scan.c' \
'../cdrecord/scsi_scan.h cdda2wav/scsi_scan.h' \
'../libscg/scg include/scg' \
'../cdrecord/cd_misc.c mkisofs/cd_misc.c' \
'../cdrecord/defaults.c mkisofs/defaults.c' \
'../cd_misc.c mkisofs/diag/cd_misc.c' \
'../defaults.c mkisofs/diag/defaults.c' \
'../getnum.c mkisofs/diag/getnum.c' \
'../modes.c mkisofs/diag/modes.c' \
'../scsi.c mkisofs/diag/scsi.c' \
'../scsi_cdr.c mkisofs/diag/scsi_cdr.c' \
'../cdrecord/getnum.c mkisofs/getnum.c' \
'../cdrecord/modes.c mkisofs/modes.c' \
'../cdrecord/scsi_cdr.c mkisofs/scsi_cdr.c' \
'../cdrecord/cd_misc.c readcd/cd_misc.c' \
'../cdrecord/defaults.c readcd/defaults.c' \
'../cdrecord/getnum.c readcd/getnum.c' \
'../cdrecord/misc.c readcd/misc.c' \
'../cdrecord/modes.c readcd/modes.c' \
'../cdrecord/movesect.c readcd/movesect.c' \
'../cdrecord/movesect.h readcd/movesect.h' \
'../cdrecord/scsi_cdr.c readcd/scsi_cdr.c' \
'../cdrecord/scsi_mmc.c readcd/scsi_mmc.c' \
'../cdrecord/scsi_scan.c readcd/scsi_scan.c' \
'../cdrecord/scsi_scan.h readcd/scsi_scan.h' \
'../cdrecord/cd_misc.c scgcheck/cd_misc.c' \
'../cdrecord/modes.c scgcheck/modes.c' \
'../cdrecord/scsi_cdr.c scgcheck/scsi_cdr.c' \
'../cdrecord/scsi_scan.c scgcheck/scsi_scan.c' \
'../cdrecord/cdrecord.h scgskeleton/cdrecord.h' \
'../cdrecord/cd_misc.c scgskeleton/cd_misc.c' \
'../cdrecord/defaults.c scgskeleton/defaults.c' \
'../cdrecord/defaults.h scgskeleton/defaults.h' \
'../cdrecord/getnum.c scgskeleton/getnum.c' \
'../readcd/io.c scgskeleton/io.c' \
'../cdrecord/misc.c scgskeleton/misc.c' \
'../cdrecord/modes.c scgskeleton/modes.c' \
'../cdrecord/scsimmc.h scgskeleton/scsimmc.h' \
'../cdrecord/scsi_cdr.c scgskeleton/scsi_cdr.c' \
'../cdrecord/scsi_scan.c scgskeleton/scsi_scan.c' \
'../cdrecord/scsi_scan.h scgskeleton/scsi_scan.h' \
)

fix_links() {
  cd $1
  for l in "${LINKS[@]}"; do
    read -r -a from_to <<< "$l"    
    from=${from_to[0]}
    to=${from_to[1]}

    from_real=$(cd $(dirname $to)/$(dirname $from); pwd )/$(basename $from)
    if [ -f $from_real ]; then
      if [ ! -f $to ]; then
        echo "FILE $from_real -> $to"
        ln $from_real $to
      fi
    else
      echo "DIR $from_real -> $to"
      ln -s $from_real $to
    fi

  done
  cd -
}
