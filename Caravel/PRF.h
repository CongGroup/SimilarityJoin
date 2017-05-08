#ifndef __PRF_H__
#define __PRF_H__

#include <string>
#include <stdint.h>


using namespace std;


namespace caravel{

class PRF
{
public:

    //if pOut == NULL, it will pointer to a static array.
    //Do not use pOut = NULL !
    static uint32_t Sha256(char *pKey, uint32_t uiKeyLen, char *pData, uint32_t uiDataLen, char *pOut, uint32_t uiOutLen);

};

}


#endif
