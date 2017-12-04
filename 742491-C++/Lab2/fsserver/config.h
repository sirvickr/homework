/* sca_impl/config.h.  Generated automatically by configure.  */
/* sca_impl/config.h.in.  Generated automatically from configure.in by autoheader.  */

/* #undef HAVE_BOOST */

/* #undef ORB_MICO */
#define ORB_ACETAO 1
/* #undef ORB_ORBIX2000 */
/* #undef ORB_VISIBROKER */

/* Define if you have the access function.  */
#define HAVE_ACCESS 1

/* Define if you have the fstat function.  */
#define HAVE_FSTAT 1

/* Define if you have the glob function.  */
#if !defined (_WIN32)
# define HAVE_GLOB 1
#endif

/* Define if you have the mkdir function.  */
#define HAVE_MKDIR 1

/* Define if you have the opendir function.  */
#define HAVE_OPENDIR 1

/* Define if you have the rmdir function.  */
#define HAVE_RMDIR 1

/* Define if you have the stat function.  */
#define HAVE_STAT 1

/* Define if you have the statfs function.  */
#if !defined (_WIN32)
# define HAVE_STATFS 1
#endif

/* Define if you have the statvfs function.  */
#if !defined (_WIN32)
#define HAVE_STATVFS 1
#endif

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the time function.  */
#define HAVE_TIME 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <time.h> header file.  */
#define HAVE_TIME_H 1

/* Define if you have the <unistd.h> header file.  */
#if !defined (_WIN32)
#define HAVE_UNISTD_H 1
#endif

/* Name of package */
#define PACKAGE "CISCA"

/* Version number of package */
#define VERSION "1.0"

/* C++ compiler supports template repository */
#define HAVE_TEMPLATE_REPOSITORY 1

/* Define if you have the sprintf function */
#define HAVE_SPRINTF 1

/* Define if you have the snprintf function */
#define HAVE_SNPRINTF 1

