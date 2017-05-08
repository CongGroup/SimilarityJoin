#include "EncIndex.h"


#include <stdint.h>
#include <string.h>
#include <string>
#include <iostream>
#include <string>
#include <stdint.h>
#include <random>
#include <ctime>

#include "../Caravel/BukHash.h"
#include "../Caravel/SemCtl.h"
#include "../Caravel/ShmCtl.h"
#include "../Caravel/Digest.h"
#include "../Caravel/TimeDiff.h"
#include "../Caravel/PRF.h"

using namespace std;
using namespace caravel;

EncIndex::EncIndex()
{
}


EncIndex::~EncIndex()
{
    //delete[] m_arCounterBitmap;
}

uint64_t EncIndex::Init(uint32_t uiAllNum, double dLoad, uint32_t uiFloor, key_t keyShm)
{

    uint32_t uiL = uiFloor;
    uiAllNum = uiAllNum / dLoad + 1;
    uint32_t uiW = uiAllNum / uiL + 1;
    if (0 == keyShm)
    {
        m_bukHash.Create((char*)NULL, uiW, uiL, true);
    }
    else
    {
        m_bukHash.Create(keyShm, uiW, uiL, true);
    }
	if (m_bukHash.GetErr().size() > 0)
	{
		cout << m_bukHash.GetErr() << endl;
	}
}

void EncIndex::AttachIndex(uint32_t uiAllNum, double dLoad, uint32_t uiFloor, key_t keyShm)
{
    uint32_t uiL = uiFloor;
    uiAllNum = uiAllNum / dLoad + 1;
    uint32_t uiW = uiAllNum / uiL + 1;
    m_bukHash.Create(keyShm, uiW, uiL, false);
}

void EncIndex::BuildIndex(uint32_t **arAllLsh, uint32_t uiNum, uint32_t uiAllL)
{
    for (uint32_t uiCur = 0; uiCur < uiNum; uiCur++)
    {
        uint32_t *arLsh = arAllLsh[uiCur];
        for (uint32_t uiL = 0; uiL < uiAllL; uiL++)
        {
            uint32_t uiID = uiCur;
            uint32_t uiLsh = arLsh[uiL];
            Insert(uiLsh, uiL, uiID);
        }

        if (uiCur % (uiNum / 100) == 0)
        {
            cout << "Build Index => " << uiCur / (uiNum / 100) << "%" << endl;
        }

    }
}


void EncIndex::GenerateTrapdoorAndMask(uint32_t uiLsh, uint32_t uiF, char *pCombine, uint32_t uiCombine, char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask)
{
    //LSH + Floor => Combine
    PRF::Sha256((char*)&uiF, sizeof(uint32_t), (char*)&uiLsh, sizeof(uint32_t), pCombine, uiCombine);
    //Init trapdoor
    PRF::Sha256(m_TrapdoorKey, sizeof(m_TrapdoorKey), pCombine, uiCombine, pTrapdoor, sizeof(uiTrapdoor));
    //Init Mask
    PRF::Sha256(m_MaskKey, sizeof(m_MaskKey), pCombine, uiCombine, pMask, sizeof(uiMask));
}

void EncIndex::Insert(uint32_t uiLsh, uint32_t uiF, uint32_t uiVal)
{
    char szCombine[SHA256_DIGEST_LENGTH];
    char szTD[SHA256_DIGEST_LENGTH];
    char szMask[SHA256_DIGEST_LENGTH];
    GenerateTrapdoorAndMask(uiLsh, uiF, szCombine, SHA256_DIGEST_LENGTH, szTD, SHA256_DIGEST_LENGTH, szMask, SHA256_DIGEST_LENGTH);
    //Add Counter
	uint64_t uiCounterKey = m_BinaryToUint64(szCombine, SHA256_DIGEST_LENGTH);
    uint32_t uiC = m_GetCounterAndInc(uiCounterKey);
    //cout << "uiC = m_GetCounterAndInc(" << uiCounterKey << ") = " << uiC << endl;
    char szTrapdoorAddC[SHA256_DIGEST_LENGTH];
    PRF::Sha256((char*)&uiC, sizeof(uint32_t), szTD, sizeof(szTD), szTrapdoorAddC, sizeof(szTrapdoorAddC));
    
    m_Set(szTrapdoorAddC, SHA256_DIGEST_LENGTH, szMask, SHA256_DIGEST_LENGTH, uiVal);

}

