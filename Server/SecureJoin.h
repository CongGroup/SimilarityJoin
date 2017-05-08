#pragma once

#include <string>
#include <vector>
#include <set>
#include <stdint.h>

#include "../Caravel/EncIndex.h"
#include "../Caravel/C2Lsh.h"

using std::string;


class SecureJoin
{
public:
	struct Proportion
	{
		uint32_t total = 0;
		uint32_t counter[4] = { 0 };
	};

public:
	SecureJoin();
	~SecureJoin();
	int init();
	int free();

	uint32_t loadData(string mataDataPaht, uint32_t maxSize = UINT32_MAX);
	bool computeLSH(uint32_t L, double w);
	bool computeLsh(uint32_t* lsh, double* mateData);
	bool buildIndex(uint32_t size);
	string getMataDataByID(uint32_t id);
	string getTypeByID(uint32_t id);
	string getTypeByData(double* oneMateData);
	Proportion getDistributedByID(vector<uint32_t> ids);
	Proportion getDistributedofIndex();
	vector<int> joinByStrategy1(double** joinMataData, int num, int ThresholdK, int timeout = INT32_MAX);
	vector<int> joinByStrategy2(double** joinMataData, int num, int ThresholdK, int timeout = INT32_MAX);
	vector<int> joinByStrategy3(double** joinMataData, int num, int ThresholdK, double selfQueryR,int timeout = INT32_MAX);

	uint32_t uiAllNum = 0;
	uint32_t uiUserNum = 0;
	uint32_t indexSize = 0;
	uint32_t indexMomery = 0;

	uint32_t uiDataDimension = 0;
	const uint32_t uiFinanceDimension = 102;

	Proportion indexDistributed;

	int splitString(const string& s, vector<string>& v, const string& c, bool notNull = true);
	void normalize(double *arMetaData, uint32_t uiDimension);

protected:
	int countLines(string sPath);
	int countDimension(string sPath);
	void formalize(double **arMetaData, uint32_t uiAllNum, uint32_t uiDimension);
	void checkAndDeleteArr(void** p);
	int checkLevel(double * pMateData);
	int checkLevel(double ** arMateData, uint32_t mateDataID);
	Proportion countLevel(double ** pArrMateData, uint32_t datacount);
	Proportion countLevel(double ** pArrMateData, const vector<uint32_t>& ids);
	Proportion countLevel(double ** pArrMateData, const set<uint32_t>& ids);
	char Type2c(int lvl);
	int markSecond();

protected:

	const string strMaxBoundry = "900000 75000 7500 7500 3500 3000 6500 7500 44000 4000 900 800 400 150 155500 10000 1500 500 1000 5000 500 500 500 500 3000 5000 2500 4000 8000 2000 2000 800 800 5000 800 8420 400 3800 580 800 300 500 300 300 2500 500 110 300 800 20000 230 250 500 1500 130 800 400 40 240 2000 4000 2500 1000 107100 30000 39 30000 10000 8 9000 80000 5 500 400000 5 4500 9400 5000 0.01 1000 4000 3000 1000 500 400 1e+07 7e+06 30000 2 1000 40900 3e+06 1500 90 65 25000 900 90 8 1200 600 3000";
	int * arMaxBoundry = 0; 

	double **arMetaVal = 0;
	uint32_t **arLsh = 0;

	uint32_t uiLshL = 250;
	double dLshW = 0;

	C2Lsh c2lsh;
	EncIndex encIndex;
	const string masterKey = "AbcdefgdfklajdflkajsdlfaS";
	const int shmKey = 0x1000;

};