--- lib/Support/Unix/Path.inc.orig	2018-08-30 09:31:52 UTC
+++ lib/Support/Unix/Path.inc
@@ -53,13 +53,13 @@
 
 #include <sys/types.h>
 #if !defined(__APPLE__) && !defined(__OpenBSD__) && !defined(__FreeBSD__) &&   \
-    !defined(__linux__)
+    !defined(__linux__) && !defined(__DragonFly__)
 #include <sys/statvfs.h>
 #define STATVFS statvfs
 #define FSTATVFS fstatvfs
 #define STATVFS_F_FRSIZE(vfs) vfs.f_frsize
 #else
-#if defined(__OpenBSD__) || defined(__FreeBSD__)
+#if defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
 #include <sys/mount.h>
 #include <sys/param.h>
 #elif defined(__linux__)
