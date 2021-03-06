--- lib/freebl/verified/kremlib.h.orig	2018-01-18 14:19:59 UTC
+++ lib/freebl/verified/kremlib.h
@@ -184,7 +184,10 @@ typedef const char *Prims_string;
 /* ... for Linux */
 #if defined(__linux__) || defined(__CYGWIN__)
 #include <endian.h>
-
+/* ... for BSDs */
+#elif defined(__DragonFly__) || defined(__FreeBSD__) \
+   || defined(__NetBSD__) || defined(__OpenBSD__)
+#include <sys/endian.h>
 /* ... for OSX */
 #elif defined(__APPLE__)
 #include <libkern/OSByteOrder.h>
