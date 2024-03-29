/* @(#)interface.c	1.32 03/12/27 Copyright 1998-2002 Heiko Eissfeldt */
#ifndef lint
static char     sccsid[] =
"@(#)interface.c	1.32 03/12/27 Copyright 1998-2002 Heiko Eissfeldt";

#endif
/***
 * CopyPolicy: GNU Public License 2 applies
 * Copyright (C) 1994-1997 Heiko Eissfeldt heiko@colossus.escape.de
 *
 * Interface module for cdrom drive access
 *
 * Two interfaces are possible.
 *
 * 1. using 'cooked' ioctls() (Linux only)
 *    : available for atapi, sbpcd and cdu31a drives only.
 *
 * 2. using the generic scsi device (for details see SCSI Prog. HOWTO).
 *    NOTE: a bug/misfeature in the kernel requires blocking signal
 *          SIGINT during SCSI command handling. Once this flaw has
 *          been removed, the sigprocmask SIG_BLOCK and SIG_UNBLOCK calls
 *          should removed, thus saving context switches.
 *
 * For testing purposes I have added a third simulation interface.
 *
 * Version 0.8: used experiences of Jochen Karrer.
 *              SparcLinux port fixes
 *              AlphaLinux port fixes
 *
 */
#if 0
#define SIM_CD
#endif

#include "config.h"
#include <stdio.h>
#include <standard.h>
#include <stdxlib.h>
#include <unixstd.h>
#include <strdefs.h>
#include <errno.h>
#include <signal.h>
#include <fctldefs.h>
#include <assert.h>
#include <schily.h>
#include <device.h>

#include <sys/ioctl.h>
#include <statdefs.h>


#include "mycdrom.h"
#include "lowlevel.h"
/* some include file locations have changed with newer kernels */
#if defined (__linux__)
# if LINUX_VERSION_CODE > 0x10300 + 97
#  if LINUX_VERSION_CODE < 0x200ff
#   include <linux/sbpcd.h>
#   include <linux/ucdrom.h>
#  endif
#  if !defined(CDROM_SELECT_SPEED)
#   include <linux/ucdrom.h>
#  endif
# endif
#endif

#include <scg/scsitransp.h>

#include "mytype.h"
#include "byteorder.h"
#include "interface.h"
#include "cdda2wav.h"
#include "semshm.h"
#include "setuid.h"
#include "ringbuff.h"
#include "toc.h"
#include "global.h"
#include "ioctl.h"
#include "exitcodes.h"
#include "scsi_cmds.h"

#include <utypes.h>
#include <cdrecord.h>
#include "scsi_scan.h"

unsigned interface;

int trackindex_disp = 0;

void     (*EnableCdda) __PR((SCSI *, int Switch, unsigned uSectorsize));
unsigned (*doReadToc) __PR(( SCSI *scgp ));
void	 (*ReadTocText) __PR(( SCSI *scgp ));
unsigned (*ReadLastAudio) __PR(( SCSI *scgp ));
int      (*ReadCdRom) __PR((SCSI *scgp, UINT4 *p, unsigned lSector, unsigned SectorBurstVal ));
int      (*ReadCdRomData) __PR((SCSI *scgp, unsigned char *p, unsigned lSector, unsigned SectorBurstVal ));
int      (*ReadCdRomSub) __PR((SCSI *scgp, UINT4 *p, unsigned lSector, unsigned SectorBurstVal ));
subq_chnl *(*ReadSubChannels) __PR(( SCSI *scgp, unsigned lSector ));
subq_chnl *(*ReadSubQ) __PR(( SCSI *scgp, unsigned char sq_format, unsigned char track ));
void     (*SelectSpeed) __PR(( SCSI *scgp, unsigned speed ));
int	(*Play_at) __PR(( SCSI *scgp, unsigned int from_sector, unsigned int sectors));
int	(*StopPlay) __PR(( SCSI *scgp));
void	(*trash_cache) __PR((UINT4 *p, unsigned lSector, unsigned SectorBurstVal));

#if	defined	USE_PARANOIA
long cdda_read __PR((void *d, void * buffer, long beginsector, long sectors));

long cdda_read(d, buffer, beginsector, sectors)
	void *d;
	void * buffer;
	long beginsector;
	long sectors;
{
	long ret = ReadCdRom(d, buffer, beginsector, sectors);
	return ret;
}
#endif

