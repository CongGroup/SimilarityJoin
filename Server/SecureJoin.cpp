#include <fstream>
#include <map>
#include <time.h>
#include <sstream>
#include <sys/shm.h>

#include "SecureJoin.h"
#include "../Caravel/BukHash.h"
#include "../Caravel/ShmCtl.h"


using namespace std;
using namespace caravel;

SecureJoin::SecureJoin()
{
	init();
}

SecureJoin::~SecureJoin()
{
	free();
}

int SecureJoin::init()
{
	arMaxBoundry = new int[uiFinanceDimension];
	vector<string> splitRes;
	splitString(strMaxBoundry, splitRes, " ");
	for (int i = 0; i < uiFinanceDimension; i++)
	{
		if (i < splitRes.size())
		{
			arMaxBoundry[i] = stod(splitRes[i]);
		}
		else
		{
			arMaxBoundry[i] = 0;
		}
	}

	encIndex.SetKey(masterKey);

	return 0;
}

int SecureJoin::free()
{
	checkAndDeleteArr((void**)&arMaxBoundry);
	if (arMetaVal != NULL)
	{
		for (int i = 0; i < uiAllNum; i++)
		{
			checkAndDeleteArr((void**)&arMetaVal[i]);
		}
		delete[] arMetaVal;
		arMetaVal = NULL;
	}
	return 0;
}

uint32_t SecureJoin::loadData(string mataDataPaht, uint32_t maxSize)
{
	uint32_t tmpAllNum = countLines(mataDataPaht);
	uint32_t tmpDimension = countDimension(mataDataPaht);

	ifstream ifs;
	ifs.open(mataDataPaht, ios::in);
	string strLines;

	if (ifs.fail() || uiAllNum < 0 || uiDataDimension < 0)
	{
		cout << "The file did not exists." << endl;
		return -1;
	}
	else
	{
		uiAllNum = tmpAllNum;
		uiDataDimension = tmpDimension;
		indexDistributed.total = uiAllNum;

		arMetaVal = new double*[uiAllNum];
		for (int i = 0; i < uiAllNum; i++)
		{
			arMetaVal[i] = new double[uiDataDimension];
		}

		vector<string> splitRes;
		uint32_t uiCurNum = 0;
		while (getline(ifs, strLines, '\n'))
		{
			splitRes.clear();
			splitString(strLines, splitRes, " ");
			for (int i = 0; i < splitRes.size() && i < uiDataDimension; i++)
			{
				try
				{
					arMetaVal[uiCurNum][i] = stod(splitRes[i]);
				}
				catch (invalid_argument e)
				{
					cout << splitRes[i] << "can`t be convert to double" << endl;
					arMetaVal[uiCurNum][i] = 0;
				}
			}
			indexDistributed.counter[checkLevel(arMetaVal[uiCurNum])]++;
			uiCurNum++;
		}
		ifs.close();
		return tmpAllNum;
	}
}

bool SecureJoin::computeLSH(uint32_t L, double w)
{
	if (arMetaVal == NULL)
	{
		return false;
	}
	uiLshL = L;
	dLshW = w;
	if (arMetaVal[0][0] > 1)
	{
		formalize(arMetaVal, uiAllNum, uiDataDimension);
	}


	c2lsh.Init(uiDataDimension, uiLshL, dLshW);
	arLsh = new uint32_t*[uiAllNum];

	for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
	{
		arLsh[uiCur] = new uint32_t[uiLshL];
		c2lsh.Compute(arMetaVal[uiCur], arLsh[uiCur]);
	}

	return true;
}

bool SecureJoin::computeLsh(uint32_t * lsh, double * mateData)
{
	if (lsh&&mateData)
	{
		c2lsh.Compute(mateData, lsh);
	}

	return false;
}

bool SecureJoin::buildIndex(uint32_t size)
{
	if (arLsh == NULL)
	{
		return false;
	}
	uiUserNum = size;

	int iShmID = shmget(shmKey, 0, 0);
	if (iShmID >= 0)
	{
		encIndex.AttachIndex(uiUserNum * uiLshL, 0.7, 24, shmKey);
	}
	else
	{
		encIndex.Init(uiUserNum * uiLshL, 0.7, 24, shmKey);

		encIndex.BuildIndex(arLsh, uiUserNum, uiLshL);
	}



	encIndex.ShowBukHashState();
	indexSize = encIndex.getIndexSize();
	indexMomery = indexSize * sizeof(BukEncBlock);

	return true;
}

