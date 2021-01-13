#include "matrix.h"
#include<bits/stdc++.h>
extern float BLOCK_SIZE;
extern BufferManager bufferManager;
map<string,Matrix*> Matrix::matrixCatlog;

     Matrix* Matrix::getMatrix(string mat_name){
        if(matrixCatlog.find(mat_name)!=matrixCatlog.end()){
            return matrixCatlog[mat_name];
        }else{
            return NULL;
        }
    }
    void Matrix::setMatrix(string mat_name,Matrix* mat){
        if(matrixCatlog.find(mat_name)==matrixCatlog.end()){
            matrixCatlog[mat_name]=mat;
        }else{
            cout<<"this is already set"<<endl;
        }
    }
    //sets the source file path and counts the number of row in a matrix.
    Matrix::Matrix(string matrix_name){
        this->MatrixName=matrix_name;
        this->sourceFileName="../data/" + matrix_name + ".csv";
        this->stable_sourceFileName="../data/temp/" + matrix_name;
        this->num_rows=0;
        fstream fin(this->sourceFileName, ios::in);
        string line;
        if(getline(fin,line)){
            int n=line.length();
            int count=0;
            for(int i=0;i<n;i++){
                if(line[i]==','){
                    count++;
                }
            }
            this->num_cols=count+1;
        }
        this->max_block_limit=BLOCK_SIZE*1024/sizeof(int);
    }

    bool Matrix::load(){
        fstream fin(this->sourceFileName,ios::in);
        string line,word;
        vector<vector<int>> temp_arr(this->max_block_limit,vector<int>(1));
        int fill_num=0;
        int block_id=0;
        //getline(fin, line);
        
        while (getline(fin, line)){
            stringstream s(line);
            for (int columnCounter = 0; columnCounter < this->num_cols; columnCounter++){
                if (!getline(s, word, ',')){
                    return false;
                }
                //now the word cointains the intiger.
                temp_arr[fill_num][0]=stoi(word);
                fill_num++;
                if(fill_num>=this->max_block_limit){
                    //we need to save in stable storage
                    bufferManager.writePage(this->MatrixName,block_id,temp_arr,fill_num);
                    fill_num=0;
                    block_id++;
                }
            }
            this->num_rows++;
        }
        if(fill_num>0){
            //we need to save in stable storage.
            bufferManager.writePage(this->MatrixName,block_id,temp_arr,fill_num);
            fill_num=0;
            block_id++;
        }
        return true;
    }

    void reset(bool *visited,int size)
    {
        for(int i=0;i<size;i++)
        {
            visited[i]=false;
        }
    }

    void Matrix::Transpose(){
        MatrixCursor* mc=new MatrixCursor(this->MatrixName);
        //cout<<mc->getItem(11)<<endl;
        int size = this->num_rows*this->num_cols - 1; 
        int t; 
        int next;  
        int cycle;  
        int i;  
        bool *visited=new bool[size+1];
        reset(visited,size+1); 
        visited[0] = visited[size] = true; 
        i = 1;  
        while (i < size) 
        { 
            cycle = i; 
            t = mc->getItem(i); 
            do
            { 
                next = (i*this->num_rows)%size; 
                int val=mc->getItem(next);
                mc->setItem(next,t);
                t=val;
                visited[i] = true; 
                i = next; 
            } 
            while (i != cycle); 

            for (i=1;i<size&&visited[i];i++) ;  
        }
        int temp=this->num_rows;
        this->num_rows=this->num_cols;
        this->num_cols=temp;
        mc->flushTempBuffer();
    }

    void Matrix::makePermanent(){
        logger.log("Table::makePermanent");
        bufferManager.deleteFile(this->sourceFileName);
        string newSourceFile = "../data/" + this->MatrixName + ".csv";
        ofstream fout(this->sourceFileName, ios::out);
        MatrixCursor* mycursor=new MatrixCursor(this->MatrixName);
        vector<int> row(this->num_cols);
        int data_counter=0;
        for(int j=0;j<this->num_rows;j++){
            for(int i=0;i<this->num_cols;i++){
                row[i]=mycursor->getItem(data_counter);
                data_counter++;
            }
            this->writeRow(row,fout);
        }
        fout.close();
    } 