--- vcl/Executable_visualbackendtest.mk.orig	2018-02-08 14:35:19 UTC
+++ vcl/Executable_visualbackendtest.mk
@@ -41,7 +41,7 @@ $(eval $(call gb_Executable_use_static_l
     vclmain \
 ))
 
-ifeq ($(OS),LINUX)
+ifeq ($(OS), $(filter LINUX %BSD, $(OS)))
 $(eval $(call gb_Executable_add_libs,visualbackendtest,\
 	-lm \
 	-ldl \
