#include "global.h"
bool syntacticParseTANSPOSEMATRIX()
{
    logger.log("syntacticParseTRANSPOSEMATRIX");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType =TRANSPOSE;
    parsedQuery.transposeRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParseTRANSPOSEMATRIX()
{
    logger.log("semanticParseTRANSPOSEMATRIX");
    if(Matrix::getMatrix(parsedQuery.transposeRelationName)!=NULL){
        return true;
    }
    cout<<"semantic error"<<endl;
    return false;
}

void executeTRANSPOSEMATRIX()
{
    Matrix* mymatrix=Matrix::getMatrix(parsedQuery.transposeRelationName);
    mymatrix->Transpose();
}