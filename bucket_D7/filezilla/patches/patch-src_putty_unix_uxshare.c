$NetBSD: patch-src_putty_unix_uxshare.c,v 1.1 2015/07/08 16:46:43 richard Exp $
Use POSIX fcntl instead of flock for setting file locking options on SunOS

--- src/putty/unix/uxshare.c.orig	2016-02-05 08:54:46 UTC
+++ src/putty/unix/uxshare.c
@@ -267,6 +267,13 @@ int platform_ssh_share(const char *pi_na
     int lockfd;
     Socket retsock;
 
+    struct flock lock;
+
+    lock.l_type = F_WRLCK;
+    lock.l_whence = SEEK_SET;
+    lock.l_start = 0;
+    lock.l_len = 0;
+
     /*
      * Sort out what we're going to call the directory in which we
      * keep the socket. This has the side effect of potentially
@@ -298,9 +305,15 @@ int platform_ssh_share(const char *pi_na
         sfree(lockname);
         return SHARE_NONE;
     }
+#ifdef __sun
+    if (fcntl(lockfd, F_SETLK, &lock) == -1) {
+        *logtext = dupprintf("%s: fcntl(F_SETLK): %s",
+                             lockname, strerror(errno));
+#else
     if (flock(lockfd, LOCK_EX) < 0) {
         *logtext = dupprintf("%s: flock(LOCK_EX): %s",
                              lockname, strerror(errno));
+#endif
         sfree(dirname);
         sfree(lockname);
         close(lockfd);