typedef struct string_len {
  char *str;
  unsigned int sl;
} mystring;

static mystring drv_is_not_mmc[] = {
	{"DEC     RRD47   (C) DEC ",24},
/*	{"SONY    CD-ROM CDU625    1.0",28}, */
	{NULL,0}	/* must be last entry */
};

static mystring drv_has_mmc_cdda[] = {
	{"HITACHI CDR-7930",16},
/*	{"TOSHIBA CD-ROM XM-5402TA3605",28}, */
	{NULL,0}	/* must be last entry */
};

static int	Is_a_Toshiba3401;

int Toshiba3401 __PR((void));

int Toshiba3401 ( ) {
  return Is_a_Toshiba3401;
}

/* hook */
static void Dummy __PR(( void ));
static void Dummy ( )
{
}

static SCSI    *scgp;

SCSI * get_scsi_p __PR((void));

SCSI * get_scsi_p( )
{
    return scgp;
}

#if !defined(SIM_CD)

static void trash_cache_SCSI __PR((UINT4 *p, unsigned lSector, unsigned SectorBurstVal));

static void trash_cache_SCSI(p, lSector, SectorBurstVal)
	UINT4 *p;
	unsigned lSector;
	unsigned SectorBurstVal;
{
      /* trash the cache */
      ReadCdRom(get_scsi_p(), p, find_an_off_sector(lSector, SectorBurstVal), min(global.nsectors,6));
}



static void Check_interface_for_device __PR((struct stat *statstruct, char *pdev_name));
static int OpenCdRom __PR((char *pdev_name));

static void SetupSCSI __PR((void));

