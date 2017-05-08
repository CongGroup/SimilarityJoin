#pragma once
#include <string>
#include <vector>
class SimilarityJoin
{
	/**
	* Load data set and compute LSH.
	*
	* @param fileName    The path of the dataset.
	* @param dimension   The dimension of the dataset.
	* @param L           c2lsh param L.
	* @param w           c2lsh param w.
	*
	* @return ERROR_NONE on success and a corresponding error code on failure
	*         (see errors.h for the full list of possible error codes)
	*/
	virtual int loadData(std::string fileName, int dimension, int L, int w) = 0;

	/**
	* Build index after load dataset.
	*
	* @param size		 The size of index.
	* @param shmKey      Share memory key, zero means do not use share memory.
	*
	* @return ERROR_NONE on success and a corresponding error code on failure
	*         (see errors.h for the full list of possible error codes)
	*/
	virtual int buildIndex(int size, int shmKey) = 0;

	/**
	* Execute join operation by Strategy I.
	*
	* @param queryLSH      The c2lsh value of dataset A.
	* @param joinedID      The id joined by A in dataset B.
	* @param thresholdK    Collision threshold.
	*
	* @return ERROR_NONE on success and a corresponding error code on failure
	*         (see errors.h for the full list of possible error codes)
	*/
	virtual int joinByStrategyI(const std::vector<const std::vector<int>> queryLSH, std::vector<int>& joinedID, int thresholdK) = 0;
	
	/**
	* Execute join operation by Strategy II.
	*
	* @param queryLSH      The c2lsh value of dataset A.
	* @param joinedID      The id joined by A in dataset B.
	* @param thresholdK    Collision threshold.
	*
	* @return ERROR_NONE on success and a corresponding error code on failure
	*         (see errors.h for the full list of possible error codes)
	*/
	virtual int joinByStrategyII(const std::vector<const std::vector<int>> queryLSH, std::vector<int>& joinedID, int thresholdK) = 0;

	/**
	* Execute join operation by Strategy III.
	*
	* @param queryLSH			  The c2lsh value of dataset A.
	* @param joinedID			  The id joined by A in dataset B.
	* @param selfQueryR           Param R in selfQuery.
	* @param securityThreshold    Minimal threshold for security reason.
	* @param thresholdK			  Collision threshold.
	*
	* @return ERROR_NONE on success and a corresponding error code on failure
	*         (see errors.h for the full list of possible error codes)
	*/
	virtual int joinByStrategyIII(const std::vector<const std::vector<int>> queryLSH, std::vector<int>& joinedID,int selfQueryR, int securityThreshold, int thresholdK) = 0;
	
	/**
	* Save encrypted index to specific file in the cloud.
	*
	* @param fileName  The path of the file.
	*
	* @return ERROR_NONE on success and a corresponding error code on failure
	*         (see errors.h for the full list of possible error codes)
	*/
	virtual int saveIndex(std::string fileName) = 0;

	/**
	* Load encrypted index from specific file in the cloud.
	*
	* @param fileName  The path of the file.
	*
	* @return ERROR_NONE on success and a corresponding error code on failure
	*         (see errors.h for the full list of possible error codes)
	*/
	virtual int loadIndex(std::string fileName) = 0;

};