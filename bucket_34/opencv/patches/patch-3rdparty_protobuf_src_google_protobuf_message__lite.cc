Needed for INT_MAX on sunos

--- 3rdparty/protobuf/src/google/protobuf/message_lite.cc.orig	2018-07-04 11:05:47 UTC
+++ 3rdparty/protobuf/src/google/protobuf/message_lite.cc
@@ -40,6 +40,7 @@
 #include <google/protobuf/message_lite.h>
 #include <google/protobuf/repeated_field.h>
 #include <string>
+#include <climits>
 #include <google/protobuf/stubs/logging.h>
 #include <google/protobuf/stubs/common.h>
 #include <google/protobuf/io/coded_stream.h>
