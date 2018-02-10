--- src/nvim/macros.h.orig	2017-11-18 11:39:14 UTC
+++ src/nvim/macros.h
@@ -151,7 +151,9 @@
 #define RGB(r, g, b) ((r << 16) | (g << 8) | b)
 
 #define STR_(x) #x
+#ifndef __sun__
 #define STR(x) STR_(x)
+#endif
 
 #ifndef __has_attribute
 # define NVIM_HAS_ATTRIBUTE(x) 0