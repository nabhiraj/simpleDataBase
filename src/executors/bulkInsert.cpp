#include "global.h"
//BULK_INSERT <csv_file_name> INTO <table_name>
bool syntacticParseBULK_INSERT(){
    if(tokenizedQuery.size()!=4||tokenizedQuery[2]!="INTO"||tokenizedQuery[0]!="BULK_INSERT"){
        cout<<"syntax error"<<endl;
        return false;
    }
    parsedQuery.bulkInsertRelationName=tokenizedQuery[3];//this table should exist
    parsedQuery.bulkInsertRelationFileName=tokenizedQuery[1];//this file should exist
    parsedQuery.bulkInsertFilePath="../data/"+parsedQuery.bulkInsertRelationFileName+".csv";
    //entry in the above file should be consistent.
    parsedQuery.queryType=BULK_INSERT;
    //cout<<"syntax analysis done "<<endl;
    cout<<parsedQuery.bulkInsertRelationName<<endl;
    cout<<parsedQuery.bulkInsertRelationFileName<<endl;
    cout<<parsedQuery.bulkInsertFilePath<<endl;
    return true;
}

//checking the existance of the table and the existance of the file.
bool semanticParseBULK_INSERT(){
    if(!tableCatalogue.isTable(parsedQuery.bulkInsertRelationName)){
        cout<<"semantic error"<<endl;
        return false;
    }

    ifstream fin(parsedQuery.bulkInsertFilePath);
    if(!fin.good()){
        cout<<"semantic error file is not good"<<endl;
        return false;
    }
    return true;
}

//calling the bulk insert method in the table class.
void executeBULK_INSERT(){
    Table* t=tableCatalogue.getTable(parsedQuery.bulkInsertRelationName);
    t->bulkInsert(parsedQuery.bulkInsertFilePath);
    cout<<"bulk insertioin done"<<endl;
    return;
}