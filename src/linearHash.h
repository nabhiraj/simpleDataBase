#include<bits/stdc++.h>
using namespace std;
class LinearHash{
    public:
    //data memebers.
    int primary_hashM;
    int secondry_hashM;
    int numberOfBuckets;
    int divisionIndex;
    int bucketLimit;
    vector<vector<pair<int,int>>> bucket;
    vector<int> bucketEntryCount;
    //methods
    LinearHash();
    LinearHash(int numBuckets,int bucketLimit);
    void balance();
    void insert(int key,int value);
    vector<int> search(int key);//returns the list of row ids.
};