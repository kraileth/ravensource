--- src/command.c.orig	2018-04-07 21:41:21 UTC
+++ src/command.c
@@ -115,6 +115,9 @@ int thread_rl_RetCode = -1; /* return co
 
 #ifndef _Windows
 # include "help.h"
+# if defined __FreeBSD__ || defined __DragonFly__
+# include <sys/wait.h>
+# endif
 #endif /* _Windows */
 
 #ifdef _Windows
