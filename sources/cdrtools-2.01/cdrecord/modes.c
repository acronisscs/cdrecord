/* @(#)modes.c	1.25 04/03/02 Copyright 1988, 1997-2001, 2004 J. Schilling */
#ifndef lint
static	char sccsid[] =
	"@(#)modes.c	1.25 04/03/02 Copyright 1988, 1997-2001, 2004 J. Schilling";
#endif
/*
 *	SCSI mode page handling
 *
 *	Copyright (c) 1988, 1997-2001, 2004 J. Schilling
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
#include <utypes.h>
#include <standard.h>
#include <schily.h>
#include <scg/scgcmd.h>
#include <scg/scsireg.h>
#include <scg/scsitransp.h>

#include "cdrecord.h"

EXPORT int	scsi_compliant;

LOCAL	BOOL	has_mode_page	__PR((SCSI *scgp, int page, char *pagename, int *lenp));
EXPORT	BOOL	get_mode_params	__PR((SCSI *scgp, int page, char *pagename,
					Uchar *modep, Uchar *cmodep,
					Uchar *dmodep, Uchar *smodep,
					int *lenp));
EXPORT	BOOL	set_mode_params	__PR((SCSI *scgp, char *pagename, Uchar *modep,
					int len, int save, int secsize));

#define	XXX

#ifdef	XXX
LOCAL BOOL
has_mode_page(scgp, page, pagename, lenp)
	SCSI	*scgp;
	int	page;
	char	*pagename;
	int	*lenp;
{
	Uchar	mode[0x100];
	int	hdlen;
	int	len = 1;				/* Nach SCSI Norm */
	int	try = 0;
	struct	scsi_mode_page_header *mp;

	/*
	 * ATAPI drives (used e.g. by IOMEGA) from y2k have the worst firmware
	 * I've seen. They create DMA buffer overruns if we request less than
	 * 3 bytes with 6 byte mode sense which equals 4 byte with 10 byte mode
	 * sense. In order to prevent repeated bus resets, we remember this
	 * bug.
	 *
	 * IOMEGA claims that they are using Philips clone drives but a Philips
	 * drive I own does not have the problem.
	 */
	if ((scgp->dflags & DRF_MODE_DMA_OVR) != 0)
		len = sizeof (struct scsi_mode_header);
again:
	fillbytes((caddr_t)mode, sizeof (mode), '\0');
	if (lenp)
		*lenp = 0;

	scgp->silent++;
	(void) unit_ready(scgp);
/* Maxoptix bringt Aborted cmd 0x0B mit code 0x4E (overlapping cmds)*/

	/*
	 * The Matsushita CW-7502 will sometimes deliver a zeroed
	 * mode page 2A if "Page n default" is used instead of "current".
	 */
	if (mode_sense(scgp, mode, len, page, 0) < 0) {	/* Page n current */
		scgp->silent--;
		if (len < (int)sizeof (struct scsi_mode_header) && try == 0) {
			len = sizeof (struct scsi_mode_header);
			goto again;
		}
		return (FALSE);
	} else {
		if (len > 1 && try == 0) {
			/*
			 * If we come here, we got a hard failure with the
			 * fist try. Remember this (IOMEGA USB) firmware bug.
			 */
			if ((scgp->dflags & DRF_MODE_DMA_OVR) == 0) {
				/* XXX if (!nowarn) */
				errmsgno(EX_BAD,
				"Warning: controller creates hard SCSI failure when retrieving %s page.\n",
								pagename);
				scgp->dflags |= DRF_MODE_DMA_OVR;
			}
		}
		len = ((struct scsi_mode_header *)mode)->sense_data_len + 1;
	}
	/*
	 * ATAPI drives as used by IOMEGA may receive a SCSI bus device reset
	 * in between these two mode sense commands.
	 */
	(void) unit_ready(scgp);
	if (mode_sense(scgp, mode, len, page, 0) < 0) {	/* Page n current */
		scgp->silent--;
		return (FALSE);
	}
	scgp->silent--;

	if (scgp->verbose)
		scg_prbytes("Mode Sense Data", mode, len - scg_getresid(scgp));
	hdlen = sizeof (struct scsi_mode_header) +
			((struct scsi_mode_header *)mode)->blockdesc_len;
	mp = (struct scsi_mode_page_header *)(mode + hdlen);
	if (scgp->verbose)
		scg_prbytes("Mode Page  Data", (Uchar *)mp, mp->p_len+2);

	if (mp->p_len == 0) {
		if (!scsi_compliant && try == 0) {
			len = hdlen;
			/*
			 * add sizeof page header (page # + len byte)
			 * (should normaly result in len == 14)
			 * this allowes to work with:
			 * 	Quantum Q210S	(wants at least 13)
			 * 	MD2x		(wants at least 4)
			 */
			len += 2;
			try++;
			goto again;
		}
		/* XXX if (!nowarn) */
		errmsgno(EX_BAD,
			"Warning: controller returns zero sized %s page.\n",
								pagename);
	}
	if (!scsi_compliant &&
	    (len < (int)(mp->p_len + hdlen + 2))) {
		len = mp->p_len + hdlen + 2;

		/* XXX if (!nowarn) */
		errmsgno(EX_BAD,
			"Warning: controller returns wrong size for %s page.\n",
								pagename);
	}
	if (mp->p_code != page) {
		/* XXX if (!nowarn) */
		errmsgno(EX_BAD,
			"Warning: controller returns wrong page %X for %s page (%X).\n",
						mp->p_code, pagename, page);
		return (FALSE);
	}

	if (lenp)
		*lenp = len;
	return (mp->p_len > 0);
}
#endif