string SecureJoin::getMataDataByID(uint32_t id)
{
	stringstream ss;
	if (id < uiAllNum)
	{
		for (int i = 0; i < uiDataDimension; i++)
		{
			ss << arMetaVal[i];
			if (i != uiDataDimension - 1)
			{
				ss << " ";
			}
		}
	}
	return ss.str();
}

string SecureJoin::getTypeByID(uint32_t id)
{
	string level;
	if (id < uiAllNum)
	{
		int lvl = checkLevel(arMetaVal[id]);
		level.append(1, Type2c(lvl));
	}
	return level;
}

string SecureJoin::getTypeByData(double * oneMateData)
{
	string level;
	level.append(1, Type2c(checkLevel(oneMateData)));
	return level;
}

SecureJoin::Proportion SecureJoin::getDistributedByID(vector<uint32_t> ids)
{
	set<int> setIDs;
	setIDs.insert(ids.begin(), ids.end());
	return countLevel(arMetaVal, ids);
}

SecureJoin::Proportion SecureJoin::getDistributedofIndex()
{
	return indexDistributed;
}

vector<int> SecureJoin::joinByStrategy1(double ** joinMataData, int num, int ThresholdK, int timeout)
{
	uint32_t uiJoinNum = num;
	uint64_t ulNeedBandwidthNum = 0;
	uint32_t uiLimitK = ThresholdK;

	set<uint32_t> setResult;
	markSecond();

	uint32_t **queryLsh = new uint32_t*[uiJoinNum];
	for (int i = 0; i < uiJoinNum; i++)
	{
		queryLsh[i] = new uint32_t[uiDataDimension];
		computeLsh(queryLsh[i], joinMataData[i]);
	}

	for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
	{
		uint32_t *arQueryLsh = queryLsh[uiCur];

		vector<uint32_t> vecResult;
		for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
		{
			uint32_t uiLsh = arQueryLsh[uiL];
			encIndex.QueryOne(uiLsh, uiL, vecResult);
		}

		map<uint32_t, uint32_t> mapCombine;
		vector<uint32_t> vecResultInK;
		for (vector<uint32_t>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
		{
			if (++mapCombine[*it] == uiLimitK)
			{
				vecResultInK.push_back(*it);
			}
		}
		ulNeedBandwidthNum += vecResultInK.size();

		setResult.insert(vecResultInK.begin(), vecResultInK.end());

		if (timeout -= markSecond() < 0)
		{
			break;
		}

	}
	cout << "ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;

	vector<int> vecRes;
	for (auto i = setResult.begin(); i != setResult.end(); i++)
	{
		vecRes.push_back((int)*i);
	}

	for (int i = 0; i < uiJoinNum; i++)
	{
		delete[]queryLsh[i];
	}
	delete[]queryLsh;
	return vecRes;

}

vector<int> SecureJoin::joinByStrategy2(double ** joinMataData, int num, int ThresholdK, int timeout)
{
	uint32_t uiJoinNum = num;
	uint32_t uiLimitK = ThresholdK;

	uint64_t ulNeedBandwidthNum = 0;
	uint32_t uiHitCache = 0;
	uint64_t ulSaveResultNum = 0;

	map<uint32_t, map<uint32_t, vector<uint32_t> > >mapCacheResult;
	set<uint32_t> setResult;
	markSecond();

	uint32_t **queryLsh = new uint32_t*[uiJoinNum];
	for (int i = 0; i < uiJoinNum; i++)
	{
		queryLsh[i] = new uint32_t[uiDataDimension];
		computeLsh(queryLsh[i], joinMataData[i]);
	}

	for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
	{
		uint32_t *arQueryLsh = queryLsh[uiCur];
		map<uint32_t, uint32_t> mapCombine;

		for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
		{
			uint32_t uiLsh = arQueryLsh[uiL];
			vector<uint32_t> *pVecReslt = &mapCacheResult[uiLsh][uiL];

			if (pVecReslt->size() != 0)
			{
				uiHitCache++;
				ulSaveResultNum += pVecReslt->size();
			}
			else
			{
				encIndex.QueryOne(uiLsh, uiL, *pVecReslt);
				ulNeedBandwidthNum += pVecReslt->size();
			}
			for (vector<uint32_t>::iterator it = pVecReslt->begin(); it != pVecReslt->end(); it++)
			{
				mapCombine[*it]++;
			}
		}

		vector<uint32_t> vecResultInK;

		for (map<uint32_t, uint32_t>::iterator it = mapCombine.begin(); it != mapCombine.end(); it++)
		{
			if (it->second > uiLimitK)
			{
				vecResultInK.push_back(it->first);
			}
		}
		setResult.insert(vecResultInK.begin(), vecResultInK.end());

		if (timeout -= markSecond() < 0)
		{
			break;
		}

	}
	cout << "ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;
	cout << "uiHitCache = " << uiHitCache << endl;
	cout << "ulSaveResultNum = " << ulSaveResultNum << endl;

	vector<int> vecRes;
	for (auto i = setResult.begin(); i != setResult.end(); i++)
	{
		vecRes.push_back((int)*i);
	}

	for (int i = 0; i < uiJoinNum; i++)
	{
		delete[]queryLsh[i];
	}
	delete[]queryLsh;
	return vecRes;
}

vector<int> SecureJoin::joinByStrategy3(double ** joinMataData, int num, int ThresholdK, double selfQueryR, int timeout)
{
	uint32_t uiJoinNum = num;
	double dSelfQueryR = selfQueryR;

	uint32_t **queryLsh = new uint32_t*[uiJoinNum];
	for (int i = 0; i < uiJoinNum; i++)
	{
		queryLsh[i] = new uint32_t[uiDataDimension];
		computeLsh(queryLsh[i], joinMataData[i]);
	}

	vector<uint32_t> vecJoin;
	for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
	{
		vecJoin.push_back(uiCur);
	}

	vector<uint32_t> vecSelfQuery;

	while (vecJoin.size() != 0)
	{
		uint32_t uiOffset = rand() % vecJoin.size();
		uint32_t uiQuerySample = vecJoin[uiOffset];

		//delete it from vecQuery
		vecJoin.erase(vecJoin.begin() + uiOffset);
		//Add the sample point to result vector
		vecSelfQuery.push_back(uiQuerySample);
		//query for others
		for (vector<uint32_t>::iterator it = vecJoin.begin(); it != vecJoin.end();)
		{
			if (C2Lsh::ComputeL2(joinMataData[uiQuerySample], joinMataData[*it], uiDataDimension) < dSelfQueryR)
			{
				it = vecJoin.erase(it);
				if (it == vecJoin.end())
				{
					break;
				}
			}
			else
			{
				it++;
			}
		}
	}

	double ** arrSelfQueryMataData = new double *[vecSelfQuery.size()];
	for (int i = 0; i < vecSelfQuery.size(); i++)
	{
		arrSelfQueryMataData[i] = new double[uiDataDimension];
		memcpy(arrSelfQueryMataData, joinMataData[vecSelfQuery[i]], sizeof(double)*uiDataDimension);
	}

	vector<int> res = joinByStrategy2(arrSelfQueryMataData, vecSelfQuery.size(), ThresholdK, timeout);

	for (int i = 0; i < uiJoinNum; i++)
	{
		delete[]queryLsh[i];
	}
	delete[]queryLsh;
	for (int i = 0; i < vecSelfQuery.size(); i++)
	{
		delete[]arrSelfQueryMataData[i];
	}
	delete[]arrSelfQueryMataData;

	return res;
}

int SecureJoin::countLines(string sPath)
{
	ifstream ifs;
	ifs.open(sPath, ios::in);
	int iLines = 0;
	string strLines;
	if (ifs.fail())
	{
		return -1;
	}
	else
	{
		while (getline(ifs, strLines, '\n'))
		{
			iLines++;
		}
	}
	ifs.close();
	return iLines;
}

int SecureJoin::countDimension(string sPath)
{
	ifstream ifs;
	ifs.open(sPath, ios::in);
	int iDimension = 0;
	string strLines;
	if (ifs.fail())
	{
		return -1;
	}
	else
	{
		getline(ifs, strLines, '\n');
		vector<string> splitRes;
		splitString(strLines, splitRes, " ");
		iDimension = splitRes.size();
		ifs.close();
		return iDimension;
	}
}

int SecureJoin::splitString(const std::string& s, std::vector<std::string>& v, const std::string& c, bool notNull)
{
	std::string::size_type pos1, pos2;
	pos1 = 0;
	pos2 = s.find(c, pos1);
	while (std::string::npos != pos2)
	{
		if (!notNull || (pos2 - pos1) > 0)
			v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 < s.length())
		v.push_back(s.substr(pos1));

	return v.size();
}

void SecureJoin::normalize(double * arMetaData, uint32_t uiDimension)
{
	for (uint32_t uiD = 0; uiD < uiDimension; uiD++)
	{
		arMetaData[uiD] = arMetaData[uiD] / arMaxBoundry[uiD];
	}
}

void SecureJoin::formalize(double ** arMetaData, uint32_t uiAllNum, uint32_t uiDimension)
{
	for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
	{
		double *arVal = arMetaData[uiCur];
		double dMax = 0;
		double dMin = UINT32_MAX;
		for (uint32_t uiD = 0; uiD < uiDimension; uiD++)
		{
			dMax = dMax > arVal[uiD] ? dMax : arVal[uiD];
			dMin = dMin < arVal[uiD] ? dMin : arVal[uiD];
		}
		for (uint32_t uiD = 0; uiD < uiDimension; uiD++)
		{
			arVal[uiD] = (arVal[uiD] - dMin) / (dMax - dMin);
		}
	}
}

void SecureJoin::checkAndDeleteArr(void** p)
{
	if (p != nullptr && *p != nullptr)
	{
		delete[] * p;
		*p = nullptr;
	}
}

int SecureJoin::checkLevel(double* pMateData)
{
	//not normalize
	if (pMateData[0] > 1 && pMateData[1] > 1)
	{
		if (pMateData[1] < 10000)
		{
			return 0;
		}
		else if (pMateData[1] < 25000)
		{
			return 1;
		}
		else if (pMateData[1] < 50000)
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
	else
	{
		if (pMateData[1] < 10000 / 75000.0)
		{
			return 0;
		}
		else if (pMateData[1] < 25000 / 75000.0)
		{
			return 1;
		}
		else if (pMateData[1] < 50000 / 75000.0)
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
}

int SecureJoin::checkLevel(double** arMateData, uint32_t mateDataID)
{
	return checkLevel(arMateData[mateDataID]);
}

SecureJoin::Proportion SecureJoin::countLevel(double** pArrMateData, uint32_t datacount)
{
	Proportion res;
	res.total = datacount;
	for (int i = 0; i < datacount; i++)
	{
		int lvl = checkLevel(pArrMateData[i]);
		res.counter[lvl]++;
	}
	return res;
}

SecureJoin::Proportion SecureJoin::countLevel(double** pArrMateData, const vector<uint32_t>& ids)
{
	Proportion res;
	res.total = ids.size();
	for (int i = 0; i < ids.size(); i++)
	{
		if (ids[i] >= uiAllNum)
		{
			continue;
		}
		int lvl = checkLevel(pArrMateData, ids[i]);
		res.counter[lvl]++;
	}
	return res;
}

SecureJoin::Proportion SecureJoin::countLevel(double** pArrMateData, const set<uint32_t>& ids)
{
	Proportion res;
	res.total = ids.size();
	for (auto i = ids.begin(); i != ids.end(); i++)
	{
		if (*i >= uiAllNum)
		{
			continue;
		}
		int lvl = checkLevel(pArrMateData, *i);
		res.counter[lvl]++;
	}
	return res;
}

char SecureJoin::Type2c(int lvl)
{
	return (char)(lvl + 'A');
}

int SecureJoin::markSecond()
{
	static time_t t_cur;
	uint32_t ui_Time = time(NULL) - t_cur;
	t_cur = time(NULL);

	return ui_Time;
}