static void SetupSCSI( )
{
    unsigned char *p;

    if (interface != GENERIC_SCSI) {
	/* unfortunately we have the wrong interface and are
	 * not able to change on the fly */
	fprintf(stderr, "The generic SCSI interface and devices are required\n");
	exit(SYNTAX_ERROR);
    }

    /* do a test unit ready to 'init' the device. */
    TestForMedium(scgp);

    /* check for the correct type of unit. */
    p = Inquiry(scgp);

#undef TYPE_ROM
#define TYPE_ROM 5
#undef TYPE_WORM
#define TYPE_WORM  4
    if (p == NULL) {
	fprintf(stderr, "Inquiry command failed. Aborting...\n");
	exit(DEVICE_ERROR);
    }

    if ((*p != TYPE_ROM && *p != TYPE_WORM)) {
	fprintf(stderr, "this is neither a scsi cdrom nor a worm device\n");
	exit(SYNTAX_ERROR);
    }

    if (global.quiet == 0) {
	fprintf(stderr,
		 "Type: %s, Vendor '%8.8s' Model '%16.16s' Revision '%4.4s' ",
		 *p == TYPE_ROM ? "ROM" : "WORM"
		 ,p+8
		 ,p+16
		 ,p+32);
    }
    /* generic Sony type defaults */
    density = 0x0;
    accepts_fua_bit = -1;
    EnableCdda = (void (*) __PR((SCSI *, int, unsigned)))Dummy;
    ReadCdRom = ReadCdda12;
    ReadCdRomSub = ReadCddaSubSony;
    ReadCdRomData = (int (*) __PR((SCSI *, unsigned char *, unsigned, unsigned ))) ReadStandardData;
    ReadLastAudio = ReadFirstSessionTOCSony;
    SelectSpeed = SpeedSelectSCSISony;
    Play_at = Play_atSCSI;
    StopPlay = StopPlaySCSI;
    trash_cache = trash_cache_SCSI;
    ReadTocText = ReadTocTextSCSIMMC;
    doReadToc = ReadTocSCSI;
    ReadSubQ = ReadSubQSCSI;
    ReadSubChannels = NULL;

    /* check for brands and adjust special peculiaritites */

    /* If your drive is not treated correctly, you can adjust some things
       here:

       global.in_lendian: should be to 1, if the CDROM drive or CD-Writer
		  delivers the samples in the native byteorder of the audio cd
		  (LSB first).
		  HP CD-Writers need it set to 0.
       NOTE: If you get correct wav files when using sox with the '-x' option,
             the endianess is wrong. You can use the -C option to specify
	     the value of global.in_lendian.

     */

    {
      int mmc_code;

      scgp->silent ++;
      allow_atapi(scgp, 1);
      if (*p == TYPE_ROM) {
        mmc_code = heiko_mmc(scgp);
      } else {
        mmc_code = 0;
      }
      scgp->silent --;

      /* Exceptions for drives that report incorrect MMC capability */
      if (mmc_code != 0) {
	/* these drives are NOT capable of MMC commands */
        mystring *pp = drv_is_not_mmc;
	while (pp->str != NULL) {
	  if (!strncmp(pp->str, (char *)p+8,pp->sl)) {
	    mmc_code = 0;
	    break;
	  }
	  pp++;
        }
      }
      {
	/* these drives flag themselves as non-MMC, but offer CDDA reading
	   only with a MMC method. */
        mystring *pp = drv_has_mmc_cdda;
	while (pp->str != NULL) {
	  if (!strncmp(pp->str, (char *)p+8,pp->sl)) {
	    mmc_code = 1;
	    break;
	  }
	  pp++;
        }
      }

      switch (mmc_code) {
       case 2:      /* SCSI-3 cdrom drive with accurate audio stream */
	/* fall through */
       case 1:      /* SCSI-3 cdrom drive with no accurate audio stream */
	/* fall through */
lost_toshibas:
	 global.in_lendian = 1;
         if (mmc_code == 2)
	   global.overlap = 0;
	 else
           global.overlap = 1;
         ReadCdRom = ReadCddaFallbackMMC;
	 ReadCdRomSub = ReadCddaSubSony;
         ReadLastAudio = ReadFirstSessionTOCMMC;
         SelectSpeed = SpeedSelectSCSIMMC;
    	 ReadTocText = ReadTocTextSCSIMMC;
	 doReadToc = ReadTocMMC;
	 ReadSubChannels = ReadSubChannelsFallbackMMC;
	 if (!memcmp(p+8,"SONY    CD-RW  CRX100E  1.0", 27)) ReadTocText = NULL;
	 if (!global.quiet) fprintf(stderr, "MMC+CDDA\n");
       break;
       case -1: /* "MMC drive does not support cdda reading, sorry\n." */
	 doReadToc = ReadTocMMC;
	 if (!global.quiet) fprintf(stderr, "MMC-CDDA\n");
	 /* FALLTHROUGH */
       case 0:      /* non SCSI-3 cdrom drive */
	 if (!global.quiet) fprintf(stderr, "no MMC\n");
         ReadLastAudio = NULL;
    if (!memcmp(p+8,"TOSHIBA", 7) ||
        !memcmp(p+8,"IBM", 3) ||
        !memcmp(p+8,"DEC", 3)) {
	    /*
	     * older Toshiba ATAPI drives don't identify themselves as MMC.
	     * The last digit of the model number is '2' for ATAPI drives.
	     * These are treated as MMC.
	     */
	    if (!memcmp(p+15, " CD-ROM XM-", 11) && p[29] == '2') {
         	goto lost_toshibas;
	    }
	density = 0x82;
	EnableCdda = EnableCddaModeSelect;
	ReadSubChannels = ReadStandardSub;
 	ReadCdRom = ReadStandard;
        SelectSpeed = SpeedSelectSCSIToshiba;
        if (!memcmp(p+15, " CD-ROM XM-3401",15)) {
	   Is_a_Toshiba3401 = 1;
	}
	global.in_lendian = 1;
    } else if (!memcmp(p+8,"IMS",3) ||
               !memcmp(p+8,"KODAK",5) ||
               !memcmp(p+8,"RICOH",5) ||
               !memcmp(p+8,"HP",2) ||
               !memcmp(p+8,"PHILIPS",7) ||
               !memcmp(p+8,"PLASMON",7) ||
               !memcmp(p+8,"GRUNDIG CDR100IPW",17) ||
               !memcmp(p+8,"MITSUMI CD-R ",13)) {
	EnableCdda = EnableCddaModeSelect;
	ReadCdRom = ReadStandard;
        SelectSpeed = SpeedSelectSCSIPhilipsCDD2600;

	/* treat all of these as bigendian */
	global.in_lendian = 0;

	/* no overlap reading for cd-writers */
	global.overlap = 0;
    } else if (!memcmp(p+8,"NRC",3)) {
        SelectSpeed = NULL;
    } else if (!memcmp(p+8,"YAMAHA",6)) {
	EnableCdda = EnableCddaModeSelect;
        SelectSpeed = SpeedSelectSCSIYamaha;

	/* no overlap reading for cd-writers */
	global.overlap = 0;
	global.in_lendian = 1;
    } else if (!memcmp(p+8,"PLEXTOR",7)) {
	global.in_lendian = 1;
	global.overlap = 0;
        ReadLastAudio = ReadFirstSessionTOCSony;
    	ReadTocText = ReadTocTextSCSIMMC;
	doReadToc = ReadTocSony;
	ReadSubChannels = ReadSubChannelsSony;
    } else if (!memcmp(p+8,"SONY",4)) {
	global.in_lendian = 1;
        if (!memcmp(p+16, "CD-ROM CDU55E",13)) {
	   ReadCdRom = ReadCddaMMC12;
	}
        ReadLastAudio = ReadFirstSessionTOCSony;
    	ReadTocText = ReadTocTextSCSIMMC;
	doReadToc = ReadTocSony;
	ReadSubChannels = ReadSubChannelsSony;
    } else if (!memcmp(p+8,"NEC",3)) {
	ReadCdRom = ReadCdda10;
        ReadTocText = NULL;
        SelectSpeed = SpeedSelectSCSINEC;
	global.in_lendian = 1;
        if (!memcmp(p+29,"5022.0r",3)) /* I assume all versions of the 502 require this? */
               global.overlap = 0;           /* no overlap reading for NEC CD-ROM 502 */
    } else if (!memcmp(p+8,"MATSHITA",8)) {
	ReadCdRom = ReadCdda12Matsushita;
	global.in_lendian = 1;
    }
    } /* switch (get_mmc) */
    }


    /* look if caddy is loaded */
    if (interface == GENERIC_SCSI) {
	scgp->silent++;
	while (!wait_unit_ready(scgp, 60)) {
		fprintf(stderr,"load cdrom please and press enter");
		getchar();
	}
	scgp->silent--;
    }
}

