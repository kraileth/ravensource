--- lib/erl_interface/src/connect/ei_resolve.c.orig	2018-03-09 10:01:10 UTC
+++ lib/erl_interface/src/connect/ei_resolve.c
@@ -650,7 +650,8 @@ struct hostent *ei_gethostbyname_r(const
 
   return result;
 #else
-  return gethostbyname_r(name,hostp,buffer,buflen,h_errnop);
+  struct hostent *dummy;
+  return gethostbyname_r(name,hostp,buffer,buflen,&dummy,h_errnop);
 #endif
 #endif
 #endif
