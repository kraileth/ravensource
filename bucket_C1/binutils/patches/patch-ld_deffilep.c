--- ld/deffilep.c.orig	2018-01-13 14:08:33 UTC
+++ ld/deffilep.c
@@ -204,6 +204,8 @@ static const char *lex_parse_string_end
 extern int yydebug;
 #endif
 
+#undef SHARED
+
 /* Token type.  */
 #ifndef YYTOKENTYPE
 # define YYTOKENTYPE
