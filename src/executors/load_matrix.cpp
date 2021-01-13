#include "global.h"
bool syntacticParseLOADMATRIX()
{
    logger.log("syntacticParseLOADMATRIX");
    parsedQuery.queryType = LOAD_MATRIX;
    parsedQuery.loadMatrixRelationName = tokenizedQuery[2];
    return true;
}

bool semanticParseLOADMATRIX()
{
    logger.log("semanticParseLOADMATRIX");
    if (!isFileExists(parsedQuery.loadMatrixRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    if(Matrix::getMatrix(parsedQuery.loadMatrixRelationName)==NULL){
        return true;
    }
    return true;
    
}

void executeLOADMATRIX()
{
    Matrix* mymatrix=new Matrix(parsedQuery.loadMatrixRelationName);
    Matrix::setMatrix(parsedQuery.loadMatrixRelationName,mymatrix);
    mymatrix->load();
}