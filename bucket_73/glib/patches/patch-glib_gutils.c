--- glib/gutils.c.orig	2018-08-17 00:03:20 UTC
+++ glib/gutils.c
@@ -2040,7 +2040,7 @@ g_get_system_data_dirs (void)
 
 #ifndef G_OS_WIN32
       if (!data_dirs || !data_dirs[0])
-          data_dirs = "/usr/local/share/:/usr/share/";
+          data_dirs = "/usr/local/share/";
 
       data_dir_vector = g_strsplit (data_dirs, G_SEARCHPATH_SEPARATOR_S, 0);
 #else
@@ -2116,7 +2116,7 @@ g_get_system_config_dirs (void)
 	}
 #else
       if (!conf_dirs || !conf_dirs[0])
-          conf_dirs = "/etc/xdg";
+          conf_dirs = "/usr/local/etc/xdg:/etc/xdg";
 
       conf_dir_vector = g_strsplit (conf_dirs, G_SEARCHPATH_SEPARATOR_S, 0);
 #endif
