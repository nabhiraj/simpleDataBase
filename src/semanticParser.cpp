#include"global.h"

bool semanticParse(){
    logger.log("semanticParse");
    switch(parsedQuery.queryType){
        case CLEAR: return semanticParseCLEAR();
        case CROSS: return semanticParseCROSS();
        case DISTINCT: return semanticParseDISTINCT();
        case EXPORT: return semanticParseEXPORT();
        case INDEX: return semanticParseINDEX();
        case JOIN: return semanticParseJOIN();
        case LIST: return semanticParseLIST();
        case LOAD: return semanticParseLOAD();
        case PRINT: return semanticParsePRINT();
        case PROJECTION: return semanticParsePROJECTION();
        case RENAME: return semanticParseRENAME();
        case SELECTION: return semanticParseSELECTION();
        case SORT: return semanticParseSORT();
        case SOURCE: return semanticParseSOURCE();
        case LOAD_MATRIX: return semanticParseLOADMATRIX();//added by me.
        case INSERT: return semanticParseINSERT();//added by me.
        case BULK_INSERT: return semanticParseBULK_INSERT();//added by me.
        case TRANSPOSE: return semanticParseTRANSPOSEMATRIX();
        case GROUP: return semanticParseGroupBy();//added by me.
        default: cout<<"SEMANTIC ERROR"<<endl;
    }

    return false;
}