/* @(#)cdr_drv.c	1.36 04/03/02 Copyright 1997-2004 J. Schilling */
#ifndef lint
static	char sccsid[] =
	"@(#)cdr_drv.c	1.36 04/03/02 Copyright 1997-2004 J. Schilling";
#endif
/*
 *	CDR device abstraction layer
 *
 *	Copyright (c) 1997-2004 J. Schilling
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; see the file COPYING.  If not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <mconfig.h>
#include <stdio.h>
#include <stdxlib.h>
#include <unixstd.h>	/* Include sys/types.h to make off_t available */
#include <standard.h>
#include <schily.h>

#include <scg/scsidefs.h>
#include <scg/scsireg.h>
#include <scg/scsitransp.h>

#include "cdrecord.h"

extern	int	xdebug;

extern	cdr_t	cdr_oldcd;
extern	cdr_t	cdr_cd;
extern	cdr_t	cdr_mmc;
extern	cdr_t	cdr_mmc_sony;
extern	cdr_t	cdr_cd_dvd;
extern	cdr_t	cdr_philips_cdd521O;
extern	cdr_t	cdr_philips_dumb;
extern	cdr_t	cdr_philips_cdd521;
extern	cdr_t	cdr_philips_cdd522;
extern	cdr_t	cdr_tyuden_ew50;
extern	cdr_t	cdr_kodak_pcd600;
extern	cdr_t	cdr_pioneer_dw_s114x;
extern	cdr_t	cdr_plasmon_rf4100;
extern	cdr_t	cdr_yamaha_cdr100;
extern	cdr_t	cdr_sony_cdu924;
extern	cdr_t	cdr_ricoh_ro1060;
extern	cdr_t	cdr_ricoh_ro1420;
extern	cdr_t	cdr_teac_cdr50;
extern	cdr_t	cdr_cw7501;
extern	cdr_t	cdr_cdr_simul;
extern	cdr_t	cdr_dvd_simul;

EXPORT	cdr_t 	*drive_identify		__PR((SCSI *scgp, cdr_t *, struct scsi_inquiry *ip));
EXPORT	int	drive_attach		__PR((SCSI *scgp, cdr_t *));
EXPORT	int	attach_unknown		__PR((void));
EXPORT	int	blank_dummy		__PR((SCSI *scgp, cdr_t *, long addr, int blanktype));
EXPORT	int	format_dummy		__PR((SCSI *scgp, cdr_t *, int fmtflags));
EXPORT	int	drive_getdisktype	__PR((SCSI *scgp, cdr_t *dp));
EXPORT	int	cmd_ill			__PR((SCSI *scgp));
EXPORT	int	cmd_dummy		__PR((SCSI *scgp, cdr_t *));
EXPORT	int	no_sendcue		__PR((SCSI *scgp, cdr_t *, track_t *trackp));
EXPORT	int	buf_dummy		__PR((SCSI *scgp, long *sp, long *fp));
EXPORT	BOOL	set_cdrcmds		__PR((char *name, cdr_t **dpp));
EXPORT	cdr_t	*get_cdrcmds		__PR((SCSI *scgp));

/*
 * List of CD-R drivers
 */
cdr_t	*drivers[] = {
	&cdr_cd_dvd,
	&cdr_mmc,
	&cdr_mmc_sony,
	&cdr_cd,
	&cdr_oldcd,
	&cdr_philips_cdd521O,
	&cdr_philips_dumb,
	&cdr_philips_cdd521,
	&cdr_philips_cdd522,
	&cdr_tyuden_ew50,
	&cdr_kodak_pcd600,
	&cdr_pioneer_dw_s114x,
	&cdr_plasmon_rf4100,
	&cdr_yamaha_cdr100,
	&cdr_ricoh_ro1060,
	&cdr_ricoh_ro1420,
	&cdr_sony_cdu924,
	&cdr_teac_cdr50,
	&cdr_cw7501,
	&cdr_cdr_simul,
	&cdr_dvd_simul,
	(cdr_t *)NULL,
};

EXPORT cdr_t *
drive_identify(scgp, dp, ip)
	SCSI			*scgp;
	cdr_t			*dp;
	struct scsi_inquiry	*ip;
{
	return (dp);
}

EXPORT int
drive_attach(scgp, dp)
	SCSI			*scgp;
	cdr_t			*dp;
{
	return (0);
}

EXPORT int
attach_unknown()
{
	errmsgno(EX_BAD, "Unsupported drive type\n");
	return (-1);
}

EXPORT int
blank_dummy(scgp, dp, addr, blanktype)
	SCSI	*scgp;
	cdr_t	*dp;
	long	addr;
	int	blanktype;
{
	printf("This drive or media does not support the 'BLANK media' command\n");
	return (-1);
}

EXPORT int
format_dummy(scgp, dp, fmtflags)
	SCSI	*scgp;
	cdr_t	*dp;
	int	fmtflags;
{
	printf("This drive or media does not support the 'FORMAT media' command\n");
	return (-1);
}

EXPORT int
drive_getdisktype(scgp, dp)
	SCSI	*scgp;
	cdr_t	*dp;
{
/*	dstat_t	*dsp = dp->cdr_dstat;*/
	return (0);
}

EXPORT int
cmd_ill(scgp)
	SCSI	*scgp;
{
	errmsgno(EX_BAD, "Unspecified command not implemented for this drive.\n");
	return (-1);
}

EXPORT int
cmd_dummy(scgp, dp)
	SCSI	*scgp;
	cdr_t	*dp;
{
	return (0);
}

EXPORT int
no_sendcue(scgp, dp, trackp)
	SCSI	*scgp;
	cdr_t	*dp;
	track_t	*trackp;
{
	errmsgno(EX_BAD, "SAO writing not available or not implemented for this drive.\n");
	return (-1);
}