/********************** General setup *******************************/

/* As the name implies, interfaces and devices are checked.  We also
   adjust nsectors, overlap, and interface for the first time here.
   Any unnecessary privileges (setuid, setgid) are also dropped here.
*/
static void Check_interface_for_device( statstruct, pdev_name)
	struct stat *statstruct;
	char *pdev_name;
{

#if !defined (STAT_MACROS_BROKEN) || (STAT_MACROS_BROKEN != 1)
    if (!S_ISCHR(statstruct->st_mode) &&
	!S_ISBLK(statstruct->st_mode)) {
      fprintf(stderr, "%s is not a device\n",pdev_name);
      exit(SYNTAX_ERROR);
    }
#endif

#if defined (HAVE_ST_RDEV) && (HAVE_ST_RDEV == 1)
    switch (major(statstruct->st_rdev)) {
#if defined (__linux__)
    case SCSI_GENERIC_MAJOR:	/* generic */
#else
    default:			/* ??? what is the proper value here */
#endif
#if !defined (STAT_MACROS_BROKEN) || (STAT_MACROS_BROKEN != 1)
#if defined (__linux__)
       if (!S_ISCHR(statstruct->st_mode)) {
	 fprintf(stderr, "%s is not a char device\n",pdev_name);
	 exit(SYNTAX_ERROR);
       }

       if (interface != GENERIC_SCSI) {
	 fprintf(stderr, "wrong interface (cooked_ioctl) for this device (%s)\nset to generic_scsi\n", pdev_name);
	 interface = GENERIC_SCSI;
       }
#endif
#endif
       break;
#if defined (__linux__) || defined (__FreeBSD__)
#if defined (__linux__)
    case SCSI_CDROM_MAJOR:     /* scsi cd */
    default:			/* for example ATAPI cds */
#else
#if defined (__FreeBSD__)
    case 117:
	if (!S_ISCHR(statstruct->st_mode)) {
	    fprintf(stderr, "%s is not a char device\n",pdev_name);
	    exit(SYNTAX_ERROR);
	}
	if (interface != COOKED_IOCTL) {
	    fprintf(stderr,
"cdrom device (%s) is not of type generic SCSI. \
Setting interface to cooked_ioctl.\n", pdev_name);
	    interface = COOKED_IOCTL;
	}
	break;
    case 19:     /* first atapi cd */
#endif
#endif
	if (!S_ISBLK(statstruct->st_mode)) {
	    fprintf(stderr, "%s is not a block device\n",pdev_name);
	    exit(SYNTAX_ERROR);
	}
	if (interface != COOKED_IOCTL) {
	    fprintf(stderr, 
"cdrom device (%s) is not of type generic SCSI. \
Setting interface to cooked_ioctl.\n", pdev_name);
	    interface = COOKED_IOCTL;
	}
	break;
#endif
    }
#endif
    if (global.overlap >= global.nsectors)
      global.overlap = global.nsectors-1;
}

