#include "matrix_cursor.h"
class Matrix;
extern float BLOCK_SIZE;
extern BufferManager bufferManager;
MatrixCursor::MatrixCursor(string name){
    this->MatrixName=name;
    block_stored=-1;
    this->matrixBlockLimit=BLOCK_SIZE*1024/sizeof(int);
    this->temp_buffer.resize(this->matrixBlockLimit);
    vector<int> arr(1);
    for(int i=0;i<this->matrixBlockLimit;i++){
        this->temp_buffer[i]=arr;
    }
    this->num_rows=0;
}

int MatrixCursor::getblockid(int index){
    return index/matrixBlockLimit;
}

int MatrixCursor::getLocalrowid(int index){
    return index%matrixBlockLimit;
}

void MatrixCursor::flushTempBuffer(){
    bufferManager.writePage(this->MatrixName,this->block_stored,this->temp_buffer,this->num_rows);
    this->block_stored=-1;
    this->num_rows=0;
}


//this requires exccessive checking.
void MatrixCursor::changebuffer(int blockid){
    if(blockid==this->block_stored){
        return;
    }else{
        if(this->block_stored!=-1&&this->num_rows>0)
            flushTempBuffer();
        this->block_stored=blockid;
        string file_name="../data/temp/"+this->MatrixName+"_Page" + to_string(this->block_stored);
        string s;
        ifstream fin(file_name, ios::in);
        int row_counter=0;
        while(getline(fin,s)){
            int temp=stoi(s);
            this->temp_buffer[row_counter][0]=temp;
            row_counter++;
        }
        this->num_rows=row_counter;
        fin.close();
    }
}

int MatrixCursor::getItem(int index){
    int blockid=getblockid(index);
    int localRowid=getLocalrowid(index);
    if(this->block_stored!=blockid){
        changebuffer(blockid);
    }
    return this->temp_buffer[localRowid][0];
}

void MatrixCursor::setItem(int index,int value){
    int blockid=getblockid(index);
    int localRowid=getLocalrowid(index);
    if(this->block_stored!=blockid){
        changebuffer(blockid);
    }
    this->temp_buffer[localRowid][0]=value;
}