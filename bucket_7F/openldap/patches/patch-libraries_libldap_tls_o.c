Fix build with LibreSSL.

--- libraries/libldap/tls_o.c.orig	2017-06-01 20:01:07 UTC
+++ libraries/libldap/tls_o.c
@@ -47,7 +47,7 @@
 #include <ssl.h>
 #endif
 
-#if OPENSSL_VERSION_NUMBER >= 0x10100000
+#if OPENSSL_VERSION_NUMBER >= 0x10100000 && !defined(LIBRESSL_VERSION_NUMBER)
 #define ASN1_STRING_data(x)	ASN1_STRING_get0_data(x)
 #endif
 
@@ -157,7 +157,7 @@ tlso_init( void )
 	(void) tlso_seed_PRNG( lo->ldo_tls_randfile );
 #endif
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000
+#if OPENSSL_VERSION_NUMBER < 0x10100000 || defined(LIBRESSL_VERSION_NUMBER)
 	SSL_load_error_strings();
 	SSL_library_init();
 	OpenSSL_add_all_digests();
@@ -205,7 +205,7 @@ static void
 tlso_ctx_ref( tls_ctx *ctx )
 {
 	tlso_ctx *c = (tlso_ctx *)ctx;
-#if OPENSSL_VERSION_NUMBER < 0x10100000
+#if OPENSSL_VERSION_NUMBER < 0x10100000 || defined(LIBRESSL_VERSION_NUMBER)
 #define	SSL_CTX_up_ref(ctx)	CRYPTO_add( &(ctx->references), 1, CRYPTO_LOCK_SSL_CTX )
 #endif
 	SSL_CTX_up_ref( c );
@@ -464,7 +464,7 @@ tlso_session_my_dn( tls_session *sess, s
 	if (!x) return LDAP_INVALID_CREDENTIALS;
 	
 	xn = X509_get_subject_name(x);
-#if OPENSSL_VERSION_NUMBER < 0x10100000
+#if OPENSSL_VERSION_NUMBER < 0x10100000 || defined(LIBRESSL_VERSION_NUMBER)
 	der_dn->bv_len = i2d_X509_NAME( xn, NULL );
 	der_dn->bv_val = xn->bytes->data;
 #else
@@ -500,7 +500,7 @@ tlso_session_peer_dn( tls_session *sess,
 		return LDAP_INVALID_CREDENTIALS;
 
 	xn = X509_get_subject_name(x);
-#if OPENSSL_VERSION_NUMBER < 0x10100000
+#if OPENSSL_VERSION_NUMBER < 0x10100000 || defined(LIBRESSL_VERSION_NUMBER)
 	der_dn->bv_len = i2d_X509_NAME( xn, NULL );
 	der_dn->bv_val = xn->bytes->data;
 #else
@@ -721,7 +721,7 @@ struct tls_data {
 	Sockbuf_IO_Desc		*sbiod;
 };
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000
+#if OPENSSL_VERSION_NUMBER < 0x10100000 || defined(LIBRESSL_VERSION_NUMBER)
 #define BIO_set_init(b, x)	b->init = x
 #define BIO_set_data(b, x)	b->ptr = x
 #define BIO_clear_flags(b, x)	b->flags &= ~(x)
@@ -822,7 +822,7 @@ tlso_bio_puts( BIO *b, const char *str )
 	return tlso_bio_write( b, str, strlen( str ) );
 }
 
-#if OPENSSL_VERSION_NUMBER >= 0x10100000
+#if OPENSSL_VERSION_NUMBER >= 0x10100000 && !defined(LIBRESSL_VERSION_NUMBER)
 struct bio_method_st {
     int type;
     const char *name;