/* open the cdrom device */
static int OpenCdRom ( pdev_name )
	char *pdev_name;
{
  int retval = 0;
  struct stat fstatstruct;

  /*  The device (given by pdevname) can be:
      a. an SCSI device specified with a /dev/xxx name,
      b. an SCSI device specified with bus,target,lun numbers,
      c. a non-SCSI device such as ATAPI or proprietary CDROM devices.
   */
#ifdef HAVE_IOCTL_INTERFACE
  struct stat statstruct;
  int have_named_device = 0;

	have_named_device = FALSE;
	if (pdev_name) {
		have_named_device = strchr(pdev_name, ':') == NULL
					&& memcmp(pdev_name, "/dev/", 5) == 0;
	}

  if (have_named_device) {
    if (stat(pdev_name, &statstruct)) {
      fprintf(stderr, "cannot stat device %s\n", pdev_name);
      exit(STAT_ERROR);
    } else {
      Check_interface_for_device( &statstruct, pdev_name );
    }
  }
#endif

  if (interface == GENERIC_SCSI) {
	char	errstr[80];

	needroot(0);
	needgroup(0);
	/*
	 * Call scg_remote() to force loading the remote SCSI transport library
	 * code that is located in librscg instead of the dummy remote routines
	 * that are located inside libscg.
	 */
	scg_remote();
	if (pdev_name != NULL &&
	    ((strncmp(pdev_name, "HELP", 4) == 0) ||
	     (strncmp(pdev_name, "help", 4) == 0))) {
		scg_help(stderr);
		exit(NO_ERROR);
	}

	/* device name, debug, verboseopen */
	scgp = scg_open(pdev_name, errstr, sizeof(errstr), 0, 0);

	if (scgp == NULL) {
		int	err = geterrno();

		errmsgno(err, "%s%sCannot open SCSI driver.\n", errstr, errstr[0]?". ":"");
		errmsgno(EX_BAD, "For possible targets try 'cdda2wav -scanbus'.%s\n",
					geteuid() ? " Make sure you are root.":"");
        	dontneedgroup();
        	dontneedroot();
#if defined(sun) || defined(__sun)
		fprintf(stderr, "On SunOS/Solaris make sure you have Joerg Schillings scg SCSI driver installed.\n");
#endif
#if defined (__linux__)
	        fprintf(stderr, "Use the script scan_scsi.linux to find out more.\n");
#endif
	        fprintf(stderr, "Probably you did not define your SCSI device.\n");
	        fprintf(stderr, "Set the CDDA_DEVICE environment variable or use the -D option.\n");
	        fprintf(stderr, "You can also define the default device in the Makefile.\n");
		fprintf(stderr, "For possible transport specifiers try 'cdda2wav dev=help'.\n");
	        exit(SYNTAX_ERROR);
	}
	scg_settimeout(scgp, 300);
	scg_settimeout(scgp, 60);
	scgp->silent = global.scsi_silent;
	scgp->verbose = global.scsi_verbose;
      dontneedgroup();
      dontneedroot();
      if (global.nsectors > (unsigned) scg_bufsize(scgp, 3*1024*1024)/CD_FRAMESIZE_RAW)
        global.nsectors = scg_bufsize(scgp, 3*1024*1024)/CD_FRAMESIZE_RAW;
      if (global.overlap >= global.nsectors)
        global.overlap = global.nsectors-1;

	init_scsibuf(scgp, global.nsectors*CD_FRAMESIZE_RAW);
	if (global.scanbus) {
		select_target(scgp, stdout);
		exit(0);
	}
  } else {
      needgroup(0);
      retval = open(pdev_name,O_RDONLY
#ifdef	linux
				| O_NONBLOCK
#endif
	);
      dontneedgroup();

      if (retval < 0) {
        fprintf(stderr, "while opening %s :", pdev_name);
        perror("");
        exit(DEVICEOPEN_ERROR);
      }

      /* Do final security checks here */
      if (fstat(retval, &fstatstruct)) {
        fprintf(stderr, "Could not fstat %s (fd %d): ", pdev_name, retval);
        perror("");
        exit(STAT_ERROR);
      }
      Check_interface_for_device( &fstatstruct, pdev_name );

#if defined HAVE_IOCTL_INTERFACE
      /* Watch for race conditions */
      if (have_named_device 
          && (fstatstruct.st_dev != statstruct.st_dev ||
              fstatstruct.st_ino != statstruct.st_ino)) {
         fprintf(stderr,"Race condition attempted in OpenCdRom.  Exiting now.\n");
         exit(RACE_ERROR);
      }
#endif
	/*
	 * The structure looks like a desaster :-(
	 * We do this more than once as it is impossible to understand where
	 * the right place would be to do this....
	 */
	if (scgp != NULL) {
		scgp->verbose = global.scsi_verbose;
	}
  }
  return retval;
}
#endif /* SIM_CD */

