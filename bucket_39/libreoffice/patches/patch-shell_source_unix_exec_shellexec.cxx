--- shell/source/unix/exec/shellexec.cxx.orig	2018-02-08 14:35:19 UTC
+++ shell/source/unix/exec/shellexec.cxx
@@ -145,7 +145,7 @@ void SAL_CALL ShellExec::execute( const
         aBuffer.append("open --");
 #else
         // Just use xdg-open on non-Mac
-        aBuffer.append("/usr/bin/xdg-open");
+        aBuffer.append("%%LOCALBASE%%/bin/xdg-open");
 #endif
         aBuffer.append(" ");
         escapeForShell(aBuffer, OUStringToOString(aURL, osl_getThreadTextEncoding()));