EXPORT BOOL
get_mode_params(scgp, page, pagename, modep, cmodep, dmodep, smodep, lenp)
	SCSI	*scgp;
	int	page;
	char	*pagename;
	Uchar	*modep;
	Uchar	*cmodep;
	Uchar	*dmodep;
	Uchar	*smodep;
	int	*lenp;
{
	int	len;
	BOOL	ret = TRUE;

#ifdef	XXX
	if (lenp)
		*lenp = 0;
	if (!has_mode_page(scgp, page, pagename, &len)) {
		if (!scgp->silent) errmsgno(EX_BAD,
			"Warning: controller does not support %s page.\n",
								pagename);
		return (FALSE);
	}
	if (lenp)
		*lenp = len;
#else
	if (lenp == 0)
		len = 0xFF;
#endif

	if (modep) {
		fillbytes(modep, 0x100, '\0');
		scgp->silent++;
		(void) unit_ready(scgp);
		scgp->silent--;
		if (mode_sense(scgp, modep, len, page, 0) < 0) { /* Page x current */
			errmsgno(EX_BAD, "Cannot get %s data.\n", pagename);
			ret = FALSE;
		} else if (scgp->verbose) {
			scg_prbytes("Mode Sense Data", modep, len - scg_getresid(scgp));
		}
	}

	if (cmodep) {
		fillbytes(cmodep, 0x100, '\0');
		scgp->silent++;
		(void) unit_ready(scgp);
		scgp->silent--;
		if (mode_sense(scgp, cmodep, len, page, 1) < 0) { /* Page x change */
			errmsgno(EX_BAD, "Cannot get %s mask.\n", pagename);
			ret = FALSE;
		} else if (scgp->verbose) {
			scg_prbytes("Mode Sense Data", cmodep, len - scg_getresid(scgp));
		}
	}

	if (dmodep) {
		fillbytes(dmodep, 0x100, '\0');
		scgp->silent++;
		(void) unit_ready(scgp);
		scgp->silent--;
		if (mode_sense(scgp, dmodep, len, page, 2) < 0) { /* Page x default */
			errmsgno(EX_BAD, "Cannot get default %s data.\n",
								pagename);
			ret = FALSE;
		} else if (scgp->verbose) {
			scg_prbytes("Mode Sense Data", dmodep, len - scg_getresid(scgp));
		}
	}

	if (smodep) {
		fillbytes(smodep, 0x100, '\0');
		scgp->silent++;
		(void) unit_ready(scgp);
		scgp->silent--;
		if (mode_sense(scgp, smodep, len, page, 3) < 0) { /* Page x saved */
			errmsgno(EX_BAD, "Cannot get saved %s data.\n", pagename);
			ret = FALSE;
		} else if (scgp->verbose) {
			scg_prbytes("Mode Sense Data", smodep, len - scg_getresid(scgp));
		}
	}

	return (ret);
}

EXPORT BOOL
set_mode_params(scgp, pagename, modep, len, save, secsize)
	SCSI	*scgp;
	char	*pagename;
	Uchar	*modep;
	int	len;
	int	save;
	int	secsize;
{
	int	i;

	((struct scsi_modesel_header *)modep)->sense_data_len	= 0;
	((struct scsi_modesel_header *)modep)->res2		= 0;

	i = ((struct scsi_mode_header *)modep)->blockdesc_len;
	if (i > 0) {
		i_to_3_byte(
			((struct scsi_mode_data *)modep)->blockdesc.nlblock,
								0);
		if (secsize >= 0)
		i_to_3_byte(((struct scsi_mode_data *)modep)->blockdesc.lblen,
							secsize);
	}

	scgp->silent++;
	(void) unit_ready(scgp);
	scgp->silent--;
	if (save == 0 || mode_select(scgp, modep, len, save, scgp->inq->data_format >= 2) < 0) {
		scgp->silent++;
		(void) unit_ready(scgp);
		scgp->silent--;
		if (mode_select(scgp, modep, len, 0, scgp->inq->data_format >= 2) < 0) {
			if (scgp->silent == 0) {
				errmsgno(EX_BAD,
					"Warning: using default %s data.\n",
					pagename);
				scg_prbytes("Mode Select Data", modep, len);
			}
			return (FALSE);
		}
	}
	return (TRUE);
}
