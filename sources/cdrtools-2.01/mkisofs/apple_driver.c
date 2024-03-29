/* @(#)apple_driver.c	1.6 04/03/04 joerg */
#ifndef lint
static	char sccsid[] =
	"@(#)apple_driver.c	1.6 04/03/04 joerg";
#endif
/*
 *	apple_driver.c: extract Mac partition label, maps and boot driver
 *
 *	Based on Apple_Driver.pl, part of "mkisofs 1.05 PLUS" by Andy Polyakov
 *	<appro@fy.chalmers.se> (I don't know Perl, so I rewrote it C ...)
 *	(see http://fy.chalmers.se/~appro/mkisofs_plus.html for details)
 *
 *	usage: apple_driver CDROM_device > HFS_driver_file
 *
 *	The format of the HFS driver file:
 *
 *	HFS CD Label Block				512 bytes
 *	Driver Partition Map (for 2048 byte blocks)	512 bytes
 *	Driver Partition Map (for 512 byte blocks)	512 bytes
 *	Empty						512 bytes
 *	Driver Partition				N x 2048 bytes
 *	HFS Partition Boot Block			1024 bytes
 *
 *	By extracting a driver from an Apple CD, you become liable to obey
 *	Apple Computer, Inc. Software License Agreements.
 *
 *	James Pearson 17/5/98
 */

#include <mconfig.h>
#include "mkisofs.h"
#include <mac_label.h>
#include <schily.h>

EXPORT	int	get_732	__PR((char *p));
EXPORT	int	get_722	__PR((char *p));
EXPORT	int	main	__PR((int argc, char **argv));

EXPORT int
get_732(p)
	char	*p;
{
	return ((p[3] & 0xff)
		| ((p[2] & 0xff) << 8)
		| ((p[1] & 0xff) << 16)
		| ((p[0] & 0xff) << 24));
}

EXPORT int
get_722(p)
	char	*p;
{
	return ((p[1] & 0xff)
		| ((p[0] & 0xff) << 8));
}

EXPORT int
main(argc, argv)
	int	argc;
	char	**argv;
{
	FILE		*fp;
	MacLabel	*mac_label;
	MacPart		*mac_part;
	unsigned char	Block0[HFS_BLOCKSZ];
	unsigned char	block[SECTOR_SIZE];
	unsigned char	bootb[2*HFS_BLOCKSZ];
	unsigned char	pmBlock512[HFS_BLOCKSZ];
	unsigned int	sbBlkSize;
	unsigned int	pmPyPartStart;
	unsigned int	pmPartStatus;
	unsigned int	pmMapBlkCnt;
	int		have_boot = 0, have_hfs = 0;
	int		hfs_start;
	int		i, j;


	save_args(argc, argv);

	if (argc != 2)
	    comerrno(EX_BAD, "Usage: %s device-path", argv[0]);

	if ((fp = fopen(argv[1], "rb")) == NULL)
	    comerr("Can't open '%s'.", argv[1]);

	if (fread(Block0, 1, HFS_BLOCKSZ, fp) != HFS_BLOCKSZ)
	    comerr("Can't read '%s'.", argv[1]);

	mac_label = (MacLabel *)Block0;
	mac_part = (MacPart *)block;

	sbBlkSize = get_722((char *)mac_label->sbBlkSize);

	if (! IS_MAC_LABEL(mac_label) || sbBlkSize != SECTOR_SIZE)
	    comerrno(EX_BAD, "%s is not a bootable Mac disk", argv[1]);

	i = 1;
	do {
		if (fseek(fp, i * HFS_BLOCKSZ, SEEK_SET) != 0)
			comerr("Ccan't seek %s", argv[1]);

		if (fread(block, 1, HFS_BLOCKSZ, fp) != HFS_BLOCKSZ)
			comerr("Can't read '%s'.", argv[1]);

		pmMapBlkCnt = get_732((char *)mac_part->pmMapBlkCnt);

		if (!have_boot && strncmp((char *)mac_part->pmPartType, pmPartType_2, 12) == 0) {
			hfs_start = get_732((char *)mac_part->pmPyPartStart);

			fprintf(stderr, "%s: found 512 driver partition (at block %d)\n", argv[0], hfs_start);
			memcpy(pmBlock512, block, HFS_BLOCKSZ);
			have_boot = 1;
		}

		if (!have_hfs && strncmp((char *)mac_part->pmPartType, pmPartType_4, 9) == 0) {

			hfs_start = get_732((char *)mac_part->pmPyPartStart);

			if (fseek(fp, hfs_start*HFS_BLOCKSZ, SEEK_SET) != 0)
				comerr("Can't seek '%s'.", argv[1]);

			if (fread(bootb, 2, HFS_BLOCKSZ, fp) != HFS_BLOCKSZ)
				comerr("Can't read '%s'.", argv[1]);

			if (get_722((char *)bootb) == 0x4c4b) {

				fprintf(stderr, "%s: found HFS partition (at blk %d)\n", argv[0], hfs_start);
				have_hfs = 1;
			}
		}
	} while (i++ < pmMapBlkCnt);

	if (!have_hfs || !have_boot)
		comerrno(EX_BAD, "%s is not a bootable Mac disk", argv[1]);

	i = 1;

	do {
		if (fseek(fp, i*sbBlkSize, SEEK_SET) != 0)
			comerr("Can't seek '%s'.", argv[1]);

		if (fread(block, 1, HFS_BLOCKSZ, fp) != HFS_BLOCKSZ)
			comerr("Can't read '%s'.", argv[1]);

		pmMapBlkCnt = get_732((char *)mac_part->pmMapBlkCnt);

		if (strncmp((char *)mac_part->pmPartType, pmPartType_2, 12) == 0) {

			int	start, num;

			fprintf(stderr, "%s: extracting %s ", argv[0], mac_part->pmPartType);
			start = get_732((char *)mac_part->pmPyPartStart);
			num = get_732((char *)mac_part->pmPartBlkCnt);
			fwrite(Block0, 1, HFS_BLOCKSZ, stdout);
			fwrite(block, 1, HFS_BLOCKSZ, stdout);
			fwrite(pmBlock512, 1, HFS_BLOCKSZ, stdout);
			memset(block, 0, HFS_BLOCKSZ);
			fwrite(block, 1, HFS_BLOCKSZ, stdout);

			if (fseek(fp, start*sbBlkSize, SEEK_SET) != 0)
				comerr("Can't seek '%s'.", argv[1]);

			for (j = 0; j < num; j++) {
				if (fread(block, 1, sbBlkSize, fp) != sbBlkSize)
					comerr("Can't read '%s'.", argv[1]);

				fwrite(block, 1, sbBlkSize, stdout);
				fprintf(stderr, ".");
			}
			fprintf(stderr, "\n");

			fwrite(bootb, 2, HFS_BLOCKSZ, stdout);
			fclose(fp);
			exit(0);
		}

		if (!IS_MAC_PART(mac_part))
			comerrno(EX_BAD, "Unable to find boot partition");

	} while (i++ < pmMapBlkCnt);

	return (0);
}
