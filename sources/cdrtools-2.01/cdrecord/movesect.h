/* @(#)movesect.h	1.1 01/06/02 Copyright 2001 J. Schilling */
/*
 *	Copyright (c) 2001 J. Schilling
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

#ifndef	_MOVESECT_H
#define	_MOVESECT_H

#define	move2352(from, to)	movebytes(from, to, 2352)
#define	move2336(from, to)	movebytes(from, to, 2336)
#define	move2048(from, to)	movebytes(from, to, 2048)

#define	fill2352(p, val)	fillbytes(p, 2352, val)
#define	fill2048(p, val)	fillbytes(p, 2048, val)
#define	fill96(p, val)		fillbytes(p, 96, val)

extern	void	scatter_secs	__PR((track_t *trackp, char *bp, int nsecs));

#endif
