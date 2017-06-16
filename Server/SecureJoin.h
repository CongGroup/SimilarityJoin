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
	//A struct to describe type distribution
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

	// load user mate data
	uint32_t loadData(string mataDataPaht, uint32_t maxSize = UINT32_MAX);
	// compute users c2lsh value of whole index
	bool computeLSH(uint32_t L, double w);
	// compute one user lsh from specific user mate data
	bool computeLsh(uint32_t* lsh, double* mateData);
	// build index for specific size
	bool buildIndex(uint32_t size);
	// get user mate data by user id
	string getMataDataByID(uint32_t id);
	// get user type by user id
	string getTypeByID(uint32_t id);
	// get user type by user mate data
	string getTypeByData(double* oneMa0teData);
	// get a group of users type distribution
	Proportion getDistributedByID(vector<uint32_t> ids);
	// get user type distribution of whole index
	Proportion getDistributedofIndex();
	// execute join operation between a group user mate data and index data in strategy 1
	vector<int> joinByStrategy1(double** joinMataData, int num, int ThresholdK, int timeout = INT32_MAX);
	// execute join operation between a group user mate data and index data in strategy 2
	vector<int> joinByStrategy2(double** joinMataData, int num, int ThresholdK, int timeout = INT32_MAX);
	// execute join operation between a group user mate data and index data in strategy 3
	vector<int> joinByStrategy3(double** joinMataData, int num, int ThresholdK, double selfQueryR, int timeout = INT32_MAX);

	//the count of user mate file
	uint32_t uiAllNum = 0;
	//the count of user to build index
	uint32_t uiUserNum = 0;
	//the count of index capacity
	uint32_t indexSize = 0;
	//the count of the index(share) memory size
	uint32_t indexMomery = 0;

	//Some Timer 
	int64_t perQueryTimerSelfQuery = 0;
	int64_t perQueryTimerComputeLSH = 0;
	int64_t perQueryTimerComputeToken = 0;
	int64_t perQueryTimerGetData = 0;
	int64_t perQueryTimerFilter = 0;
	int64_t perQueryTimerQuery = 0;


	//user mate dimension
	uint32_t uiDataDimension = 0;
	//user data dimension of finance dataset
	const uint32_t uiFinanceDimension = 102;

	//user type distribution of whole index
	Proportion indexDistributed;

	//split string by specific str c
	int splitString(const string& s, vector<string>& v, const string& c, bool notNull = true);
	//normalize user mate data
	void normalize(double *arMetaData, uint32_t uiDimension);
	//compute user mate data from user normalize data
	void renormalize(double *arMetaData, uint32_t uiDimension);

protected:
	//count lines of specific file
	int countLines(string sPath);
	//count dimension of specific file
	int countDimension(string sPath);
	//normalize users mate data
	void formalize(double **arMetaData, uint32_t uiAllNum, uint32_t uiDimension);
	//safe delete a pointer to array
	void checkAndDeleteArr(void** p);
	//check one user type by mate data
	int checkLevel(double * pMateData);
	//check one user type by id
	int checkLevel(double ** arMateData, uint32_t mateDataID);
	// get top users type distribution of the index 
	Proportion countLevel(double ** pArrMateData, uint32_t datacount);
	// get users type distribution of specific user ids in vector container
	Proportion countLevel(double ** pArrMateData, const vector<uint32_t>& ids);
	// get users type distribution of specific user ids in set container
	Proportion countLevel(double ** pArrMateData, const set<uint32_t>& ids);
	//convert type from int to char
	char Type2c(int lvl);
	//make a time stamp
	uint32_t markSecond(int id = 0);
	uint32_t markMicroSecond(int id = 0);

protected:

	// the max boundry of finance dataset
	const string strMaxBoundry = "900000 75000 7500 7500 3500 3000 6500 7500 44000 4000 900 800 400 150 155500 10000 1500 500 1000 5000 500 500 500 500 3000 5000 2500 4000 8000 2000 2000 800 800 5000 800 8420 400 3800 580 800 300 500 300 300 2500 500 110 300 800 20000 230 250 500 1500 130 800 400 40 240 2000 4000 2500 1000 107100 30000 39 30000 10000 8 9000 80000 5 500 400000 5 4500 9400 5000 0.01 1000 4000 3000 1000 500 400 1e+07 7e+06 30000 2 1000 40900 3e+06 1500 90 65 25000 900 90 8 1200 600 3000";
	// the max boundry of finance dataset
	double * arMaxBoundry = 0;

	// all user mate data
	double **arMetaVal = 0;
	// all user lsh value
	uint32_t **arLsh = 0;

	// the value of parameter L
	uint32_t uiLshL = 250;
	// the value of parameter W
	double dLshW = 0;

	// c2lsh object
	C2Lsh c2lsh;
	// index object
	EncIndex encIndex;
	// default master key
	const string masterKey = "AbcdefgdfklajdflkajsdlfaS";
	// default share momery key
	const int shmKey = 0x1000;

};