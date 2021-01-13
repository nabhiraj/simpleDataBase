#include "global.h"
bool isnumber(string& s){
    int n=s.length();
    for(int i=0;i<n;i++){
        if(!(s[i]>='0'&&s[i]<='9')){
            return false;
        }
    }
    return true;
}
bool syntacticParseINSERT(){
    if(tokenizedQuery.size()>=5&&tokenizedQuery[1]=="INTO"&&tokenizedQuery[3]=="VALUES"){
        parsedQuery.insertRelationName=tokenizedQuery[2];
        //now populating the values vector to be inserted.
        parsedQuery.insertTupe.clear();
        for(int i=4;i<tokenizedQuery.size();i++){
            if(isnumber(tokenizedQuery[i])){
                parsedQuery.insertTupe.push_back(stoi(tokenizedQuery[i]));
            }else{
                cout<<"syntax error in insert command"<<endl;
                return false;
            }
        }
    }else{
        cout<<"syntax error in insert command"<<endl;
        return false;
    }
    parsedQuery.queryType =INSERT;
    return true;
}

//checking the existance of the table and number of column in the table.
bool semanticParseINSERT(){
    //now checking is the table exist.
    //number of collumn in the table is same as the size of the array we have formed.
    if(!tableCatalogue.isTable(parsedQuery.insertRelationName)){
        cout<<"semantic error the table does not exit"<<endl;
        return false;
    }
    if(tableCatalogue.getTable(parsedQuery.insertRelationName)->columnCount!=parsedQuery.insertTupe.size()){
        cout<<"semantic error collumn count and number of values given are not same"<<endl;
        return false;
    }
    return true;
}
void executeINSERT()
{
    Table* table=tableCatalogue.getTable(parsedQuery.insertRelationName);
    cout<<"applying insert command"<<endl;
    table->insert(parsedQuery.insertTupe);
    cout<<"done with insertion"<<endl;
    return;
}