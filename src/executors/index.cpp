#include "global.h"
/**
 * @brief 
 * SYNTAX: INDEX ON column_name FROM relation_name USING indexing_strategy
 * indexing_strategy: ASC | DESC | NOTHING
 */
bool isnum(string& s){
    int n=s.length();
    for(int i=0;i<n;i++){
        if(s[i]>'9'||s[i]<'0'){
            return false;
        }
    }
    return true;
}
bool syntacticParseINDEX()
{
    logger.log("syntacticParseINDEX");
    if(!(tokenizedQuery.size()==7||tokenizedQuery.size()==9)){
        cout<<"syntax error"<<endl;
    }
    if (tokenizedQuery[1] != "ON" || tokenizedQuery[3] != "FROM" || tokenizedQuery[5] != "USING")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    if(tokenizedQuery.size()==9){
        if(!((tokenizedQuery[7]=="FANOUT"&&tokenizedQuery[6]=="BTREE")||(tokenizedQuery[7]=="BUCKETS"&&tokenizedQuery[6]=="HASH"))){
            cout<<"syntax error"<<endl;
        }
        if(!isnum(tokenizedQuery[8])){
            cout<<"syntax error";
        }
        parsedQuery.fan_bucket=stoi(tokenizedQuery[8]);
    }
    parsedQuery.queryType = INDEX;
    parsedQuery.indexColumnName = tokenizedQuery[2];
    parsedQuery.indexRelationName = tokenizedQuery[4];
    string indexingStrategy = tokenizedQuery[6];
    if (indexingStrategy == "BTREE")
        parsedQuery.indexingStrategy = BTREE;
    else if (indexingStrategy == "HASH")
        parsedQuery.indexingStrategy = HASH;
    else if (indexingStrategy == "NOTHING")
        parsedQuery.indexingStrategy = NOTHING;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseINDEX()
{
    logger.log("semanticParseINDEX");
    if (!tableCatalogue.isTable(parsedQuery.indexRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    if (!tableCatalogue.isColumnFromTable(parsedQuery.indexColumnName, parsedQuery.indexRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    Table* table = tableCatalogue.getTable(parsedQuery.indexRelationName);
    if(table->indexed){
        cout << "SEMANTIC ERROR: Table already indexed" << endl;
        return false;
    }
    return true;
}

void executeINDEX()
{
    logger.log("executeINDEX");
    Table* table=tableCatalogue.getTable(parsedQuery.indexRelationName);
    if(tokenizedQuery.size()==9){
        cout<<"bucket or fan count is given "<<endl;
        cout<<"the number of inisial buckets are "<<parsedQuery.fan_bucket<<endl;
        //table->createIndex()
        table->createIndex(parsedQuery.indexColumnName,parsedQuery.indexingStrategy,parsedQuery.fan_bucket);
    }else{
        table->createIndex(parsedQuery.indexColumnName,parsedQuery.indexingStrategy,2);
    }
    cout<<"created the index"<<endl;
    return;
}