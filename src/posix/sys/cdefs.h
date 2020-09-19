#ifndef SYS_CDEFS_H
#define SYS_CDEFS_H

#ifndef __BEGIN_DECLS
#	ifdef __cplusplus
#		define __BEGIN_DECLS extern "C" {
#	else
#		define __BEGIN_DECLS
#	endif
#endif

#ifndef __END_DECLS
#	ifdef __cplusplus
#		define __END_DECLS }
#	else
#		define __END_DECLS
#	endif
#endif

#endif