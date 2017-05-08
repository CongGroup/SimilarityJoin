#ifndef __ENC_INDEX_H__
#define __ENC_INDEX_H__

#include <stdint.h>
#include <string.h>
#include <string>
#include <iostream>
#include <string>
#include <stdint.h>
#include <map>
#include <vector>

#include "../Caravel/BukHash.h"
#include "../Caravel/Digest.h"
#include "../Caravel/ShmCtl.h"

using namespace std;
using namespace caravel;

typedef struct _stQueryResult
{
    int iID;
    uint32_t uiCnt;
}stQueryResult;

#define DEF_BUKENCBLOCK_SIZE 32

typedef union stBukEncBlock
{
    int iID;
    char szBuf[DEF_BUKENCBLOCK_SIZE];
}BukEncBlock;

class EncIndex
{
public:
    EncIndex();
    ~EncIndex();

    uint64_t Init(uint32_t uiAllNum, double dLoad, uint32_t uiFloor, key_t keyShm);

    /*
    void InitCounterBitmap()
    {
        size_t sizeBitmap = sizeof(uint32_t) * UINT32_MAX;
        m_arCounterBitmap = (uint32_t*)malloc(sizeBitmap);
        cout << "Malloc memory size = " << sizeBitmap << endl;

    }
    */

    void SetKey(string strMasterKey);

    void BuildIndex(uint32_t **arAllLsh, uint32_t uiNum, uint32_t uiAllL);

    void AttachIndex(uint32_t uiAllNum, double dLoad, uint32_t uiFloor, key_t keyShm);

    void Query(char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask, vector<uint32_t> &vecResult);

    void GenerateTrapdoorAndMask(uint32_t uiLsh, uint32_t uiF, char *pCombine, uint32_t uiCombine, char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask);

    map<uint64_t, uint32_t> *GetCounterMap()
    {
        return &m_mapLshCounter;
    }

    void ShowBukHashState()
    {
		m_bukHash.GetRealNum();
        m_bukHash.PrintState();
    }

    void TestEncIndex(uint32_t uiLsh, uint32_t uiVal);

    void Insert(uint32_t uiLsh, uint32_t uiF, uint32_t uiVal);

    void QueryOne(uint32_t uiLsh, uint32_t uiF, vector<uint32_t> &vecResult);

	uint32_t getIndexSize();

private:

    int m_Get(char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask);

    bool m_Set(char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask, uint32_t uiValue);

    //inline uint32_t m_BinaryToUint32(char *pData, uint32_t uiLen);
	inline uint64_t m_BinaryToUint64(char *pData, uint32_t uiLen);

    //inline function

    stBukEncBlock *m_Mask(stBukEncBlock *pBlock, char *pMask, uint32_t uiMask)
    {
        static stBukEncBlock bukEncBlock;
        for (uint32_t uiCur = 0; uiCur < uiMask; uiCur++)
        {
            bukEncBlock.szBuf[uiCur] = pBlock->szBuf[uiCur] ^ pMask[uiCur];
        }
        return &bukEncBlock;
    }

    uint32_t m_GetCounterAndInc(uint64_t uiLsh)
    {
        return m_mapLshCounter[uiLsh]++;
        //return m_arCounterBitmap[uiLsh]++;
    }

    uint32_t m_GetCounter(uint64_t uiLsh)
    {
        return m_mapLshCounter[uiLsh];
        //return m_arCounterBitmap[uiLsh];
    }

    //member vars

    //For Counter
    map<uint64_t, uint32_t> m_mapLshCounter;
    //For Counter Enhance use BucketHash
    //Bitmap
    uint32_t *m_arCounterBitmap;

    //Keys
    char m_TrapdoorKey[SHA256_DIGEST_LENGTH];
    char m_MaskKey[SHA256_DIGEST_LENGTH];

    BukHash<uint64_t, stBukEncBlock> m_bukHash;

};
#endif // __ENC_INDEX_H__

