Index: programs/Xserver/hw/xfree86/os-support/bsd/i386_video.c
--- hw/xfree86/os-support/bsd/i386_video.c.orig	2018-05-10 16:32:35 UTC
+++ hw/xfree86/os-support/bsd/i386_video.c
@@ -32,6 +32,7 @@
 #include "xf86Priv.h"
 
 #include <errno.h>
+#include <sys/types.h>
 #include <sys/mman.h>
 
 #include "xf86_OSlib.h"
