#include"matrix.h"
//#include"linearHash.h"
/**
 * @brief The cursor is an important component of the system. To read from a
 * table, you need to initialize a cursor. The cursor reads rows from a page one
 * at a time.
 *
 */
//this enum is added by me (nabhiraj)
/*enum Index_Type{
    NONE,
    LINEAR_HASHING
};*/
class Cursor{
    public:
    Page page;
    int pageIndex;//points towards the block (nabhiraj)
    string tableName;
    int pagePointer;//points towards the row in that block (nabhiraj)
    //Index_Type indexType;//(nabhiraj)
    //LinearHash* lh;
    //IndexingStrategy indexTypeUsed;
    public:
    Cursor(string tableName, int pageIndex);
    vector<int> getNext();
    void nextPage(int pageIndex);
    //void createCursor(string colname,Index_Type indextype);//(nabhiraj)
};