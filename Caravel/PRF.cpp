#include "PRF.h"
#include <stdint.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

using namespace std;


namespace caravel{

uint32_t PRF::Sha256(char *pKey, uint32_t uiKeyLen, char *pData, uint32_t uiDataLen, char *pOut, uint32_t uiOutLen)
{
    if(NULL != pOut)
    {
        memset(pOut, 0, uiOutLen);
    }
    //It places the result in md (which must have space for the output of the hash function, which is no more than EVP_MAX_MD_SIZE bytes). If md is NULL, the digest is placed in a static array. The size of the output is placed in md_len, unless it is NULL.
    HMAC(EVP_sha256(), pKey, uiKeyLen, (unsigned char*)pData, uiDataLen, (unsigned char*)pOut, &uiOutLen);
    return uiOutLen;
}

}
