#include "global.h"
//the syntax goes as follow..
//<new_table> <- GROUP BY <grouping_attribute> FROM <table_name> RETURN MAX|MIN|SUM|AVG(<attribute>)
//the example is as follow.
//T1 <- GROUP BY A FROM R RETURN MAX(C)
//0  1   2     3  4  5   6  7      8
//will learn a lot from the select operation.
bool isCorrectOperation(string str){
    str=str.substr(0,3);
    if(str=="SUM"||str=="AVG"||str=="MAX"||str=="MIN"){
        return true;
    }else{
        return false;
    }
}
string extractOperationColoumn(string str){
    return str.substr(4,str.length()-5);
}
bool syntacticParseGroupBy(){
    //now we have to parse the syntax of group by function.
    if(!(tokenizedQuery[3]=="BY" && tokenizedQuery[5]=="FROM"&& tokenizedQuery[7]=="RETURN")){
        cout<<"syntactic error "<<endl;
        return false;
    }
    if(!isCorrectOperation(tokenizedQuery[8])){
        cout<<"syntactic error "<<endl;
        return false;
    }
    parsedQuery.queryType=GROUP;
    parsedQuery.operation_coloumn=extractOperationColoumn(tokenizedQuery[8]);
    parsedQuery.grouping_coloumn=tokenizedQuery[4];
    parsedQuery.operation_name=tokenizedQuery[8].substr(0,3);
    parsedQuery.group_table=tokenizedQuery[6];
    parsedQuery.selectionResultRelationName=tokenizedQuery[0];
    return true;
}

//checking the existance of the table and the two columns whicha are in the query are real or not.
bool semanticParseGroupBy(){
    //cout<<"doing semantic analysisi"<<endl;
    //cout<<"group table is "<<parsedQuery.group_table<<endl;
    //cout<<"operation name is "<<parsedQuery.operation_name<<endl;
    //cout<<"grouping coloumn is "<<parsedQuery.grouping_coloumn<<endl;
    //cout<<"opearation will is performentd in the coloumn is "<<parsedQuery.operation_coloumn<<endl;
    //now is the semantic okey.. checking..
    //table should exist both the coloumn in table should exist.
    Table* table=NULL;
    if(tableCatalogue.isTable(parsedQuery.group_table)){
        table=tableCatalogue.getTable(parsedQuery.group_table);
    }else{
        cout<<"semantic error table does not exist"<<endl;
        return false;
    }
    if(table->isColumn(parsedQuery.grouping_coloumn)&&table->isColumn(parsedQuery.operation_coloumn)){
        return true;
    }else{
        return false;
    }
}
void executeGroupBy(){
    //now we have to execute the command.
    //select will be of the help.
    Table* table=tableCatalogue.getTable(parsedQuery.group_table);
    vector<string> col_names(2);
    col_names[0]=parsedQuery.grouping_coloumn;
    col_names[1]=parsedQuery.operation_coloumn;
    Table* resultantTable = new Table(parsedQuery.selectionResultRelationName,col_names);
    table->groupBy(table,resultantTable,parsedQuery.grouping_coloumn,parsedQuery.operation_coloumn,parsedQuery.operation_name);
    cout<<"all things are done ending the operation"<<endl;
    return;
}