#include "linearHash.h"
LinearHash::LinearHash(){
        this->bucketLimit=1024;
        this->numberOfBuckets=2;
        this->primary_hashM=2;
        this->secondry_hashM=this->primary_hashM*2;
        this->divisionIndex=0;
        vector<pair<int,int>> empty_vector;
        for(int i=0;i<this->numberOfBuckets;i++){
            this->bucket.push_back(empty_vector);
            this->bucketEntryCount.push_back(0);
        }
    }

LinearHash::LinearHash(int numBuckets,int bucketLimit){
    this->bucketLimit=bucketLimit;
    this->numberOfBuckets=numBuckets;
    this->primary_hashM=numberOfBuckets;
    this->secondry_hashM=2*this->primary_hashM;
    this->divisionIndex=0;
    for(int i=0;i<this->numberOfBuckets;i++){
        this->bucket.push_back(vector<pair<int,int>>());
        this->bucketEntryCount.push_back(0);
    }
}
   
void LinearHash::balance(){
    vector<pair<int,int>> tempBucket;
    for(int i=0;i<bucketEntryCount[divisionIndex];i++){
        tempBucket.push_back(bucket[divisionIndex][i]);
    }
    bucket[divisionIndex].clear();
    bucketEntryCount[divisionIndex]=0;
    bucket.push_back(vector<pair<int,int>>());
    bucketEntryCount.push_back(0);
    for(int i=0;i<tempBucket.size();i++){
        int bucketNumber=tempBucket[i].first%secondry_hashM;
        bucket[bucketNumber].push_back(tempBucket[i]);
        bucketEntryCount[bucketNumber]++;
    }
    divisionIndex++;
    if(divisionIndex==primary_hashM){
        divisionIndex=0;
        primary_hashM=secondry_hashM;
        secondry_hashM=2*primary_hashM;
    }
}


void LinearHash::insert(int key,int value){
    pair<int,int> indexTuple;
    indexTuple.first=key;
    indexTuple.second=value;
    int targetBucketNumber=key%primary_hashM;
    if(targetBucketNumber<divisionIndex){
        targetBucketNumber=key%secondry_hashM;
    }
    bucket[targetBucketNumber].push_back(indexTuple);
    bucketEntryCount[targetBucketNumber]++;
    if(bucketEntryCount[targetBucketNumber]>bucketLimit){
        balance();
    }
}


vector<int> LinearHash::search(int key){
    int targetBucketNumber=key%primary_hashM;
    if(targetBucketNumber<divisionIndex){
        targetBucketNumber=key%secondry_hashM;
    }
    vector<int> res;
    for(int i=0;i<bucketEntryCount[targetBucketNumber];i++){
        if(bucket[targetBucketNumber][i].first==key){
            //return bucket[targetBucketNumber][i].second;
            res.push_back(bucket[targetBucketNumber][i].second);
        }
    }
    return res;
}