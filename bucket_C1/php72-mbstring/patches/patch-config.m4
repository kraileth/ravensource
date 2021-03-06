--- config.m4.orig	2018-04-24 15:09:55 UTC
+++ config.m4
@@ -43,6 +43,30 @@ AC_DEFUN([PHP_MBSTRING_EXTENSION], [
     PHP_ADD_INCLUDE([$ext_builddir/$dir])
   done
 
+  dnl This is PECL build, check if bundled PCRE library is used
+  old_CPPFLAGS=$CPPFLAGS
+  CPPFLAGS=$INCLUDES
+  AC_EGREP_CPP(yes,[
+#include <main/php_config.h>
+#if defined(HAVE_BUNDLED_PCRE) && !defined(COMPILE_DL_PCRE)
+yes
+#endif
+  ],[
+    PHP_PCRE_REGEX=yes
+  ],[
+    AC_EGREP_CPP(yes,[
+#include <main/php_config.h>
+#if defined(HAVE_PCRE) && !defined(COMPILE_DL_PCRE)
+yes
+#endif
+    ],[
+      PHP_PCRE_REGEX=pecl
+      PHP_ADD_INCLUDE($PHP_PCRE_DIR/include)
+    ],[
+      PHP_PCRE_REGEX=no
+    ])
+  ])
+
   if test "$ext_shared" = "no"; then
     PHP_ADD_SOURCES(PHP_EXT_DIR(mbstring), $PHP_MBSTRING_BASE_SOURCES)
     out="php_config.h"
@@ -85,7 +109,6 @@ AC_DEFUN([PHP_MBSTRING_SETUP_MBREGEX], [
 int foo(int x, ...) {
 	va_list va;
 	va_start(va, x);
-	va_arg(va, int);
 	va_arg(va, char *);
 	va_arg(va, double);
 	return 0;
@@ -367,6 +390,9 @@ PHP_ARG_WITH(onig, [for external oniguru
 [  --with-onig[=DIR]         MBSTRING: Use external oniguruma. DIR is the oniguruma install prefix.
                           If DIR is not set, the bundled oniguruma will be used], no, no)
 
+PHP_ARG_WITH(pcre-dir, pcre install prefix,
+[  --with-pcre-dir           MBSTRING: pcre install prefix], no, no)
+
 if test "$PHP_MBSTRING" != "no"; then  
   AC_DEFINE([HAVE_MBSTRING],1,[whether to have multibyte string support])
 
