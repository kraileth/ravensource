# work-around for https://bugs.freedesktop.org/show_bug.cgi?id=100627
#
--- src/egl/drivers/dri2/platform_x11.c.orig	2018-09-21 10:57:00 UTC
+++ src/egl/drivers/dri2/platform_x11.c
@@ -1516,6 +1516,9 @@ dri2_initialize_x11(_EGLDriver *drv, _EG
 
    if (!disp->Options.ForceSoftware) {
 #ifdef HAVE_DRI3
+#if (defined(__FreeBSD__) || defined(__FreeBSD_kernel__)) && !defined(__DRM_NEXT__)
+      if (env_var_as_boolean("LIBGL_DRI3_ENABLE", false))
+#endif
       if (!env_var_as_boolean("LIBGL_DRI3_DISABLE", false))
          initialized = dri2_initialize_x11_dri3(drv, disp);
 #endif
