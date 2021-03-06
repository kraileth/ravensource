--- src/unix/freebsd.c.orig	2018-10-08 16:52:30 UTC
+++ src/unix/freebsd.c
@@ -67,7 +67,8 @@ void uv__platform_loop_delete(uv_loop_t*
 
 
 #ifdef __DragonFly__
-int uv_exepath(char* buffer, size_t* size) {
+/* deprecated long ago, use KERN_PROC_PATHNAME sysctl */
+int uv_exepath_procfs(char* buffer, size_t* size) {
   char abspath[PATH_MAX * 2 + 1];
   ssize_t abspath_size;
 
@@ -89,7 +90,8 @@ int uv_exepath(char* buffer, size_t* siz
 
   return 0;
 }
-#else
+#endif
+
 int uv_exepath(char* buffer, size_t* size) {
   char abspath[PATH_MAX * 2 + 1];
   int mib[4];
@@ -119,7 +121,6 @@ int uv_exepath(char* buffer, size_t* siz
 
   return 0;
 }
-#endif
 
 uint64_t uv_get_free_memory(void) {
   int freecount;