/******************* Simulation interface *****************/
#if	defined SIM_CD
#include "toc.h"
static unsigned long sim_pos=0;

/* read 'SectorBurst' adjacent sectors of audio sectors 
 * to Buffer '*p' beginning at sector 'lSector'
 */
static int ReadCdRom_sim __PR(( SCSI *x, UINT4 *p, unsigned lSector, unsigned SectorBurstVal));
static int ReadCdRom_sim (x, p, lSector, SectorBurstVal )
	SCSI *x;
	UINT4 *p;
	unsigned lSector;
	unsigned SectorBurstVal;
{
  unsigned int loop=0;
  Int16_t *q = (Int16_t *) p;
  int joffset = 0;

  if (lSector > g_toc[cdtracks].dwStartSector || lSector + SectorBurstVal > g_toc[cdtracks].dwStartSector + 1) {
    fprintf(stderr, "Read request out of bounds: %u - %u (%d - %d allowed)\n",
	lSector, lSector + SectorBurstVal, 0, g_toc[cdtracks].dwStartSector);
  }
#if 0
  /* jitter with a probability of jprob */
  if (random() <= jprob) {
    /* jitter up to jmax samples */
    joffset = random();
  }
#endif

#ifdef DEBUG_SHM
  fprintf(stderr, ", last_b = %p\n", *last_buffer);
#endif
  for (loop = lSector*CD_FRAMESAMPLES + joffset; 
       loop < (lSector+SectorBurstVal)*CD_FRAMESAMPLES + joffset; 
       loop++) {
    *q++ = loop;
    *q++ = ~loop;
  }
#ifdef DEBUG_SHM
  fprintf(stderr, "sim wrote from %p upto %p - 4 (%d), last_b = %p\n",
          p, q, SectorBurstVal*CD_FRAMESAMPLES, *last_buffer);
#endif
  sim_pos = (lSector+SectorBurstVal)*CD_FRAMESAMPLES + joffset; 
  return SectorBurstVal;
}

static int Play_at_sim __PR(( SCSI *x, unsigned int from_sector, unsigned int sectors));
static int Play_at_sim( x, from_sector, sectors)
	SCSI *x;
	unsigned int from_sector;
	unsigned int sectors;
{
  sim_pos = from_sector*CD_FRAMESAMPLES; 
  return 0;
}

static unsigned sim_indices;


