/* @(#)xio.h	1.2 04/03/02 Copyright 2003-2004 J. Schilling */
/*
 *	EXtended I/O functions for cdrecord
 *
 *	Copyright (c) 2003-2004 J. Schilling
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

#ifndef	_XIO_H
#define	_XIO_H

#include <utypes.h>

typedef struct xio {
	struct xio	*x_next;
	char		*x_name;
	Ullong		x_off;
	int		x_file;
	int		x_refcnt;
	int		x_oflag;
	int		x_omode;
} xio_t;

#define	xfileno(p)	(((xio_t *)(p))->x_file)

extern	void	*xopen		__PR((char *name, int oflag, int mode));
extern	int	xclose		__PR((void *vp));

#endif
