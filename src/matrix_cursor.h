#include "bufferManager.h"
using namespace std;
class MatrixCursor{
    public:
    int block_stored;
    int matrixBlockLimit;
    string MatrixName;
    vector<vector<int>> temp_buffer;
    int num_rows;
    MatrixCursor(string name);//done
    int getblockid(int index);//done
    int getLocalrowid(int index);//done
    int getItem(int index);//done
    void setItem(int index,int value);//done
    void changebuffer(int blockid);//done
    void flushTempBuffer();//done.
};
