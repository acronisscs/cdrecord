/* @(#)defaults.h	1.18 04/06/01 joerg */
/*
 * Header file defaults.h - assorted default values for character strings in
 * the volume descriptor.
 *
 * Copyright (c) 1999-2004 J. Schilling
 */

#define	PREPARER_DEFAULT 	NULL
#define	PUBLISHER_DEFAULT	NULL
#ifndef	APPID_DEFAULT
#ifdef	APPLE_HYB
#define	APPID_DEFAULT 		"MKISOFS ISO 9660/HFS FILESYSTEM BUILDER (C) 1993 E.YOUNGDALE (C) 1997 J.PEARSON/J.SCHILLING"
#else
#define	APPID_DEFAULT 		"MKISOFS ISO 9660 FILESYSTEM BUILDER (C) 1993 E.YOUNGDALE (C) 1997 J.PEARSON/J.SCHILLING"
#endif /* APPLE_HYB */
#endif
#define	COPYRIGHT_DEFAULT 	NULL
#define	BIBLIO_DEFAULT		NULL
#define	ABSTRACT_DEFAULT	NULL
#define	VOLSET_ID_DEFAULT	NULL
#define	VOLUME_ID_DEFAULT	"CDROM"
#define	BOOT_CATALOG_DEFAULT	"boot.catalog"
#define	BOOT_IMAGE_DEFAULT	NULL
#ifdef	APPLE_HYB
#define	APPLE_TYPE_DEFAULT	"TEXT"
#define	APPLE_CREATOR_DEFAULT	"unix"
#endif /* APPLE_HYB */

#ifdef __QNX__
#define	SYSTEM_ID_DEFAULT	"QNX"
#endif

#ifdef __osf__
#define	SYSTEM_ID_DEFAULT	"OSF"
#endif

#ifdef __sun
#ifdef __SVR4
#define	SYSTEM_ID_DEFAULT	"Solaris"
#else
#define	SYSTEM_ID_DEFAULT	"SunOS"
#endif
#endif

#ifdef __hpux
#define	SYSTEM_ID_DEFAULT	"HP-UX"
#endif

#ifdef __sgi
#define	SYSTEM_ID_DEFAULT	"SGI"
#endif

#if	defined(_IBMR2) || defined(_AIX)
#define	SYSTEM_ID_DEFAULT	"AIX"
#endif

#if	defined(_WIN) || defined(__CYGWIN32__) || defined(__CYGWIN__)
#define	SYSTEM_ID_DEFAULT	"Win32"
#endif /* _WIN */

#ifdef __EMX__
#define	SYSTEM_ID_DEFAULT	"OS/2"
#endif

#ifdef __FreeBSD__
#define	SYSTEM_ID_DEFAULT	"FreeBSD"
#endif

#ifdef __NetBSD__
#define	SYSTEM_ID_DEFAULT	"NetBSD"
#endif

#ifdef __OpenBSD__
#define	SYSTEM_ID_DEFAULT	"OpenBSD"
#endif

#ifdef __bsdi__
#define	SYSTEM_ID_DEFAULT	"BSD/OS"
#endif

#ifdef __NeXT__
#define	SYSTEM_ID_DEFAULT	"NeXT"
#endif

#if defined(__NeXT__) && defined(__TARGET_OSNAME) && __TARGET_OSNAME == rhapsody
#undef	SYSTEM_ID_DEFAULT
#define	SYSTEM_ID_DEFAULT	"Rhapsody"
#endif

#if defined(__APPLE__) && defined(__MACH__)
#undef	SYSTEM_ID_DEFAULT
#define	SYSTEM_ID_DEFAULT	"Mac OS X"
#endif

#ifdef __BEOS__
#define	SYSTEM_ID_DEFAULT	"BeOS"
#endif

#ifdef __OS2
#define	SYSTEM_ID_DEFAULT	"OS/2"
#endif

#ifdef VMS
#define	SYSTEM_ID_DEFAULT	"VMS"
#endif

#ifdef OPENSERVER
#define	SYSTEM_ID_DEFAULT	"SCO-OPENSERVER"
#endif

#ifdef UNIXWARE
#define	SYSTEM_ID_DEFAULT	"SCO-UNIXWARE"
#endif

#ifdef linux
#define	SYSTEM_ID_DEFAULT	"LINUX"
#endif

#ifdef	__DJGPP__
#define	SYSTEM_ID_DEFAULT	"DOS"
#endif

#ifndef SYSTEM_ID_DEFAULT
#define	SYSTEM_ID_DEFAULT	"UNIX"
#endif
