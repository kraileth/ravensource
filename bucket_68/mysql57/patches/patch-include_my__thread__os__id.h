--- include/my_thread_os_id.h.orig	2017-12-28 03:46:26 UTC
+++ include/my_thread_os_id.h
@@ -75,8 +75,12 @@ static inline my_thread_os_id_t my_threa
   return pthread_getthreadid_np();
 #else
 #ifdef HAVE_INTEGER_PTHREAD_SELF
+#  ifdef __DragonFly__
+  return syscall(SYS_lwp_gettid);
+#  else
   /* Unknown platform, fallback. */
   return pthread_self();
+#  endif
 #else
   /* Feature not available. */
   return 0;