/* read the table of contents (toc) via the ioctl interface */
static unsigned ReadToc_sim __PR(( SCSI *x, TOC *toc));
static unsigned ReadToc_sim ( x, toc )
	SCSI *x;
	TOC *toc;
{
    unsigned int scenario;
    int scen[12][3] = { 
      {1,1,500}, 
      {1,2,500}, 
      {1,99,150*99}, 
      {2,1,500}, 
      {2,2,500}, 
      {2,99,150*99},
      {2,1,500}, 
      {5,2,500}, 
      {5,99,150*99}, 
      {99,1,1000}, 
      {99,2,1000}, 
      {99,99,150*99}, 
    };
    unsigned int i;
    unsigned trcks;
#if 0
    fprintf(stderr, "select one of the following TOCs\n"
	    "0 :  1 track  with  1 index\n"
	    "1 :  1 track  with  2 indices\n"
	    "2 :  1 track  with 99 indices\n"
	    "3 :  2 tracks with  1 index each\n"
	    "4 :  2 tracks with  2 indices each\n"
	    "5 :  2 tracks with 99 indices each\n"
	    "6 :  2 tracks (data and audio) with  1 index each\n"
	    "7 :  5 tracks with  2 indices each\n"
	    "8 :  5 tracks with 99 indices each\n"
	    "9 : 99 tracks with  1 index each\n"
	    "10: 99 tracks with  2 indices each\n"
	    "11: 99 tracks with 99 indices each\n"
	    );

    do {
      scanf("%u", &scenario);
    } while (scenario > sizeof(scen)/2/sizeof(int));
#else
    scenario = 6;
#endif
    /* build table of contents */

#if 0
    trcks = scen[scenario][0] + 1;
    sim_indices = scen[scenario][1];

    for (i = 0; i < trcks; i++) {
        toc[i].bFlags = (scenario == 6 && i == 0) ? 0x40 : 0xb1;
        toc[i].bTrack = i + 1;
        toc[i].dwStartSector = i * scen[scenario][2];
        toc[i].mins = (toc[i].dwStartSector+150) / (60*75);
        toc[i].secs = (toc[i].dwStartSector+150 / 75) % (60);
        toc[i].frms = (toc[i].dwStartSector+150) % (75);
    }
    toc[i].bTrack = 0xaa;
    toc[i].dwStartSector = i * scen[scenario][2];
    toc[i].mins = (toc[i].dwStartSector+150) / (60*75);
    toc[i].secs = (toc[i].dwStartSector+150 / 75) % (60);
    toc[i].frms = (toc[i].dwStartSector+150) % (75);
#else
    {
      int starts[15] = { 23625, 30115, 39050, 51777, 67507, 
		88612, 112962, 116840, 143387, 162662,
		173990, 186427, 188077, 209757, 257120};
      trcks = 14 + 1;
      sim_indices = 1;

      for (i = 0; i < trcks; i++) {
        toc[i].bFlags = 0x0;
        toc[i].bTrack = i + 1;
        toc[i].dwStartSector = starts[i];
        toc[i].mins = (starts[i]+150) / (60*75);
        toc[i].secs = (starts[i]+150 / 75) % (60);
        toc[i].frms = (starts[i]+150) % (75);
      }
      toc[i].bTrack = 0xaa;
      toc[i].dwStartSector = starts[i];
      toc[i].mins = (starts[i]) / (60*75);
      toc[i].secs = (starts[i] / 75) % (60);
      toc[i].frms = (starts[i]) % (75);
    }
#endif
    return --trcks;           /* without lead-out */
}


static subq_chnl *ReadSubQ_sim __PR(( SCSI *scgp, unsigned char sq_format, unsigned char track ));
/* request sub-q-channel information. This function may cause confusion
 * for a drive, when called in the sampling process.
 */
static subq_chnl *ReadSubQ_sim ( scgp, sq_format, track )
	SCSI *scgp;
	unsigned char sq_format;
	unsigned char track;
{
    subq_chnl *SQp = (subq_chnl *) (SubQbuffer);
    subq_position *SQPp = (subq_position *) &SQp->data;
    unsigned long sim_pos1;
    unsigned long sim_pos2;

    if ( sq_format != GET_POSITIONDATA ) return NULL;  /* not supported by sim */

    /* simulate CDROMSUBCHNL ioctl */

    /* copy to SubQbuffer */
    SQp->audio_status 	= 0;
    SQp->format 	= 0xff;
    SQp->control_adr	= 0xff;
    sim_pos1 = sim_pos/CD_FRAMESAMPLES;
    sim_pos2 = sim_pos1 % 150;
    SQp->track 		= (sim_pos1 / 5000) + 1;
    SQp->index 		= ((sim_pos1 / 150) % sim_indices) + 1;
    sim_pos1 += 150;
    SQPp->abs_min 	= sim_pos1 / (75*60);
    SQPp->abs_sec 	= (sim_pos1 / 75) % 60;
    SQPp->abs_frame 	= sim_pos1 % 75;
    SQPp->trel_min 	= sim_pos2 / (75*60);
    SQPp->trel_sec 	= (sim_pos2 / 75) % 60;
    SQPp->trel_frame 	= sim_pos2 % 75;

    return (subq_chnl *)(SubQbuffer);
}

