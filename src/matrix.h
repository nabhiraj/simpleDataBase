#include "matrix_cursor.h"
class Matrix{
    public:
    //we have to create a matrix file.
    string MatrixName="";
    string sourceFileName = "";
    string stable_sourceFileName="";
    //string matrixName = "";
    int max_block_limit;
    int num_rows;
    int num_cols;
    static map<string,Matrix*> matrixCatlog;
    //methods.
    static Matrix* getMatrix(string mat_name);
    static void setMatrix(string mat_name,Matrix* mat);
    Matrix(string matrix_name);
    bool load();
    void Transpose();
    void makePermanent();
//added 
template <typename T>
void writeRow(vector<T> row, ostream &fout)
{
    logger.log("Table::printRow");
    for (int columnCounter = 0; columnCounter < row.size(); columnCounter++)
    {
        if (columnCounter != 0)
            fout << ", ";
        fout << row[columnCounter];
    }
    fout << endl;
}
};
