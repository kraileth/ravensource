--- src/mesa/drivers/dri/i965/brw_performance_query.c.orig	2018-01-18 21:30:29 UTC
+++ src/mesa/drivers/dri/i965/brw_performance_query.c
@@ -1752,9 +1752,17 @@ enumerate_sysfs_metrics(struct brw_conte
    while ((metric_entry = readdir(metricsdir))) {
       struct hash_entry *entry;
 
+#ifdef __sun__
+      struct stat entry_stat;
+
+      lstat(metric_entry->d_name, &entry_stat);
+      if ((!S_ISDIR(entry_stat.st_mode) && !S_ISLNK(entry_stat.st_mode)) ||
+          metric_entry->d_name[0] == '.') 
+#else
       if ((metric_entry->d_type != DT_DIR &&
            metric_entry->d_type != DT_LNK) ||
           metric_entry->d_name[0] == '.')
+#endif
          continue;
 
       DBG("metric set: %s\n", metric_entry->d_name);
@@ -1946,9 +1954,17 @@ get_sysfs_dev_dir(struct brw_context *br
    }
 
    while ((drm_entry = readdir(drmdir))) {
+#ifdef __sun__
+      struct stat drm_stat;
+
+      lstat(drm_entry->d_name, &drm_stat);
+      if ((S_ISDIR(drm_stat.st_mode) || S_ISLNK(drm_stat.st_mode)) &&
+          strncmp(drm_entry->d_name, "card", 4) == 0)
+#else
       if ((drm_entry->d_type == DT_DIR ||
            drm_entry->d_type == DT_LNK) &&
           strncmp(drm_entry->d_name, "card", 4) == 0)
+#endif
       {
          len = snprintf(path_buf, path_buf_len,
                         "/sys/dev/char/%d:%d/device/drm/%s",