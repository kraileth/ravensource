From d0dabb46a821b2506681f882af0d5696d2c2bade Mon Sep 17 00:00:00 2001
From: Michael Simacek <msimacek@redhat.com>
Date: Thu, 8 Feb 2018 14:47:08 +0100
Subject: [PATCH] Remove invalid pure_func qualifiers

---
 include/nasmlib.h | 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

--- include/nasmlib.h
+++ include/nasmlib.h
@@ -191,8 +191,8 @@ int64_t readstrnum(char *str, int length, bool *warn);
  * seg_init: Initialise the segment-number allocator.
  * seg_alloc: allocate a hitherto unused segment number.
  */
-void pure_func seg_init(void);
-int32_t pure_func seg_alloc(void);
+void seg_init(void);
+int32_t seg_alloc(void);
 
 /*
  * many output formats will be able to make use of this: a standard
-- 
2.14.3