EXPORT int
buf_dummy(scgp, sp, fp)
	SCSI	*scgp;
	long	*sp;
	long	*fp;
{
	return (-1);
}

EXPORT BOOL
set_cdrcmds(name, dpp)
	char	*name;
	cdr_t	**dpp;
{
	cdr_t	**d;
	int	n;

	for (d = drivers; *d != (cdr_t *)NULL; d++) {
		if (streql((*d)->cdr_drname, name)) {
			if (dpp != NULL)
				*dpp = *d;
			return (TRUE);
		}
	}
	if (dpp == NULL)
		return (FALSE);

	if (!streql("help", name))
		error("Illegal driver type '%s'.\n", name);

	error("Driver types:\n");
	for (d = drivers; *d != (cdr_t *)NULL; d++) {
		error("%s%n",
			(*d)->cdr_drname, &n);
		error("%*s%s\n",
			20-n, "",
			(*d)->cdr_drtext);
	}
	if (streql("help", name))
		exit(0);
	exit(EX_BAD);
	return (FALSE);		/* Make lint happy */
}

EXPORT cdr_t *
get_cdrcmds(scgp)
	SCSI	*scgp;
{
	cdr_t	*dp = (cdr_t *)0;
	cdr_t	*odp = (cdr_t *)0;
	BOOL	is_wr = FALSE;
	BOOL	is_cd = FALSE;
	BOOL	is_dvd = FALSE;
	BOOL	is_dvdplus = FALSE;
	BOOL	is_ddcd = FALSE;
	BOOL	is_cdwr = FALSE;
	BOOL	is_dvdwr = FALSE;
	BOOL	is_dvdpluswr = FALSE;
	BOOL	is_ddcdwr = FALSE;

	/*
	 * First check for SCSI-3/mmc-3 drives.
	 */
	if (get_proflist(scgp, &is_wr, &is_cd, &is_dvd,
						&is_dvdplus, &is_ddcd) >= 0) {

		get_wproflist(scgp, &is_cdwr, &is_dvdwr,
						&is_dvdpluswr, &is_ddcdwr);
		if (xdebug) {
			error(
			"Found MMC-3 %s CD: %s/%s DVD-: %s/%s DVD+: %s/%s DDCD: %s/%s.\n",
					is_wr ? "writer": "reader",
					is_cd?"r":"-",
					is_cdwr?"w":"-",
					is_dvd?"r":"-",
					is_dvdwr?"w":"-",
					is_dvdplus?"r":"-",
					is_dvdpluswr?"w":"-",
					is_ddcd?"r":"-",
					is_ddcdwr?"w":"-");
		}
		if (!is_wr) {
			dp = &cdr_cd;
		} else {
			dp = &cdr_cd_dvd;
		}
	} else
	/*
	 * First check for SCSI-3/mmc drives.
	 */
	if (is_mmc(scgp, &is_cdwr, &is_dvdwr)) {
		if (xdebug) {
			error("Found MMC drive CDWR: %d DVDWR: %d.\n",
							is_cdwr, is_dvdwr);
		}

		if (is_cdwr && is_dvdwr)
			dp = &cdr_cd_dvd;
		else
			dp = &cdr_mmc;

	} else switch (scgp->dev) {

	case DEV_CDROM:		dp = &cdr_oldcd;		break;
	case DEV_MMC_CDROM:	dp = &cdr_cd;			break;
	case DEV_MMC_CDR:	dp = &cdr_mmc;			break;
	case DEV_MMC_CDRW:	dp = &cdr_mmc;			break;
	case DEV_MMC_DVD_WR:	dp = &cdr_cd_dvd;		break;

	case DEV_CDD_521_OLD:	dp = &cdr_philips_cdd521O;	break;
	case DEV_CDD_521:	dp = &cdr_philips_cdd521;	break;
	case DEV_CDD_522:
	case DEV_CDD_2000:
	case DEV_CDD_2600:	dp = &cdr_philips_cdd522;	break;
	case DEV_TYUDEN_EW50:	dp = &cdr_tyuden_ew50;		break;
	case DEV_PCD_600:	dp = &cdr_kodak_pcd600;		break;
	case DEV_YAMAHA_CDR_100:dp = &cdr_yamaha_cdr100;	break;
	case DEV_MATSUSHITA_7501:dp = &cdr_cw7501;		break;
	case DEV_MATSUSHITA_7502:
	case DEV_YAMAHA_CDR_400:dp = &cdr_mmc;			break;
	case DEV_PLASMON_RF_4100:dp = &cdr_plasmon_rf4100;	break;
	case DEV_SONY_CDU_924:	dp = &cdr_sony_cdu924;		break;
	case DEV_RICOH_RO_1060C:dp = &cdr_ricoh_ro1060;		break;
	case DEV_RICOH_RO_1420C:dp = &cdr_ricoh_ro1420;		break;
	case DEV_TEAC_CD_R50S:	dp = &cdr_teac_cdr50;		break;

	case DEV_PIONEER_DW_S114X: dp = &cdr_pioneer_dw_s114x;	break;

	default:		dp = &cdr_mmc;
	}
	odp = dp;

	if (xdebug) {
		error("Using driver '%s' for identify.\n",
			dp != NULL ?
			dp->cdr_drname :
			"<no driver>");
	}

	if (dp != (cdr_t *)0)
		dp = dp->cdr_identify(scgp, dp, scgp->inq);

	if (xdebug && dp != odp) {
		error("Identify set driver to '%s'.\n",
			dp != NULL ?
			dp->cdr_drname :
			"<no driver>");
	}

	return (dp);
}
