--- src/util/disk_cache.c.orig	2018-01-18 21:30:29 UTC
+++ src/util/disk_cache.c
@@ -50,6 +50,10 @@
 
 #include "disk_cache.h"
 
+#ifndef O_CLOEXEC
+#define O_CLOEXEC	0
+#endif
+
 /* Number of bits to mask off from a cache key to get an index. */
 #define CACHE_INDEX_KEY_BITS 16
 
@@ -497,7 +501,11 @@ choose_lru_file_matching(const char *dir
          break;
 
       struct stat sb;
+#ifdef __sun__
+      if (fstatat(dir->dd_fd, entry->d_name, &sb, 0) == 0) {
+#else
       if (fstatat(dirfd(dir), entry->d_name, &sb, 0) == 0) {
+#endif
          if (!lru_atime || (sb.st_atime < lru_atime)) {
             size_t len = strlen(entry->d_name);
 