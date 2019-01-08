/*#include "Hash.h"

unsigned char* hash(char* text){

	EVP_MD_CTX *mdctx;
	const EVP_MD *md = EVP_sha256();
	//Using SHA256 (8 * 32 = 256)
	unsigned char* md_value = new unsigned char[32]; 
	unsigned int md_len;

	OpenSSL_add_all_digests();

	const EVP_MD ;
	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, text, strlen(text));
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_destroy(mdctx);

	//After function call make sure to delete pointer
	return md_value;

}*/