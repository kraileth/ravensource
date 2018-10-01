--- bfd/elfxx-x86.c.orig	2018-07-10 07:12:05 UTC
+++ bfd/elfxx-x86.c
@@ -28,8 +28,8 @@
    section.  */
 
 #define ELF32_DYNAMIC_INTERPRETER "/usr/lib/libc.so.1"
-#define ELF64_DYNAMIC_INTERPRETER "/lib/ld64.so.1"
-#define ELFX32_DYNAMIC_INTERPRETER "/lib/ldx32.so.1"
+#define ELF64_DYNAMIC_INTERPRETER "@ELF64_INTERPRETER@"
+#define ELFX32_DYNAMIC_INTERPRETER "@ELF32_INTERPRETER@"
 
 bfd_boolean
 _bfd_x86_elf_mkobject (bfd *abfd)
