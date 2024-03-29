/* @(#)pmalloc.h	1.1 04/02/20 Copyright 2004 J. Schilling */
/*
 *	Paranoia malloc() functions
 *
 *	Copyright (c) 2004 J. Schilling
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

#ifndef	_PMALLOC_H
#define	_PMALLOC_H

extern	void	_pfree		__PR((void *ptr));
extern	void	*_pmalloc	__PR((size_t size));
extern	void	*_pcalloc	__PR((size_t nelem, size_t elsize));
extern	void	*_prealloc	__PR((void *ptr, size_t size));

#endif	/* _PMALLOC_H */
