# Similarity Join Demonstration

 * Publications
 * Requirements
 * Initialization
 * Configuration
 * Setup
 * Make
 * Dataset
 * Example

# PUBLICATION

Xingliang Yuan, Xinyu Wang, Cong Wang, Chenyun Yu, and Sarana Nutanong, “Privacy-preserving Similarity Joins Over Encrypted Data”, IEEE Transactions on Information Forensics and Security (TIFS), Accepted, 2017.

LSH Algorithm and Implementation (E2LSH) (http://www.mit.edu/~andoni/LSH/)

# REQUIREMENTS

Recommended environment: Ubuntu 14.04 LTS with gcc version 4.8.4.

This software requires the following libraries:

 * OpenSSL (https://www.openssl.org/source/openssl-1.0.2a.tar.gz)
 * boost C++ library (http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz)
 * Thrift (http://archive.apache.org/dist/thrift/0.9.2/)

# INSTALLATION

Environment setup:

```shell
apt-get update
apt-get install gcc g++ openssl libssl-dev libboost-dev libevent-dev flex bison pkg-config git make cmake automake autoconf 
```

Thrift installation:

You can find the latest HTTP link on https://thrift.apache.org/

```shell
 * wget http://archive.apache.org/dist/thrift/0.9.2/thrift-0.9.2.tar.gz
 * tar zxvf 
 * cd 
 * make
 * make install
```

# CONFIGURATION
 
 * Configure Share Memery

The program support storage hash in share memery. 

Need modify the max share memery size of system setting.
 
```shell
sysctl -w kernel.shmmax=8589934592
echo "kernel.shmmax=8589934592" >> /etc/sysctl.conf
```

# MAKE

Compile Similarity Join demo

```shell
cd SimilarityJoin
make
```
# Dataset

We use two dataset to test similarity join.

covtype.data
MiniBooNE_PID.txt

covtype.data have 10 dimension and 581012 records. 
MiniBooNE_PID.txt have 50 dimension and 130064 records.

# EXAMPLE
#### Command List

Running program, the main menu like this.
```shell

              Controller Command List

  =====================================================
  ||
  ||  Input Command Below :
  ||
  =====================================================
  ||
  || 0 Exit
  || 1 Process Data, compute the informations, dump to files. 
  || 2 Load the Processed Data and informations from files.
  =====================================================
  ||
  || 3 [ Part 1 ] Build index time cost
  || 4 [ Part 2 ] Per token process time
  || 5 [ Part 3 ] Strategy I Latency CDF
  || 6 [ Part 3 ] Strategy II Latency CDF
  || 7 [ Part 4, 5 ] Strategy I Join Time
  || 8 [ Part 4, 5 ] Strategy II Join Time
  || 9 [ Part 6 ] Strategy I, II Join Accuracy
  || a [ Part 6 ] Strategy III Join Accuracy
  || b [ Part 4, 5 ] Strategy III Join Time
  || c [ Part 7 ] Strategy III Security Distribution
  || 5 
  || 6 
  || 7 
  || 8 
  ||
  =====================================================
  ||
  || A Test create index
  || B Attach created index
  || C Test single query
  || D Test Index
  =====================================================
```
#### LoadData

We need load data to program at first.
Input ‘1’ and provide some params.
```shell
Please input the Path of the dataset [ Then Press Enter ]
MiniBooNE_PID.txt
Please input the Dimension of the dataset. [ Then Press Enter ]
50
Begin to Load data from [ MiniBooNE_PID.txt ]
The dimension is [ 50 ]
Begin to Work
The dataset contains : [ 130064 ] records
Finish Read data
Press 1 to modify params. Press others use defaults params
 e.g.  D = 10   L = 300   W = 0.3
1
Input your dimension . [ Then Press Enter ]
50
Input your L . [ Then Press Enter ]
300
Input your W . [ Then Press Enter ]
30
Dimension = 50
L = 300
W = 30
Formalize data.
The Percent => 1%
...
The Percent => 100%
Compute LSH cost time : 490318
Press 1 to save data to files. [ overwrite if exists ]
2
Press [Enter] to continue...
```
#### BuildIndex

Then, we need to build the encrypted index.
Input ‘A’ and provide some params.
```shell

Process will create encrypted index.
Press a share memory key. if input 0 then the process will malloc memory to store the index. 

100
Process will use share memory to store the index. The Key is 100
All the dataset has 581012 records data.
Please input the number you want to build into the index.

10000
Build Index => 1%
...
Build Index => 100%
Build Index cost time : 27054456
uiDistinctCounter = 300
uiAllCounterSum = 3000000
uiMaxCounter = 10000
The Bhash Has 2998974 Elements And Load : 
 ***    The Floor : 0  Real Data : 178571 All Place : 178571  Data Percent : 1
 ***    The Floor : 1  Real Data : 178567 All Place : 178567  Data Percent : 1
 ***    The Floor : 2  Real Data : 178561 All Place : 178561  Data Percent : 1
 ***    The Floor : 3  Real Data : 178559 All Place : 178559  Data Percent : 1
 ***    The Floor : 4  Real Data : 178536 All Place : 178537  Data Percent : 0.999994
 ***    The Floor : 5  Real Data : 178531 All Place : 178531  Data Percent : 1
 ***    The Floor : 6  Real Data : 178511 All Place : 178513  Data Percent : 0.999989
 ***    The Floor : 7  Real Data : 178490 All Place : 178501  Data Percent : 0.999938
 ***    The Floor : 8  Real Data : 178467 All Place : 178489  Data Percent : 0.999877
 ***    The Floor : 9  Real Data : 178425 All Place : 178487  Data Percent : 0.999653
 ***    The Floor : 10  Real Data : 178299 All Place : 178481  Data Percent : 0.99898
 ***    The Floor : 11  Real Data : 177925 All Place : 178469  Data Percent : 0.996952
 ***    The Floor : 12  Real Data : 177025 All Place : 178447  Data Percent : 0.992031
 ***    The Floor : 13  Real Data : 174543 All Place : 178441  Data Percent : 0.978155
 ***    The Floor : 14  Real Data : 167899 All Place : 178439  Data Percent : 0.940932
 ***    The Floor : 15  Real Data : 151885 All Place : 178417  Data Percent : 0.851292
 ***    The Floor : 16  Real Data : 115589 All Place : 178403  Data Percent : 0.64791
 ***    The Floor : 17  Real Data : 58405 All Place : 178397  Data Percent : 0.327388
 ***    The Floor : 18  Real Data : 11774 All Place : 178393  Data Percent : 0.0660003
 ***    The Floor : 19  Real Data : 412 All Place : 178361  Data Percent : 0.00230992
 ***    The Floor : 20  Real Data : 0 All Place : 178351  Data Percent : 0
 ***    The Floor : 21  Real Data : 0 All Place : 178349  Data Percent : 0
 ***    The Floor : 22  Real Data : 0 All Place : 178333  Data Percent : 0
 ***    The Floor : 23  Real Data : 0 All Place : 178327  Data Percent : 0
The Bhash Has Real Elements : 2998974
Press [Enter] to continue...
```
We can see each floor status in the index.
#### Strategy I Time
Input ‘7’ and provide some params to test the time of join by Strategy I. 
```shell
please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)
0.0001
The number need to join is 58
the time is seconds.
Input the Threshold K to get the list.
50
508
ulNeedBandwidthNum = 580000
Press [Enter] to continue...
```
We use top 58 records and join to 10000 records in the index.
In Strategy I, it used 508 seconds.
#### Strategy II Time
Input ‘8’ and provide some params to test the time of join by Strategy II. 
```shell
please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)
0.0001
Input the Threshold K to get the list.
50
The number need to join is 58
the time is seconds.
28
ulNeedBandwidthNum = 1323070
uiHitCache = 17100
ulSaveResultNum = 75414990
Press [Enter] to continue...
```
We use top 58 records and join to 10000 records in the index.
In Strategy II, it used 28 seconds.
#### Strategy III Time
Input ‘b’ and provide some params to test the time of join by Strategy III. 
```shell
please input your self join percent. e.g. 0.1 (10%)  0.2 (20%)
0.0001
The number need to join is 58
Input your self-query R.
0.1
Input your security Threshold K.
30
Self query set = 3
uiDistinctJoinQueryLsh = 300
uiJoinQueryMaxLshCounter = 58
uiJoinQuerySumLshCounter = 17400
The number need to join is 3
the time is seconds.
Input the Threshold K to get the list.
50
11
ulNeedBandwidthNum = 11061
Press [Enter] to continue...
```
We use top 58 records and join to 10000 records in the index.
In Strategy III, it used 11 seconds.