void EncIndex::QueryOne(uint32_t uiLsh, uint32_t uiF, vector<uint32_t> &vecResult)
{
    char szCombine[SHA256_DIGEST_LENGTH];
    char szTD[SHA256_DIGEST_LENGTH];
    char szMask[SHA256_DIGEST_LENGTH];
	//cout << "L is " << uiLsh << " F is " << uiF << " Size is " << SHA256_DIGEST_LENGTH << endl;
    GenerateTrapdoorAndMask(uiLsh, uiF, szCombine, SHA256_DIGEST_LENGTH, szTD, SHA256_DIGEST_LENGTH, szMask, SHA256_DIGEST_LENGTH);
    //Add Counter
    uint32_t uiC = 0;
    while (true)
    {
        char szTrapdoorAddC[SHA256_DIGEST_LENGTH];
        PRF::Sha256((char*)&uiC, sizeof(uint32_t), szTD, sizeof(szTD), szTrapdoorAddC, sizeof(szTrapdoorAddC));
        int iID = m_Get(szTrapdoorAddC, SHA256_DIGEST_LENGTH, szMask, SHA256_DIGEST_LENGTH);

        if (iID == -1)
        {
            return;
        }
        vecResult.push_back(iID);

        uiC++;

    }
}

uint32_t EncIndex::getIndexSize()
{
	return m_bukHash.getCapacity();
}

void EncIndex::TestEncIndex(uint32_t uiLsh, uint32_t uiVal)
{
    cout << "Begin to Test Insert 10 value" << endl;

    for (uint32_t uiCur = 0; uiCur < 10; uiCur++)
    {
        Insert(uiLsh, 0, uiVal++);
    }

    vector<uint32_t> vecResult;
    QueryOne(uiLsh, 0, vecResult);

    cout << "Query the uiRet = ";
    for (vector<uint32_t>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
}

void EncIndex::Query(char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask, vector<uint32_t> &vecResult)
{
    uint32_t uiC = 0;
    while (true)
    {

        char szTrapdoorAddC[SHA256_DIGEST_LENGTH];
        PRF::Sha256((char*)&uiC, sizeof(uint32_t), pTrapdoor, uiTrapdoor, szTrapdoorAddC, sizeof(szTrapdoorAddC));
        int iID = m_Get(szTrapdoorAddC, SHA256_DIGEST_LENGTH, pMask, uiMask);

        if (iID == -1)
        {
            return;
        }

        vecResult.push_back(iID);

        uiC++;

    }
    return;
}


void EncIndex::SetKey(string strMasterKey)
{
    string sKey = strMasterKey + "Trapdoor";
    Digest::Sha256(sKey, m_TrapdoorKey, sizeof(m_TrapdoorKey));
    sKey = strMasterKey + "Mask";
    Digest::Sha256(sKey, m_MaskKey, sizeof(m_MaskKey));
}

int EncIndex::m_Get(char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask)
{
	uint64_t uiKey = m_BinaryToUint64(pTrapdoor, uiTrapdoor);
    stBukEncBlock *pRet = m_bukHash.Get(uiKey, false);
    if (NULL == pRet)
    {
        return -1;
    }
    stBukEncBlock *p = m_Mask(pRet, pMask, uiMask);

    return p->iID;
}

bool EncIndex::m_Set(char *pTrapdoor, uint32_t uiTrapdoor, char *pMask, uint32_t uiMask, uint32_t uiValue)
{
	uint64_t uiKey = m_BinaryToUint64(pTrapdoor, uiTrapdoor);
    stBukEncBlock stBlock;
    stBlock.iID = uiValue;
    stBukEncBlock *p = m_Mask(&stBlock, pMask, uiMask);
    return m_bukHash.Set(uiKey, p);
}

//uint32_t EncIndex::m_BinaryToUint32(char *pData, uint32_t uiLen)
//{
//    uint32_t *pCur = (uint32_t*)pData;
//    uint32_t uiRet = *pCur++;
//    uiLen -= sizeof(uint32_t);
//    while (uiLen >= sizeof(uint32_t))
//    {
//        uiRet ^= *pCur++;
//        uiLen -= sizeof(uint32_t);
//    }
//    return uiRet;
//}

uint64_t EncIndex::m_BinaryToUint64(char *pData, uint32_t uiLen)
{
	uint64_t *pCur = (uint64_t*)pData;
	uint64_t uiRet = *pCur;
	uiLen -= sizeof(uint64_t);
	while (uiLen >= sizeof(uint64_t))
	{
		uiRet ^= *(pCur++);
		uiLen -= sizeof(uint64_t);
	}
	return uiRet;
}





