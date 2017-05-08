#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdint.h>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <iomanip>

//getch
#include <termios.h>
#include <unistd.h>


#include "../Caravel/BukHash.h"
#include "../Caravel/ShmCtl.h"
#include "../Caravel/C2Lsh.h"
//time compute
#include "../Caravel/TimeDiff.h"


#include "../Caravel/EncIndex.h"

#define DEF_INDEX_LOAD 0.7D
#define DEF_INDEX_FLOOR 24
#define DEF_MASTER_KEY "abcdefgdfklajdflkajsdlfas"
//Config
#define DEF_MAXSIZE 150000

using namespace std;
using namespace caravel;

int getch() {
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

int countLines(string sPath)
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
	return iLines;
}

template<typename T>
T PrintAndGet(char *szMsg)
{
	T tRet;
	cout << szMsg << endl << endl;
	cin >> tRet;
	return tRet;
}

void PrintCmdList() {
	system("clear");
	cout << endl << endl << "            ITF_SimilarityJoin Command List" << endl << endl;
	cout << "  =====================================================" << endl << "  ||" << endl;
	cout << "  ||  Input Command Below :" << endl << "  ||" << endl;
	cout << "  =====================================================" << endl << "  ||" << endl;
	cout << "  || 0 Exit" << endl;
	cout << "  || 1 Process Data, compute the informations, dump to files. " << endl;
	cout << "  || 2 Load the Processed Data and informations from files." << endl;
	cout << "  =====================================================" << endl << "  ||" << endl;
	cout << "  || 3  Build index time cost" << endl;
	cout << "  || 4  Per token process time" << endl;
	cout << "  || 5  Strategy I Latency CDF" << endl;
	cout << "  || 6  Strategy II Latency CDF" << endl;
	cout << "  || 7¡ïStrategy I Join Time" << endl;
	cout << "  || 8¡ïStrategy II Join Time" << endl;
	cout << "  || 9  Strategy I, II Join Accuracy" << endl;
	cout << "  || a  Strategy III Join Accuracy" << endl;
	cout << "  || b¡ïStrategy III Join Time" << endl;
	cout << "  || c  Strategy III Security Distribution" << endl;


	cout << "  ||  " << endl;
	cout << "  ||  " << endl;



	cout << "  =====================================================" << endl << "  ||" << endl;
	cout << "  || A Test create index" << endl;
	cout << "  || B Attach created index" << endl;
	cout << "  || C Test single query" << endl;
	cout << "  || D Test Index" << endl;

	cout << "  =====================================================" << endl << endl;
	return;
}

uint32_t count_l2_small_r(double **arMetaData, uint32_t uiAllNum, uint32_t uiDimension, uint32_t uiQueryID, double dR)
{
	uint32_t uiQueryInR = 0;
	//Compute pointer number in dR
	for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
	{
		if (uiCur < 10)
		{
			//cout << "Distance L2 = " << C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[uiCur], uiDimension) << endl;
		}
		if (C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[uiCur], uiDimension) < dR)
		{
			uiQueryInR++;
		}
	}
	return uiQueryInR;

}

void formalize(double **arMetaData, uint32_t uiAllNum, uint32_t uiDimension)
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

double precise(uint32_t uiTopK, uint32_t uiQueryID, vector<uint32_t> vecRetResult,
	double **arMetaData, uint32_t uiAllNum, uint32_t uiDimension)
{
	//Real distance
	map<double, vector<uint32_t> > mapRealDistance;
	for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
	{
		mapRealDistance[C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[uiCur], uiDimension)].push_back(uiCur);
	}
	vector<uint32_t> vecRealRank;
	for (map<double, vector<uint32_t> >::iterator it = mapRealDistance.begin(); it != mapRealDistance.end(); it++)
	{
		for (vector<uint32_t>::iterator itt = it->second.begin(); itt != it->second.end(); itt++)
		{
			vecRealRank.push_back(*itt);
		}
	}
	//Query distance
	map<double, vector<uint32_t> > mapQueryDistance;
	for (vector<uint32_t>::iterator it = vecRetResult.begin(); it != vecRetResult.end(); it++)
	{
		mapQueryDistance[C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[*it], uiDimension)].push_back(*it);
	}
	vector<uint32_t> vecQueryRank;
	for (map<double, vector<uint32_t> >::iterator it = mapQueryDistance.begin(); it != mapQueryDistance.end(); it++)
	{
		for (vector<uint32_t>::iterator itt = it->second.begin(); itt != it->second.end(); itt++)
		{
			vecQueryRank.push_back(*itt);
		}
	}

	//compute precise
	double dPrecise;
	uint32_t uiSelf = 0;
	for (uint32_t uiCur = 0; uiCur < uiTopK; uiCur++)
	{
		if (C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[vecQueryRank[uiCur]], uiDimension) == 0)
		{
			uiSelf++;
			continue;
		}
		//cout << vecQueryRank[uiCur] << "   " << vecRealRank[uiCur] << endl;
		//cout << "XXX:" << C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[vecRealRank[uiCur]], uiDimension) / C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[vecQueryRank[uiCur]], uiDimension) << endl;
		dPrecise += (C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[vecRealRank[uiCur]], uiDimension)
			/ C2Lsh::ComputeL2(arMetaData[uiQueryID], arMetaData[vecQueryRank[uiCur]], uiDimension));
	}
	//uiTopK remove 1 because of himself.
	dPrecise /= (uiTopK - uiSelf);
	return dPrecise;
}

bool _CheckAndAddSecurityMap(uint32_t *arLsh, uint32_t uiLshL, map<uint64_t, uint32_t> *pMap, uint32_t uiSecK)
{
	uint64_t ulKey;
	uint32_t *pKey = (uint32_t*)&ulKey;
	for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
	{
		*pKey = arLsh[uiL];
		*(pKey + 1) = uiL;
		if ((*pMap)[ulKey] + 1 >= uiSecK)
		{
			//can't use it for security reason.
			return false;
		}
	}
	//Can use it security
	//So add 1 to security counter
	for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
	{
		*pKey = arLsh[uiL];
		*(pKey + 1) = uiL;
		(*pMap)[ulKey]++;
	}
	return true;
}

