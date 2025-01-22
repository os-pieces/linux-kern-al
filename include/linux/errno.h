#pragma once

#include <errno.h>

#ifndef EOPENSTALE
#define EOPENSTALE	518	/* open found a stale dentry */
#endif
#ifndef ESTALE
#define ESTALE 116
#endif

#ifndef ENOIOCTLCMD
#define ENOIOCTLCMD	515	/* No ioctl command */
#endif

#define ENOPARAM	519	/* Parameter not supported */

#ifndef ENOTBLK
#define ENOTBLK		15	/* Block device required */
#endif

#ifndef ENODEV
#define	ENODEV		19	/* No such device */
#endif
#ifndef EBUSY
#define	EBUSY		16	/* Device or resource busy */
#endif
#ifndef EXDEV
#define	EXDEV		18	/* Cross-device link */
#endif
#ifndef ENOENT
#define	ENOENT		 2	/* No such file or directory */
#endif
#ifndef EPERM
#define	EPERM		 1	/* Operation not permitted */
#endif
#ifndef ENOTDIR
#define	ENOTDIR		20	/* Not a directory */
#endif
#ifndef EISDIR
#define	EISDIR		21	/* Is a directory */
#endif
#ifndef ENAMETOOLONG
#define	ENAMETOOLONG	36	/* File name too long */
#endif
#ifndef EBADF
#define	EBADF		 9	/* Bad file number */
#endif
#ifndef EOVERFLOW
#define	EOVERFLOW	75	/* Value too large for defined data type */
#endif
#ifndef EFAULT
#define	EFAULT		14	/* Bad address */
#endif
#ifndef EIO
#define	EIO		 5	/* I/O error */
#endif
#ifndef ENXIO
#define	ENXIO		 6	/* No such device or address */
#endif
#ifndef ENOTTY
#define	ENOTTY		25	/* Not a typewriter */
#endif
#ifndef EAGAIN
#define	EAGAIN		11	/* Try again */
#endif
#ifndef ECHILD
#define	ECHILD		10	/* No child processes */
#endif
#ifndef ELOOP
#define	ELOOP		40	/* Too many symbolic links encountered */
#endif
#ifndef ENOTRECOVERABLE
#define	ENOTRECOVERABLE	131	/* State not recoverable */
#endif
#ifndef ENOTEMPTY
#define	ENOTEMPTY	39	/* Directory not empty */
#endif
#ifndef EEXIST
#define	EEXIST		17	/* File exists */
#endif
#ifndef ESPIPE
#define	ESPIPE		29	/* Illegal seek */
#endif
#ifndef EROFS
#define	EROFS		30	/* Read-only file system */
#endif
#ifndef EACCES
#define	EACCES		13	/* Permission denied */
#endif
