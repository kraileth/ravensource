--- lib/src/facts/posix/cache.cc.orig	2018-09-14 15:27:42 UTC
+++ lib/src/facts/posix/cache.cc
@@ -3,7 +3,7 @@
 namespace facter { namespace facts { namespace cache {
 
     std::string fact_cache_location() {
-        return "/opt/puppetlabs/facter/cache/cached_facts/";
+        return "%%PREFIX%%/var/facter/cache/cached_facts/";
     }
 
 }}}  // namespace facter::facts::cache