void SelfQuery(double **arMetaData, uint32_t uiAllNum, uint32_t uiDimension,
	uint32_t **arAllLsh, uint32_t uiLshL, double dSelfQueryR,
	vector<uint32_t> vecQuery, vector<uint32_t> &vecRet, uint32_t uiSecurityK)
{
	//Security map
	map<uint64_t, uint32_t> mapSecurityMap;
	uint32_t uiFailCondition = 1000;

	while (vecQuery.size() != 0)
	{
		uint32_t uiOffset = rand() % vecQuery.size();
		uint32_t uiQuerySample = vecQuery[uiOffset];
		bool bUseIt = _CheckAndAddSecurityMap(arAllLsh[uiQuerySample], uiLshL, &mapSecurityMap, uiSecurityK);
		if (bUseIt == false)
		{
			if (0 == uiFailCondition--)
			{
				cout << "Error for uiFailCondition < 0" << endl;
				break;
			}
			continue;
		}
		//delete it from vecQuery
		vecQuery.erase(vecQuery.begin() + uiOffset);
		//Add the sample point to result vector
		vecRet.push_back(uiQuerySample);
		//query for others
		for (vector<uint32_t>::iterator it = vecQuery.begin(); it != vecQuery.end();)
		{
			if (C2Lsh::ComputeL2(arMetaData[uiQuerySample], arMetaData[*it], uiDimension) < dSelfQueryR)
			{
				it = vecQuery.erase(it);
				if (it == vecQuery.end())
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
}


int checkLevel(double* pMateData)
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

int checkLevel(double** arMateData, uint32_t mateDataID)
{
	return checkLevel(arMateData[mateDataID]);
}

struct Proportion
{
	uint32_t total = 0;
	uint32_t counter[4] = { 0 };
};

Proportion countLevel(double** pArrMateData, uint32_t datacount)
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
Proportion countLevel(double** pArrMateData, const vector<uint32_t>& ids)
{
	Proportion res;
	res.total = ids.size();
	for (int i = 0; i < ids.size(); i++)
	{
		if (ids[i] >= DEF_MAXSIZE)
		{
			continue;
		}
		int lvl = checkLevel(pArrMateData, ids[i]);
		res.counter[lvl]++;
	}
	return res;
}
Proportion countLevel(double** pArrMateData, const set<uint32_t>& ids)
{
	Proportion res;
	res.total = ids.size();
	for (auto i = ids.begin(); i != ids.end(); i++)
	{
		if (*i >= DEF_MAXSIZE)
		{
			continue;
		}
		int lvl = checkLevel(pArrMateData, *i);
		res.counter[lvl]++;
	}
	return res;
}

ostream& operator << (ostream& out, const Proportion& por)
{
	cout.setf(ios::right);
	cout << "Total " << setw(6) << por.total << endl;
	cout << "Type A" << setw(6) << por.counter[0] << "  , percentage is " << setw(12) << 100 * (por.counter[0] + .0) / por.total << "%" << endl;
	cout << "Type B" << setw(6) << por.counter[1] << "  , percentage is " << setw(12) << 100 * (por.counter[1] + .0) / por.total << "%" << endl;
	cout << "Type C" << setw(6) << por.counter[2] << "  , percentage is " << setw(12) << 100 * (por.counter[2] + .0) / por.total << "%" << endl;
	cout << "Type D" << setw(6) << por.counter[3] << "  , percentage is " << setw(12) << 100 * (por.counter[3] + .0) / por.total << "%" << endl;
	cout.unsetf(ios::right);
	return out;
}

Proportion& operator +=(Proportion& self, const Proportion& por)
{
	self.total += por.total;
	for (int i = 0; i < 4; i++)
	{
		self.counter[i] += por.counter[i];
	}
}

//"Please pick a subset for join. Input the type to filter the victims. 1 means A, \
2 means B, 3 means C, 4 means D, other means mix"
bool filterType(double* pMateData, uint32_t chooseType)
{
	if (chooseType > 0 && chooseType < 5)
	{
		int lvl = checkLevel(pMateData);
		if (chooseType == lvl + 1)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	return true;
}

void printVectorHeadId(vector<uint32_t> vec, int count, int perLineCount = 10)
{
	int i;
	for (i = 0; i < count&&i < vec.size(); i++)
	{
		cout << vec[i] << " ";
		if (i != 0 && i%perLineCount == 0)
			cout << endl;
	}
	if (i - 1 % perLineCount != 0)
		cout << endl;
}

void printSetHeadId(set<uint32_t> ids, int count, int perLineCount = 10)
{
	int n = 0;
	for (auto i = ids.begin(); i != ids.end() && n < count; i++, n++)
	{
		cout << *i << " ";
		if (n != 0 && n%perLineCount == 0)
			cout << endl;
	}
	if (n - 1 % perLineCount != 0)
		cout << endl;
}

int main(int argc, char **argv)
{
	srand(0);

	//Save for meta data
	double **arMetaVal;

	//Save for lsh values
	uint32_t **arLsh;

	uint32_t uiAllNum;
	uint32_t uiLshL;
	double dLshW;
	uint32_t uiDataDimension;
	uint32_t uiUseDimension;

	//index setting
	uint32_t uiBuildIndexNum;

	//Enc Index
	EncIndex encIndex;
	encIndex.SetKey(DEF_MASTER_KEY);

	system("clear");
	int iCmd;
	do {
		PrintCmdList();
		iCmd = getch();
		switch (iCmd)
		{
		case 49: // 1 Process Data, compute the informations, dump to files. 
		{
			//Before use the data use sed -i 's/,/\t/g' covtype.data to replace the "," to "\t".

			//1
			string strPath = PrintAndGet<string>("Please input the Path of the dataset [ Then Press Enter ]\n e.g.  financeNormalize.data");

			//uiDataDimension = PrintAndGet<uint32_t>("Please input the Dimension of the dataset. [ Then Press Enter ]\n e.g.  covtype.data is 10 dimension. \n MiniBooNE_PID.txt is 50 dimension.");
			uiDataDimension = 102;

			cout << "Begin to Load data from [ " << strPath.c_str() << " ]" << endl;
			cout << "The dimension is [ " << uiDataDimension << " ]" << endl;



			uiAllNum = countLines(strPath);

			if (uiAllNum == -1)
			{
				cout << "The file did not exists." << endl;
				break;
			}

			cout << "The dataset contains : [ " << uiAllNum << " ] records" << endl;

			cout << "Begin to Work" << endl;
			//Init memory
			arMetaVal = new double*[uiAllNum];

			ifstream ifs;
			ifs.open(strPath, ios::in);

			for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
			{
				arMetaVal[uiCur] = new double[uiDataDimension];
				for (uint32_t uiD = 0; uiD < uiDataDimension; uiD++)
				{
					ifs >> arMetaVal[uiCur][uiD];
				}
			}
			ifs.close();

			cout << "Finish Read data" << endl;

			//string sInput = PrintAndGet<string>("Press 1 to modify params. Press others use defaults params\n e.g.  D = 102   L = 250   W = 1.5");
			string sInput = "2";
			uiUseDimension = 102;
			uiLshL = 250;
			dLshW = 1.5;

			if (sInput == "1")
			{
				uiUseDimension = PrintAndGet<uint32_t>("Input your dimension . [ Then Press Enter ]");
				uiLshL = PrintAndGet<uint32_t>("Input your L . [ Then Press Enter ]");
				dLshW = PrintAndGet<double>("Input your W . [ Then Press Enter ]");
			}

			cout << "Dimension = " << uiUseDimension << endl;
			cout << "L = " << uiLshL << endl;
			cout << "W = " << dLshW << endl;

			cout << "Formalize data." << endl;
			formalize(arMetaVal, uiAllNum, uiUseDimension);


			C2Lsh lsh;
			lsh.Init(uiUseDimension, uiLshL, dLshW);

			arLsh = new uint32_t*[uiAllNum];


			TimeDiff::DiffTimeInMicroSecond();

			for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
			{

				if (uiCur % (uiAllNum / 100) == 0)
				{
					cout << "The Percent => " << uiCur / (uiAllNum / 100) << "%" << endl;
				}

				arLsh[uiCur] = new uint32_t[uiLshL];
				lsh.Compute(arMetaVal[uiCur], arLsh[uiCur]);

			}

			uint32_t uiTimeCost = TimeDiff::DiffTimeInMicroSecond();
			cout << "Compute LSH cost time : " << uiTimeCost << endl;

			//Save the data

			//sInput = PrintAndGet<string>("Press 1 to save data to files. [ overwrite if exists ]");
			sInput = "2";
			if ("1" == sInput)
			{
				cout << "Begin to write data to files" << endl;

				sInput = PrintAndGet<string>("Press a name to save files. [ overwrite if exists ]");

				ofstream myfile;
				myfile.open(sInput, ios::out | ios::trunc);

				//Save the config
				myfile << uiAllNum;
				myfile << " ";
				myfile << uiLshL;
				myfile << " ";
				myfile << dLshW;
				myfile << " ";
				myfile << uiDataDimension;
				myfile << " ";
				myfile << uiUseDimension;
				myfile << " ";

				//first save meta data

				for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
				{
					for (uint32_t uiD = 0; uiD < uiDataDimension; uiD++)
					{
						myfile << arMetaVal[uiCur][uiD];
						myfile << " ";
					}
				}

				//second save LSH data

				for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
				{
					for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
					{
						myfile << arLsh[uiCur][uiL];
						myfile << " ";
					}
				}

				myfile.close();

			}

			cout << "Finish compute all information:" << endl;
			cout << "uiAllNum = " << uiAllNum << endl;
			cout << "uiLshL = " << uiLshL << endl;
			cout << "uiLshW = " << dLshW << endl;
			cout << "uiDataDimension = " << uiDataDimension << endl;
			cout << "uiUseDimension = " << uiUseDimension << endl;

			break;
		}
		case 50: // 2 Load the Processed Data and informations from files.
		{
			//2 Load data

			string sInput = PrintAndGet<string>("Press a name to load files. eg: lsh");

			ifstream myfile;
			myfile.open(sInput.c_str(), ios::in);

			//load the config
			myfile >> uiAllNum;
			myfile >> uiLshL;
			myfile >> dLshW;
			myfile >> uiDataDimension;
			myfile >> uiUseDimension;

			//first load meta data

			arMetaVal = new double*[uiAllNum];

			for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
			{
				arMetaVal[uiCur] = new double[uiDataDimension];
				for (uint32_t uiD = 0; uiD < uiDataDimension; uiD++)
				{
					myfile >> arMetaVal[uiCur][uiD];
				}
			}

			//second load LSH data
			arLsh = new uint32_t*[uiAllNum];
			for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
			{
				arLsh[uiCur] = new uint32_t[uiLshL];
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					myfile >> arLsh[uiCur][uiL];
				}
			}

			myfile.close();

			cout << "Finish Load All Information:" << endl;
			cout << "uiAllNum = " << uiAllNum << endl;
			cout << "uiLshL = " << uiLshL << endl;
			cout << "uiLshW = " << dLshW << endl;
			cout << "uiDataDimension = " << uiDataDimension << endl;
			cout << "uiUseDimension = " << uiUseDimension << endl;

			break;
		}
		case 51: // 3 [ Part 1 ] Build index time cost
		{
			//3 [ Part 1 ] Build Index Time Cost

			cout << "Step1 : Use 1 to process data" << endl;
			cout << "Step2 : Use A to build the index and it will show you the build time." << endl;

			break;
		}
		case 52: // 4 [ Part 2 ] Per token process time
		{
			//4 Per token process time

			cout << "Compute LSH time cost : please use 1 to process data, it will show you the Lsh compute time." << endl;

			cout << "Process will test single query and output the information" << endl;

			uint32_t uiQueryID = PrintAndGet<uint32_t>("Input an ID for query index");

			int lvl = checkLevel(arMetaVal[uiQueryID]);
			cout << "This user Type is " << (char)(lvl + 'A') << endl;

			uint32_t *arQueryLsh = arLsh[uiQueryID];

			char **arszCombine = new char *[uiLshL];
			char **arszTD = new char*[uiLshL];
			char **arszMask = new char*[uiLshL];
			for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
			{
				arszCombine[uiL] = new char[SHA256_DIGEST_LENGTH];
				arszTD[uiL] = new char[SHA256_DIGEST_LENGTH];
				arszMask[uiL] = new char[SHA256_DIGEST_LENGTH];
			}

			TimeDiff::DiffTimeInMicroSecond();
			for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
			{
				uint32_t uiLsh = arQueryLsh[uiL];

				encIndex.GenerateTrapdoorAndMask(uiLsh, uiL, arszCombine[uiL], SHA256_DIGEST_LENGTH,
					arszTD[uiL], SHA256_DIGEST_LENGTH, arszMask[uiL], SHA256_DIGEST_LENGTH);
			}
			uint32_t uiTimeCost = TimeDiff::DiffTimeInMicroSecond();
			cout << "Token generate time :" << uiTimeCost << endl;

			cout << "Begin to test process time" << endl;

			vector<uint32_t> vecResult;

			TimeDiff::DiffTimeInMicroSecond();
			for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
			{
				uint32_t uiLsh = arQueryLsh[uiL];

				encIndex.Query(arszTD[uiL], SHA256_DIGEST_LENGTH,
					arszMask[uiL], SHA256_DIGEST_LENGTH, vecResult);
			}

			uiTimeCost = TimeDiff::DiffTimeInMicroSecond();
			cout << "Process token cost time :" << uiTimeCost << endl;

			//NEW
			cout << "Process get vecResult size is " << vecResult.size() << endl;
			set<uint32_t> setResultInJoin;
			setResultInJoin.insert(vecResult.begin(), vecResult.end());
			cout << "Head of this result is ";
			printVectorHeadId(vecResult, 20);
			Proportion por = countLevel(arMetaVal, setResultInJoin);
			cout << "In the test resultSet. " << por << endl;
			break;
		}
		case 53: // 5 [ Part 3 ] Strategy I Latency CDF
		{
			//5 [ Part 3 ] Strategy I Latency CDF

			uint32_t uiTestNum = PrintAndGet<uint32_t>("Input the max ID of test sample. The test will begin from 0 to what you input.");

			cout << "Every request time cost is as follow" << endl;

			uint32_t uiLimitK = PrintAndGet<uint32_t>("Input the Threshold K to get the list.");

			Proportion por = countLevel(arMetaVal, uiTestNum);
			cout << "In the test sample. " << por << endl;
			uint32_t uiLimitLevel = PrintAndGet<uint32_t>("Please pick a subset for join. Input the type to filter the victims. 1 means A, \
2 means B, 3 means C, 4 means D, other means mix");

			for (uint32_t uiCur = 0; uiCur < uiTestNum; uiCur++)
			{
				if (!filterType(arMetaVal[uiCur], uiLimitLevel))
				{
					continue;
				}

				TimeDiff::DiffTimeInMicroSecond();

				uint32_t *arQueryLsh = arLsh[uiCur];

				vector<uint32_t> vecResult;
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					uint32_t uiLsh = arQueryLsh[uiL];
					encIndex.QueryOne(uiLsh, uiL, vecResult);

				}

				map<uint32_t, uint32_t> mapCombine;
				for (vector<uint32_t>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
				{
					mapCombine[*it]++;
				}

				vector<uint32_t> vecResultInK;

				for (map<uint32_t, uint32_t>::iterator it = mapCombine.begin(); it != mapCombine.end(); it++)
				{
					if (it->second > uiLimitK)
					{
						vecResultInK.push_back(it->first);
					}
				}

				uint32_t uiTimeCost = TimeDiff::DiffTimeInMicroSecond();
				cout << uiTimeCost << endl;

				//NEW
				cout << "Process get vecResultInK size is " << vecResultInK.size() << endl;
				cout << "Process get mapCombine size is " << mapCombine.size() << endl;
				cout << "Process get vecResult size is " << vecResult.size() << endl;


			}

			break;
		}
		case 54: // 6 [ Part 3 ] Strategy II Latency CDF
		{
			//6 [ Part 3 ] Strategy II Latency CDF
			uint32_t uiTestNum = PrintAndGet<uint32_t>("Input the max ID of test sample. The test will begin from 0 to what you input.");

			cout << "Every request time cost is as follow" << endl;
			map<uint32_t, map<uint32_t, vector<uint32_t> > >mapCacheResult;

			uint32_t uiHitCache = 0;

			uint32_t uiLimitK = PrintAndGet<uint32_t>("Input the Threshold K to get the list.");

			Proportion por = countLevel(arMetaVal, uiTestNum);
			cout << "In the test sample. " << por << endl;
			uint32_t uiLimitLevel = PrintAndGet<uint32_t>("Please pick a subset for join. Input the type to filter the victims. 1 means A, \
2 means B, 3 means C, 4 means D, other means mix");

			for (uint32_t uiCur = 0; uiCur < uiTestNum; uiCur++)
			{
				if (!filterType(arMetaVal[uiCur], uiLimitLevel))
				{
					continue;
				}

				TimeDiff::DiffTimeInMicroSecond();

				uint32_t *arQueryLsh = arLsh[uiCur];

				map<uint32_t, uint32_t> mapCombine;

				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					uint32_t uiLsh = arQueryLsh[uiL];
					vector<uint32_t> *pVecReslt = &mapCacheResult[uiLsh][uiL];
					if (pVecReslt->size() != 0)
					{
						uiHitCache++;
					}
					else
					{
						encIndex.QueryOne(uiLsh, uiL, *pVecReslt);
					}

					for (vector<uint32_t>::iterator it = pVecReslt->begin(); it != pVecReslt->end(); it++)
					{
						mapCombine[*it]++;
					}

				}

				//cout << "Map size : " << mapCombine.size() << endl;

				vector<uint32_t> vecResultInK;

				for (map<uint32_t, uint32_t>::iterator it = mapCombine.begin(); it != mapCombine.end(); it++)
				{
					if (it->second > uiLimitK)
					{
						vecResultInK.push_back(it->first);
					}
				}

				uint32_t uiTimeCost = TimeDiff::DiffTimeInMicroSecond();
				cout << uiTimeCost << endl;
				//NEW
				cout << "Process get vecResultInK size is " << vecResultInK.size() << endl;
				cout << "Process get mapCombine size is " << mapCombine.size() << endl;
			}

			cout << "uiHitCache = " << uiHitCache << endl;


			break;
		}
		case 55: // 7 [ Part 4 ] Strategy I Join Time
		{
			//7 [ Part 4 ] Strategy I Join Time

			//double dPercent = PrintAndGet<double>("please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)");
			//uint32_t uiJoinNum = uiAllNum * dPercent;

			uint32_t uiJoinNum = PrintAndGet<uint32_t>("Please input a number for randomly pick. e.g. 10");

			cout << "The number need to join is " << uiJoinNum << endl;
			uint64_t ulNeedBandwidthNum = 0;

			//uint32_t uiLimitK = PrintAndGet<uint32_t>("Input the Threshold K to get the list.");
			uint32_t uiLimitK = 88;

			set<uint32_t> setResultInJoin;
			Proportion por = countLevel(arMetaVal, uiJoinNum);
			cout << "In query records. " << endl << por << endl;
			uint32_t uiLimitLevel = PrintAndGet<uint32_t>("Please pick a subset for join. Input the type to filter the victims. 1 means A, 2 means B, 3 means C, 4 means D, other means mix");

			TimeDiff::DiffTimeInSecond();
			for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
			{
				if (!filterType(arMetaVal[uiCur], uiLimitLevel))
				{
					continue;
				}

				uint32_t *arQueryLsh = arLsh[uiCur];

				vector<uint32_t> vecResult;
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					uint32_t uiLsh = arQueryLsh[uiL];
					encIndex.QueryOne(uiLsh, uiL, vecResult);

				}

				map<uint32_t, uint32_t> mapCombine;
				for (vector<uint32_t>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
				{
					mapCombine[*it]++;
				}

				vector<uint32_t> vecResultInK;

				for (map<uint32_t, uint32_t>::iterator it = mapCombine.begin(); it != mapCombine.end(); it++)
				{
					if (it->second > uiLimitK)
					{
						vecResultInK.push_back(it->first);
					}
				}

				ulNeedBandwidthNum += vecResultInK.size();

				setResultInJoin.insert(vecResultInK.begin(), vecResultInK.end());

				if (uiJoinNum > 100 && uiCur % (uiJoinNum / 100) == 0)
				{
					cout << "Finish " << uiCur / (uiJoinNum / 100) << " % => " << TimeDiff::DiffTimeInSecond() << endl;
					cout << "Up to now, ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;
				}


			}
			cout << "Query time is " << TimeDiff::DiffTimeInSecond() << " second" << endl;
			cout << "ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;
			//NEW
			por = countLevel(arMetaVal, setResultInJoin);
			cout << "In result records." << endl << por << endl;

			break;
		}
		case 56: // 8 [ Part 4 ] Strategy II Join Time
		{
			//8 [ Part 4 ] Strategy II Join Time

			//double dPercent = PrintAndGet<double>("please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)");
			//uint32_t uiJoinNum = uiAllNum * dPercent;
			uint32_t uiJoinNum = PrintAndGet<uint32_t>("Please input a number for randomly pick. e.g. 10");

			//uint32_t uiLimitK = PrintAndGet<uint32_t>("Input the Threshold K to get the list.");
			uint32_t uiLimitK = 88;

			cout << "The number need to join is " << uiJoinNum << endl;
			uint64_t ulNeedBandwidthNum = 0;

			map<uint32_t, map<uint32_t, vector<uint32_t> > >mapCacheResult;
			uint32_t uiHitCache = 0;
			uint64_t ulSaveResultNum = 0;

			set<uint32_t> setResultInJoin;

			Proportion por = countLevel(arMetaVal, uiJoinNum);
			cout << "In query records. " << endl << por << endl;
			uint32_t uiLimitLevel = PrintAndGet<uint32_t>("Please pick a subset for join. Input the type to filter the victims. 1 means A, 2 means B, 3 means C, 4 means D, other means mix");

			TimeDiff::DiffTimeInSecond();
			for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
			{
				if (!filterType(arMetaVal[uiCur], uiLimitLevel))
				{
					continue;
				}

				uint32_t *arQueryLsh = arLsh[uiCur];
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
						//cout << "pVecReslt->size() = " << pVecReslt->size() << endl;
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

				setResultInJoin.insert(vecResultInK.begin(), vecResultInK.end());

				if (uiJoinNum > 100 && uiCur % (uiJoinNum / 100) == 0)
				{
					cout << "Finish " << uiCur / (uiJoinNum / 100) << " % => " << TimeDiff::DiffTimeInSecond() << endl;
					cout << "Up to now, ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;
				}

			}

			cout << "Query time is " << TimeDiff::DiffTimeInSecond() << " second" << endl;
			cout << "ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;
			cout << "uiHitCache = " << uiHitCache << endl;
			cout << "ulSaveResultNum = " << ulSaveResultNum << endl;

			//NEW
			por = countLevel(arMetaVal, setResultInJoin);
			cout << "In result records." << endl << por << endl;

			break;
		}
		case 57: // 9 [ Part 6 ] Strategy I, II Join Accuracy
		{
			//9 [ Part 6 ] Strategy I, II Join Accuracy
			cout << "Please press C to use single query information. It will show you the accuracy." << endl;

			break;
		}
		case 97: // a [ Part 6 ] Strategy III Join Accuracy
		{
			//a [ Part 6 ] Strategy III Join Accuracy
			double dPercent = PrintAndGet<double>("please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)");
			uint32_t uiJoinNum = uiAllNum * dPercent;
			uint32_t uiQueryID = PrintAndGet<uint32_t>("Input an ID for query index");
			uint32_t uiLimitK = PrintAndGet<uint32_t>("Input the Threshold K to get the list.");
			double dR = PrintAndGet<double>("Input your recall R.");
			double dInnerR = PrintAndGet<double>("Input your self query r.");

			int lvl = checkLevel(arMetaVal[uiQueryID]);
			cout << "This user Type is " << (char)(lvl + 'A') << endl;

			uint32_t *arQueryLsh = arLsh[uiQueryID];

			vector<uint32_t> vecResult;
			for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
			{
				uint32_t uiLsh = arQueryLsh[uiL];
				encIndex.QueryOne(uiLsh, uiL, vecResult);

				//cout << "vecResult.size() = " << vecResult.size() << endl;

			}

			map<uint32_t, uint32_t> mapCombine;
			for (vector<uint32_t>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
			{
				mapCombine[*it]++;
			}
			//means that how many records that query back
			cout << "mapCombine.size() = " << mapCombine.size() << endl;
			//means that the count of QueryID in one time query result
			cout << "Self query " << uiQueryID << " = " << mapCombine[uiQueryID] << endl;
			set<uint32_t> setResultInJoin;
			setResultInJoin.insert(vecResult.begin(), vecResult.end());
			Proportion por = countLevel(arMetaVal, setResultInJoin);
			cout << "In the test resultSet. " << por << endl;

			vector<uint32_t> vecQueryResult;

			for (map<uint32_t, uint32_t>::iterator it = mapCombine.begin(); it != mapCombine.end(); it++)
			{
				if (it->second > uiLimitK)
				{
					vecQueryResult.push_back(it->first);
				}
			}

			cout << "Get the result in threshold = " << vecQueryResult.size() << endl;
			uint32_t uiRealInR = count_l2_small_r(arMetaVal, uiBuildIndexNum, uiUseDimension, uiQueryID, dR);
			cout << "Get the count < R = " << uiRealInR << endl;

			//begin to self query on arMetaVal
			cout << "Begin to self query." << endl;
			vector<uint32_t> vecSelfQuery;
			for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
			{
				if (C2Lsh::ComputeL2(arMetaVal[uiQueryID], arMetaVal[uiCur], uiDataDimension) < dInnerR)
				{
					vecSelfQuery.push_back(uiCur);
				}
			}

			cout << "vecSelfQuery.size() = " << vecSelfQuery.size() << endl;
			setResultInJoin.clear();
			setResultInJoin.insert(vecSelfQuery.begin(), vecSelfQuery.end());
			por = countLevel(arMetaVal, setResultInJoin);
			cout << "In the test selfQuery <r. " << por << endl;

			double dSumRecall = 0;
			double dSumPrecise = 0;
			Proportion sumProportion;
			for (vector<uint32_t>::iterator it = vecSelfQuery.begin(); it != vecSelfQuery.end(); it++)
			{
				vector<uint32_t> vecSelfQueryToR;
				for (uint32_t uiCur = 0; uiCur < uiAllNum; uiCur++)
				{
					if (C2Lsh::ComputeL2(arMetaVal[*it], arMetaVal[uiCur], uiDataDimension) < dR)
					{
						vecSelfQueryToR.push_back(uiCur);
					}
				}

				vector<uint32_t> vecQueryResultInR;
				for (vector<uint32_t>::iterator itv = vecQueryResult.begin(); itv != vecQueryResult.end(); itv++)
				{
					if (C2Lsh::ComputeL2(arMetaVal[*itv], arMetaVal[*it], uiUseDimension) < dR)
					{
						vecQueryResultInR.push_back(*itv);
					}
				}

				setResultInJoin.clear();
				setResultInJoin.insert(vecQueryResultInR.begin(), vecQueryResultInR.end());
				por = countLevel(arMetaVal, setResultInJoin);
				cout << "In the test resultSet < R. " << por << endl;
				sumProportion += por;

				cout << "ID = " << *it << "   The Recall = " << (double)vecQueryResultInR.size() / (double)vecSelfQueryToR.size() << endl;
				dSumRecall += ((double)vecQueryResultInR.size() / (double)vecSelfQueryToR.size());

				double dPrecise = precise(vecQueryResult.size(), *it, vecQueryResult, arMetaVal, uiAllNum, uiUseDimension);
				cout << " ID = " << *it << "  The Precise = " << dPrecise << endl;
				dSumPrecise += dPrecise;

			}

			cout << "dSumRecall" << dSumRecall << endl;
			cout << "SelfQuery Num" << vecSelfQuery.size() << endl;
			cout << "dSumPrecise" << dSumPrecise << endl;
			cout << "For all result " << sumProportion << endl;

			cout << "AvgRecall = " << dSumRecall / vecSelfQuery.size() << endl;

			cout << "AvgPrecise = " << dSumPrecise / vecSelfQuery.size() << endl;

			cout << "AvgTypeRecall = " << sumProportion.counter[lvl] / (double)sumProportion.total << endl;

			break;
		}
		case 98: // b [ Part 4, 5 ] Strategy III Join Time
		{
			//b [ Part 4, 5 ] Strategy III Join Time

			//SelfQuery
			//double dPercent = PrintAndGet<double>("please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)");
			//uint32_t uiJoinNum = uiAllNum * dPercent;
			uint32_t uiJoinNum = PrintAndGet<uint32_t>("Please input a number for randomly pick. e.g. 10");

			cout << "The number need to join is " << uiJoinNum << endl;

			Proportion por = countLevel(arMetaVal, uiJoinNum);
			cout << "In query records. " << endl << por << endl;
			uint32_t uiLimitLevel = PrintAndGet<uint32_t>("Please pick a subset for join. Input the type to filter the victims. 1 means A, 2 means B, 3 means C, 4 means D, other means mix");


			//Init Join vector
			vector<uint32_t> vecJoin;
			for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
			{
				vecJoin.push_back(uiCur);
			}
			double dSelfQueryR = PrintAndGet<double>("Input your self-query R.");
			vector<uint32_t> vecSelfQuery;
			//uint32_t uiSecurityK = PrintAndGet<uint32_t>("Input your security Threshold K.");
			uint32_t uiSecurityK = 999;
			SelfQuery(arMetaVal, uiAllNum, uiUseDimension, arLsh, uiLshL, dSelfQueryR, vecJoin, vecSelfQuery, uiSecurityK);
			cout << "Self query set = " << vecSelfQuery.size() << endl;

			//map<uint64_t, uint32_t> mapJoinQueryLsh;
			//uint64_t ulKey;
			//uint32_t *pKey = (uint32_t*)&ulKey;
			//for (vector<uint32_t>::iterator it = vecJoin.begin(); it != vecJoin.end(); it++)
			//{
			//    for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
			//    {
			//        *pKey = arLsh[*it][uiL];
			//        *(pKey + 1) = uiL;
			//        mapJoinQueryLsh[ulKey]++;
			//    }
			//}
			////collect information
			//uint32_t uiDistinctJoinQueryLsh = 0;
			//uint32_t uiJoinQueryMaxLshCounter = 0;
			//uint32_t uiJoinQuerySumLshCounter = 0;
			//for (map<uint64_t, uint32_t>::iterator it = mapJoinQueryLsh.begin(); it != mapJoinQueryLsh.end(); it++)
			//{
			//    uiDistinctJoinQueryLsh++;
			//    uiJoinQuerySumLshCounter += it->second;
			//    uiJoinQueryMaxLshCounter = uiJoinQueryMaxLshCounter > it->second ? uiJoinQueryMaxLshCounter : it->second;
			//}
			//cout << "uiDistinctJoinQueryLsh = " << uiDistinctJoinQueryLsh << endl;
			//cout << "uiJoinQueryMaxLshCounter = " << uiJoinQueryMaxLshCounter << endl;
			//cout << "uiJoinQuerySumLshCounter = " << uiJoinQuerySumLshCounter << endl;

			//Begin to join.

			cout << "The number need to join is " << vecSelfQuery.size() << endl;

			uint64_t ulNeedBandwidthNum = 0;

			//uint32_t uiLimitK = PrintAndGet<uint32_t>("Input the Threshold K to get the list.");
			uint32_t uiLimitK = 88;
			set<uint32_t> setResultInJoin;

			por = countLevel(arMetaVal, vecSelfQuery);
			cout << "In self query records. " << endl << por << endl;

			TimeDiff::DiffTimeInSecond();
			for (uint32_t uiCur = 0; uiCur < vecSelfQuery.size(); uiCur++)
			{
				if (!filterType(arMetaVal[vecSelfQuery[uiCur]], uiLimitLevel))
				{
					continue;
				}

				uint32_t *arQueryLsh = arLsh[vecSelfQuery[uiCur]];

				vector<uint32_t> vecResult;
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					uint32_t uiLsh = arQueryLsh[uiL];
					encIndex.QueryOne(uiLsh, uiL, vecResult);

				}

				map<uint32_t, uint32_t> mapCombine;
				for (vector<uint32_t>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
				{
					mapCombine[*it]++;
				}

				vector<uint32_t> vecResultInK;

				for (map<uint32_t, uint32_t>::iterator it = mapCombine.begin(); it != mapCombine.end(); it++)
				{
					if (it->second > uiLimitK)
					{
						vecResultInK.push_back(it->first);
					}
				}

				ulNeedBandwidthNum += vecResultInK.size();

				setResultInJoin.insert(vecResultInK.begin(), vecResultInK.end());

				if (vecSelfQuery.size() > 100 && uiCur % (vecSelfQuery.size() / 100) == 0)
				{
					cout << "Finish " << uiCur / (vecSelfQuery.size() / 100) << " % => " << TimeDiff::DiffTimeInSecond() << endl;
					cout << "Up to now, ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;
				}


			}
			cout << TimeDiff::DiffTimeInSecond() << endl;
			cout << "ulNeedBandwidthNum = " << ulNeedBandwidthNum << endl;

			//NEW
			por = countLevel(arMetaVal, setResultInJoin);
			cout << "In result records. " << por << endl;


			break;
		}
		case 99: // c [ Part 7 ] Strategy III Security Distribution
		{
			//c [ Part 7 ] Strategy III Security Distribution
			//SelfQuery
			double dPercent = PrintAndGet<double>("please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)");
			uint32_t uiJoinNum = uiAllNum * dPercent;
			cout << "All the Join data is " << uiJoinNum << endl;

			//Init Join vector
			vector<uint32_t> vecJoin;
			for (uint32_t uiCur = 0; uiCur < uiJoinNum; uiCur++)
			{
				vecJoin.push_back(uiCur);
			}
			double dSelfQueryR = PrintAndGet<double>("Input your self-query R.");
			vector<uint32_t> vecSelfQuery;
			uint32_t uiSecurityK = PrintAndGet<uint32_t>("Input your security Threshold K.");
			SelfQuery(arMetaVal, uiAllNum, uiUseDimension, arLsh, uiLshL, dSelfQueryR, vecJoin, vecSelfQuery, uiSecurityK);
			cout << "Self query set = " << vecSelfQuery.size() << endl;

			/* For check Self-Query

			for (uint32_t uiCur = 0; uiCur < vecSelfQuery.size(); uiCur++)
			{
				for (vector<uint32_t>::iterator it = vecSelfQuery.begin(); it != vecSelfQuery.end(); it++)
				{
					if (C2Lsh::ComputeL2(arMetaVal[vecSelfQuery[uiCur]], arMetaVal[*it], uiDataDimension) < dSelfQueryR)
					{
						cout << vecSelfQuery[uiCur] <<"   VS   "<<*it<<"  Distance : "<< C2Lsh::ComputeL2(arMetaVal[vecSelfQuery[uiCur]], arMetaVal[*it], uiDataDimension) <<" < "<<dSelfQueryR<< endl;
					}
				}
			}

			*/

			map<uint64_t, uint32_t> mapJoinQueryLsh;
			uint64_t ulKey;
			uint32_t *pKey = (uint32_t*)&ulKey;
			for (vector<uint32_t>::iterator it = vecJoin.begin(); it != vecJoin.end(); it++)
			{
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					*pKey = arLsh[*it][uiL];
					*(pKey + 1) = uiL;
					mapJoinQueryLsh[ulKey]++;
				}
			}
			//collect information
			uint32_t uiDistinctJoinQueryLsh = 0;
			uint32_t uiJoinQueryMaxLshCounter = 0;
			uint32_t uiJoinQuerySumLshCounter = 0;
			for (map<uint64_t, uint32_t>::iterator it = mapJoinQueryLsh.begin(); it != mapJoinQueryLsh.end(); it++)
			{
				uiDistinctJoinQueryLsh++;
				uiJoinQuerySumLshCounter += it->second;
				uiJoinQueryMaxLshCounter = uiJoinQueryMaxLshCounter > it->second ? uiJoinQueryMaxLshCounter : it->second;
			}
			cout << "uiDistinctJoinQueryLsh = " << uiDistinctJoinQueryLsh << endl;
			cout << "uiJoinQueryMaxLshCounter = " << uiJoinQueryMaxLshCounter << endl;
			cout << "uiJoinQuerySumLshCounter = " << uiJoinQuerySumLshCounter << endl;

			//vecSelfQuery and vecAnotherSelfQuery combine and distinct
			map<uint64_t, uint32_t> mapForCombine;
			for (vector<uint32_t>::iterator it = vecJoin.begin(); it != vecJoin.end(); it++)
			{
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					*pKey = arLsh[*it][uiL];
					*(pKey + 1) = uiL;
					mapForCombine[ulKey]++;
				}
			}
			for (vector<uint32_t>::iterator it = vecSelfQuery.begin(); it != vecSelfQuery.end(); it++)
			{
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					*pKey = arLsh[*it][uiL];
					*(pKey + 1) = uiL;
					mapForCombine[ulKey]++;
				}
			}
			//Init SelfQuery LSH Map
			map<uint64_t, uint32_t> mapSelfQueryLsh;
			for (vector<uint32_t>::iterator it = vecSelfQuery.begin(); it != vecSelfQuery.end(); it++)
			{
				for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
				{
					*pKey = arLsh[*it][uiL];
					*(pKey + 1) = uiL;
					mapSelfQueryLsh[ulKey]++;
				}
			}

			//After Combine
			cout << "Begin to output data:" << endl;
			vector<uint32_t> vecJoinLshCount;
			vector<uint32_t> vecSelfQueryLshCount;
			for (map<uint64_t, uint32_t>::iterator it = mapForCombine.begin(); it != mapForCombine.end(); it++)
			{
				vecJoinLshCount.push_back(mapJoinQueryLsh[it->first]);
				vecSelfQueryLshCount.push_back(mapSelfQueryLsh[it->first]);
			}
			//Compute sum
			uint32_t uiJoinLshSum = 0;
			uint32_t uiSelfQueryLshSum = 0;
			for (vector<uint32_t>::iterator it = vecJoinLshCount.begin(); it != vecJoinLshCount.end(); it++)
			{
				uiJoinLshSum += *it;
			}
			for (vector<uint32_t>::iterator it = vecSelfQueryLshCount.begin(); it != vecSelfQueryLshCount.end(); it++)
			{
				uiSelfQueryLshSum += *it;
			}
			//print percent
			cout << "Begin to print first one." << endl;

			cout << "vecJoinLSH_Count SelfQueryLSH_Count" << endl;
			for (uint32_t uiCur = 0; uiCur < vecJoinLshCount.size(); uiCur++)
			{
				uint32_t uiTmp = ((double)(vecJoinLshCount[uiCur]) / (double)uiJoinLshSum) * 100000000;
				cout << uiTmp << "\t";
				uiTmp = ((double)(vecSelfQueryLshCount[uiCur]) / (double)uiSelfQueryLshSum) * 100000000;
				cout << uiTmp << endl;

			}
			/*

			for (map<uint64_t, uint32_t>::iterator it = mapSelfQueryLsh.begin(); it != mapSelfQueryLsh.end(); it++)
			{
				cout << it->second << endl;
			}


			*/


			break;
		}
		case 65: // A Test Create Index
		{
			//A Test Create Index
			cout << "Process will create encrypted index." << endl;

			//key_t key = PrintAndGet<key_t>("Press a share memory key. if input 0 then the process will malloc memory to store the index. ");
			key_t key = 0;
			if (0 == key)
			{
				cout << "Process will malloc memory to store the index." << endl;
			}
			else
			{
				cout << "Process will use share memory to store the index. The Key is " << key << endl;
			}

			cout << "All the dataset has " << uiAllNum << " records data." << endl;
			uiBuildIndexNum = PrintAndGet<uint32_t>("Please input the number you want to build into the index.");

			Proportion por = countLevel(arMetaVal, uiBuildIndexNum);
			cout << "In index records. " << endl << por << endl;

			encIndex.Init(uiBuildIndexNum * uiLshL, DEF_INDEX_LOAD, DEF_INDEX_FLOOR, key);

			TimeDiff::DiffTimeInMicroSecond();

			encIndex.BuildIndex(arLsh, uiBuildIndexNum, uiLshL);

			uint32_t uiTimeCost = TimeDiff::DiffTimeInMicroSecond();
			cout << "Build Index cost time : " << uiTimeCost << endl;

			//map<uint64_t, uint32_t> *pMapCounter = encIndex.GetCounterMap();
			//uint32_t uiAllCounterSum = 0;
			//uint32_t uiDistinctCounter = 0;
			//uint32_t uiMaxCounter = 0;
			//for (map<uint64_t, uint32_t>::iterator it = pMapCounter->begin(); it != pMapCounter->end(); it++)
			//{
			//    uiDistinctCounter++;
			//    uiAllCounterSum += it->second;
			//    uiMaxCounter = uiMaxCounter > it->second ? uiMaxCounter : it->second;
			//}
			//cout << "uiDistinctCounter = " << uiDistinctCounter << endl;
			//cout << "uiAllCounterSum = " << uiAllCounterSum << endl;
			//cout << "uiMaxCounter = " << uiMaxCounter << endl;

			encIndex.ShowBukHashState();

			break;
		}
		case 66: // B Attach Created Index
		{
			//B Attach Created Index

			cout << "Process will attach an created encrypted index." << endl;

			key_t key = PrintAndGet<key_t>("Press an existed share memory key. Process will attach on the index on share memory. eg:100");

			cout << "All the dataset has " << uiAllNum << " records data." << endl;
			//uiBuildIndexNum = PrintAndGet<uint32_t>("Please input the number of the index items.");
			uiBuildIndexNum = uiAllNum;

			cout << "Attach the index on share memory whose key is " << key << endl;

			encIndex.AttachIndex(uiBuildIndexNum * uiLshL, DEF_INDEX_LOAD, DEF_INDEX_FLOOR, key);

			encIndex.ShowBukHashState();

			break;

		}
		case 67: // C Test single query
		{
			//C Test single query

			cout << "Process will test single query and output the information" << endl;

			uint32_t uiQueryID = PrintAndGet<uint32_t>("Input an ID for query index");

			int lvl = checkLevel(arMetaVal[uiQueryID]);
			cout << "This user Type is " << (char)(lvl + 'A') << endl;

			uint32_t *arQueryLsh = arLsh[uiQueryID];

			vector<uint32_t> vecResult;
			for (uint32_t uiL = 0; uiL < uiLshL; uiL++)
			{
				uint32_t uiLsh = arQueryLsh[uiL];
				encIndex.QueryOne(uiLsh, uiL, vecResult);

				//cout << "vecResult.size() = " << vecResult.size() << endl;

			}

			map<uint32_t, uint32_t> mapCombine;
			for (vector<uint32_t>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
			{
				mapCombine[*it]++;
			}
			cout << "mapCombine.size() = " << mapCombine.size() << endl;
			cout << "Self query " << uiQueryID << " = " << mapCombine[uiQueryID] << endl;

			uint32_t uiLimitK = PrintAndGet<uint32_t>("Input the Threshold K to get the list.");
			double dR = PrintAndGet<double>("Input a R to count number.");

			vector<uint32_t> vecQueryResult;
			vector<uint32_t> vecQueryResultInR;

			for (map<uint32_t, uint32_t>::iterator it = mapCombine.begin(); it != mapCombine.end(); it++)
			{
				if (it->second > uiLimitK)
				{
					vecQueryResult.push_back(it->first);
					if (C2Lsh::ComputeL2(arMetaVal[it->first], arMetaVal[uiQueryID], uiUseDimension) < dR)
					{
						vecQueryResultInR.push_back(it->first);
					}
				}
			}

			cout << "Get the result in threshold = " << vecQueryResult.size() << endl;
			cout << "Get the result in threshold and distance < dR = " << vecQueryResultInR.size() << endl;
			uint32_t uiRealInR = count_l2_small_r(arMetaVal, uiBuildIndexNum, uiUseDimension, uiQueryID, dR);
			cout << "Whole dataset has the count < R = " << uiRealInR << endl;
			cout << "The Recall is " << vecQueryResultInR.size() / (double)uiRealInR << endl;
			set<uint32_t> setResultInJoin;
			setResultInJoin.insert(vecQueryResultInR.begin(), vecQueryResultInR.end());
			cout << "Process get setResultInJoin size is " << setResultInJoin.size() << endl;
			cout << "Head of this result is ";
			printSetHeadId(setResultInJoin, 20);
			Proportion por = countLevel(arMetaVal, setResultInJoin);
			cout << "In the test resultSet. " << por << endl;
			//Compute precise
			while (true)
			{
				uint32_t uiPreciseCmd = PrintAndGet<uint32_t>("Press 1 to test precise again.");
				if (uiPreciseCmd != 1)
				{
					break;
				}

				cout << "Begin to compute precise." << endl;
				uint32_t uiTopK = PrintAndGet<uint32_t>("Please input your TopK to compute precise.");
				double dPrecise = precise(uiTopK, uiQueryID, vecQueryResult, arMetaVal, uiAllNum, uiUseDimension);
				cout << "The precise is " << dPrecise << endl;

			}



			break;
		}
		case 68: // D Test Index
		{
			EncIndex encTestIndex;

			encTestIndex.SetKey(DEF_MASTER_KEY);
			encTestIndex.Init(10000, DEF_INDEX_LOAD, DEF_INDEX_FLOOR, 0);
			uint32_t uiKey = PrintAndGet<uint32_t>("Press a uiLsh for test");
			uint32_t uiVal = PrintAndGet<uint32_t>("Press a uiVal for the uiLsh");

			encTestIndex.TestEncIndex(uiKey, uiVal);
			encTestIndex.ShowBukHashState();


			break;
		}
		default:
		{
			continue;
			break;
		}
		};

		cout << "Press [Enter] to continue..." << endl;
		getch();
		getch();

	} while (48 != iCmd);

	cout << "Good Bye !" << endl;

	return 0;
}