static void SelectSpeed_sim __PR(( SCSI *x, unsigned sp));
/* ARGSUSED */
static void SelectSpeed_sim(x, sp)
	SCSI *x;
	unsigned sp;
{
}

static void trash_cache_sim __PR((UINT4 *p, unsigned lSector, unsigned SectorBurstVal));

/* ARGSUSED */
static void trash_cache_sim(p, lSector, SectorBurstVal)
	UINT4 *p;
	unsigned lSector;
	unsigned SectorBurstVal;
{
}

static void SetupSimCd __PR((void));

static void SetupSimCd()
{
    EnableCdda = (void (*) __PR((SCSI *, int, unsigned)))Dummy;
    ReadCdRom = ReadCdRom_sim;
    ReadCdRomData = (int (*) __PR((SCSI *, unsigned char *, unsigned, unsigned ))) ReadCdRom_sim;
    doReadToc = ReadToc_sim;
    ReadTocText = NULL;
    ReadSubQ = ReadSubQ_sim;
    ReadSubChannels = NULL;
    ReadLastAudio = NULL;
    SelectSpeed = SelectSpeed_sim;
    Play_at = Play_at_sim;
    StopPlay = (int (*) __PR((SCSI *)))Dummy;
    trash_cache = trash_cache_sim;
 
}

#endif /* def SIM_CD */

/* perform initialization depending on the interface used. */
void SetupInterface( )
{
#if	defined SIM_CD
    fprintf( stderr, "SIMULATION MODE !!!!!!!!!!!\n");
#else
    /* ensure interface is setup correctly */
    global.cooked_fd = OpenCdRom ( global.dev_name );
#endif

#ifdef  _SC_PAGESIZE
    global.pagesize = sysconf(_SC_PAGESIZE);
#else
    global.pagesize = getpagesize();
#endif

    /* request one sector for table of contents */
    bufferTOC = malloc( CD_FRAMESIZE_RAW + 96 );      /* assumes sufficient aligned addresses */
    /* SubQchannel buffer */
    SubQbuffer = malloc( 48 );               /* assumes sufficient aligned addresses */
    cmd = malloc( 18 );                      /* assumes sufficient aligned addresses */
    if ( !bufferTOC || !SubQbuffer || !cmd ) {
       fprintf( stderr, "Too low on memory. Giving up.\n");
       exit(NOMEM_ERROR);
    }

#if	defined SIM_CD
    scgp = malloc(sizeof(* scgp));
    if (scgp == NULL) {
	FatalError("No memory for SCSI structure.\n");
    }
    scgp->silent = 0;
    SetupSimCd();
#else
    /* if drive is of type scsi, get vendor name */
    if (interface == GENERIC_SCSI) {
        unsigned sector_size;

	SetupSCSI();
        sector_size = get_orig_sectorsize(scgp, &orgmode4, &orgmode10, &orgmode11);
	if (!SCSI_emulated_ATAPI_on(scgp)) {
          if ( sector_size != 2048 && set_sectorsize(scgp, 2048) ) {
	    fprintf( stderr, "Could not change sector size from %d to 2048\n", sector_size );
          }
        } else {
          sector_size = 2048;
        }

	/* get cache setting */

	/* set cache to zero */

    } else {
#if defined (HAVE_IOCTL_INTERFACE)
	scgp = malloc(sizeof(* scgp));
	if (scgp == NULL) {
		FatalError("No memory for SCSI structure.\n");
	}
	scgp->silent = 0;
	SetupCookedIoctl( global.dev_name );
#else
	FatalError("Sorry, there is no known method to access the device.\n");
#endif
    }
#endif	/* if def SIM_CD */
	/*
	 * The structure looks like a desaster :-(
	 * We do this more than once as it is impossible to understand where
	 * the right place would be to do this....
	 */
	if (scgp != NULL) {
		scgp->verbose = global.scsi_verbose;
	}
}
