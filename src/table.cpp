#include "global.h"

/**
 * @brief Construct a new Table:: Table object
 *
 */
Table::Table()
{
    logger.log("Table::Table");
}

/**
 * @brief Construct a new Table:: Table object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param tableName 
 */
Table::Table(string tableName)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/" + tableName + ".csv";
    this->tableName = tableName;
}

/**
 * @brief Construct a new Table:: Table object used when an assignment command
 * is encountered. To create the table object both the table name and the
 * columns the table holds should be specified.
 *
 * @param tableName 
 * @param columns 
 */
Table::Table(string tableName, vector<string> columns)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/temp/" + tableName + ".csv";
    this->tableName = tableName;
    this->columns = columns;
    this->columnCount = columns.size();
    //this below line needs correction
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (32 * columnCount));
    this->writeRow<string>(columns);
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks and updates table
 * statistics.
 *
 * @return true if the table has been successfully loaded 
 * @return false if an error occurred 
 */
bool Table::load()
{
    logger.log("Table::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();
        if (this->extractColumnNames(line))
            if (this->blockify())
                return true;
    }
    fin.close();
    return false;
}

/**
 * @brief Function extracts column names from the header line of the .csv data
 * file. 
 *
 * @param line 
 * @return true if column names successfully extracted (i.e. no column name
 * repeats)
 * @return false otherwise
 */
bool Table::extractColumnNames(string firstLine)
{
    logger.log("Table::extractColumnNames");
    unordered_set<string> columnNames;
    string word;
    stringstream s(firstLine);
    while (getline(s, word, ','))
    {
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        if (columnNames.count(word))
            return false;
        columnNames.insert(word);
        this->columns.emplace_back(word);
    }
    this->columnCount = this->columns.size();
    //below line needs to be corrected.
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (32 * this->columnCount));
    return true;
}

/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Table::blockify()
{
    logger.log("Table::blockify");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(this->columnCount, 0);
    vector<vector<int>> rowsInPage(this->maxRowsPerBlock, row);
    int pageCounter = 0;//number of rows filled in that block.
    unordered_set<int> dummy;
    dummy.clear();
    this->distinctValuesInColumns.assign(this->columnCount, dummy);
    this->distinctValuesPerColumnCount.assign(this->columnCount, 0);
    getline(fin, line);
    while (getline(fin, line))
    {
        stringstream s(line);
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (!getline(s, word, ','))
                return false;
            row[columnCounter] = stoi(word);
            rowsInPage[pageCounter][columnCounter] = row[columnCounter];
        }
        pageCounter++;
        this->updateStatistics(row);
        if (pageCounter == this->maxRowsPerBlock)
        {
            bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }
    }
    if (pageCounter)
    {
        bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
        this->blockCount++;
        this->rowsPerBlockCount.emplace_back(pageCounter);
        pageCounter = 0;
    }

    if (this->rowCount == 0)
        return false;
    this->distinctValuesInColumns.clear();
    return true;
}

/**
 * @brief Given a row of values, this function will update the statistics it
 * stores i.e. it updates the number of rows that are present in the column and
 * the number of distinct values present in each column. These statistics are to
 * be used during optimisation.
 *
 * @param row 
 */
void Table::updateStatistics(vector<int> row)
{
    this->rowCount++;
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (!this->distinctValuesInColumns[columnCounter].count(row[columnCounter]))
        {
            this->distinctValuesInColumns[columnCounter].insert(row[columnCounter]);
            this->distinctValuesPerColumnCount[columnCounter]++;
        }
    }
}

/**
 * @brief Checks if the given column is present in this table.
 *
 * @param columnName 
 * @return true 
 * @return false 
 */
bool Table::isColumn(string columnName)
{
    logger.log("Table::isColumn");
    for (auto col : this->columns)
    {
        if (col == columnName)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Renames the column indicated by fromColumnName to toColumnName. It is
 * assumed that checks such as the existence of fromColumnName and the non prior
 * existence of toColumnName are done.
 *
 * @param fromColumnName 
 * @param toColumnName 
 */
void Table::renameColumn(string fromColumnName, string toColumnName)
{
    logger.log("Table::renameColumn");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (columns[columnCounter] == fromColumnName)
        {
            columns[columnCounter] = toColumnName;
            break;
        }
    }
    return;
}

/**
 * @brief Function prints the first few rows of the table. If the table contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Table::print()
{
    logger.log("Table::print");
    uint count = min((long long)PRINT_COUNT, this->rowCount);

    //print headings
    this->writeRow(this->columns, cout);

    Cursor cursor(this->tableName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < count; rowCounter++)
    {
        //cout<<"the page index of the cursor is "<<cursor.pageIndex<<endl;
        //cout<<"the page pointer of the cursor is "<<cursor.pagePointer<<endl;
        //cout<<"infor about the page is "<<cursor.page.rowCount<<endl;
        row = cursor.getNext();
        this->writeRow(row, cout);
    }
    printRowCount(this->rowCount);
}



/**
 * @brief This function returns one row of the table using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
void Table::getNextPage(Cursor *cursor)
{
    logger.log("Table::getNext");

        if (cursor->pageIndex < this->blockCount - 1)
        {
            cursor->nextPage(cursor->pageIndex+1);
        }
}



/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
void Table::makePermanent()
{
    logger.log("Table::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->tableName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    //print headings
    this->writeRow(this->columns, fout);

    Cursor cursor(this->tableName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, fout);
    }
    fout.close();
}

/**
 * @brief Function to check if table is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Table::isPermanent()
{
    logger.log("Table::isPermanent");
    if (this->sourceFileName == "../data/" + this->tableName + ".csv")
    return true;
    return false;
}

/**
 * @brief The unload function removes the table from the database by deleting
 * all temporary files created as part of this table
 *
 */
void Table::unload(){
    logger.log("Table::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->tableName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}

/**
 * @brief Function that returns a cursor that reads rows from this table
 * 
 * @return Cursor 
 */
Cursor Table::getCursor()
{
    logger.log("Table::getCursor");
    Cursor cursor(this->tableName, 0);
    return cursor;
}
/**
 * @brief Function that returns the index of column indicated by columnName
 * 
 * @param columnName 
 * @return int 
 */
int Table::getColumnIndex(string columnName)
{
    logger.log("Table::getColumnIndex");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (this->columns[columnCounter] == columnName)
            return columnCounter;
    }
}



/*
 this method takes the index type coloumn name and bucket_count as paramets and created a index
 accordingly.
 it used a linear hashing implementation created by us and it puts each row into that linear hashing object
 such that the column value is the key and the tuple <col_value,rownumber> is the value.
*/
void Table::createIndex(string colname,IndexingStrategy indextype,int bucket_count){
    cout<<"creating the index"<<endl;
    int col_index=getColumnIndex(colname);
    this->indexedColumn=colname;
    Cursor tableCursor=getCursor();
    if(indextype==HASH){
        cout<<"this is a linear hashing index"<<endl;
        if(lh!=NULL){
            delete lh;
        }
        lh=new LinearHash(bucket_count,1024);//here we may need to place the bucket count.
        for(long long int i=0;i<rowCount;i++){
            vector<int> row=tableCursor.getNext();
            lh->insert(row[col_index],i);
        }
        indexUsed=HASH;
        cout<<"done with indexing."<<endl;
    }else{
        cout<<"other hasing methods not implimented yet"<<endl;
    }
}



/*
this method takes arr vector as a paramenter (the value of the row) and insert it in the table.
if the last block has any empty rows in it then it places the row arr in it otherwise it 
created a new block and puts the row arr in it.
*/
void Table::insert(vector<int> arr){
    int last_block=this->blockCount-1;
    Cursor* mycursor=new Cursor(this->tableName,last_block);
    if(mycursor->page.rowCount<this->maxRowsPerBlock){
        mycursor->page.rows[mycursor->page.rowCount]=arr;
        mycursor->page.rowCount++;
        mycursor->page.writePage();
        this->rowsPerBlockCount[last_block]++;
        bufferManager.pages.clear();
    }else{
        vector<vector<int>> tempmat(this->maxRowsPerBlock,vector<int>(this->columnCount));
        tempmat[0]=arr;
        bufferManager.writePage(this->tableName, this->blockCount,tempmat,1);
        this->rowsPerBlockCount.emplace_back(1);
        this->blockCount++;
    }
    this->rowCount++;
    if(indexUsed==HASH){
        cout<<"updating hashing index"<<endl;
        int col_index=getColumnIndex(this->indexedColumn);
        lh->insert(arr[col_index],rowCount-1);
    }
}
vector<int> getRowVectorFromString(string& s){
    vector<int> res;
    int n=s.length();
    string temp="";
    for(int i=0;i<n;i++){
        if(s[i]>='0'&&s[i]<='9'){
            temp+=s[i];
        }else if(temp!=""){
            res.push_back(stoi(temp));
            temp="";
        }
    }
    if(temp!=""){
        res.push_back(stoi(temp));
    }
    return res;
}


/*
in this method we read each row one by one from the file present in the filepath
and then use the above inplimented insert method to insert these row one by one in the
table.
*/
void Table::bulkInsert(string filePath){
    //cout<<"from here we start the phase 2 of the project"<<endl;
    ifstream fin(filePath);
    if(!fin.is_open()){
        cout<<"the file is unable to be opened"<<endl;
        return;
    }
    string temp;
    getline(fin,temp);
    while(getline(fin,temp)){//everything will work correctly if getline function is written correctly.
        vector<int> row=getRowVectorFromString(temp);
        this->insert(row);
    }
    fin.close();
    cout<<"doen with bulk insert"<<endl;
}


/*
this is a helper class which is used by the group by method to perform diffrent operations like AVG,MAX,MIN,SUM
*/
class myoperations{
    public:
    string operation_type;
    int temp_value1;//sum,min,max
    int temp_value2;//number of elements
    void set_operation(string op){
        operation_type=op;
        //insialization will be diffrent for min and max.
        if(operation_type=="MIN"){
            temp_value1=INT_MAX;
        }else if(operation_type=="MAX"){
            temp_value1=INT_MIN;
        }else{
            temp_value1=0;
        }
        temp_value2=0;
    }
    void set_value(int value){
        if(operation_type=="SUM"||operation_type=="AVG"){
            temp_value1+=value;
        }else if(operation_type=="MAX"){
            temp_value1=max(temp_value1,value);
        }else if(operation_type=="MIN"){
            temp_value1=min(temp_value1,value);
        }else{
            cout<<"this opertion is not allowed"<<endl;
        }
        temp_value2++;
    }
    void clearValue(){
        if(operation_type=="MIN"){
            temp_value1=INT_MAX;
        }else if(operation_type=="MAX"){
            temp_value1=INT_MIN;
        }else{
            temp_value1=0;
        }
        temp_value2=0;
    }
    int get_value(){
        if(operation_type!="AVG"){
            return temp_value1;
        }else{
            return temp_value1/temp_value2;//this is a integer data base floor will be returned.
        }
        return 0;
    }
};
//helper method used by groupby operation to fill the mapper.
void fillMapper(map<int,vector<int>>& mapper,vector<pair<int,int>>& bucket){
    //this is quite essencial.
    int num_elements=bucket.size();
    for(int i=0;i<num_elements;i++){
        mapper[bucket[i].first].push_back(bucket[i].second);
    }
}

/*
mainTable -> table in which we have to perform the group by operation.
resultantTable -> the table which is the result of this group by operation.
grouping_col_name -> the column whoes common values will be group thogether.
operating_col_name -> the column on which the operation will be performed.
operationType-> the type of operation {MIN MAX AVG and SUM}

first we we check if the indexing is present or not if the indexing is present then we
delete the indexing create the indexing with on the grouping_col_name 
and then we will our mapper structure which cointains rows which has only same col_name of the
grouping coloumn.
we fill in a loop for each bucket in the linear hashing index created by us.
and write data collected in mapper in a newly created resultant table.
hence doing the group by operation.
*/
void Table::groupBy(Table* mainTable,Table* resultantTable,
string grouping_col_name,string operating_col_name,string operationType){
    //setting up the correct index.
    //cout<<"first displaying the paramenters"<<endl;
    IndexingStrategy orignalStrategy=mainTable->indexUsed;
    string orignal_col_name="";
    if(orignalStrategy!=NOTHING){
        orignal_col_name=mainTable->indexedColumn;
    }
    delete mainTable->lh;
    mainTable->lh=NULL;
    mainTable->createIndex(grouping_col_name,HASH,2);
    Cursor cursor=mainTable->getCursor();
    //here we will do the group by operation.
    int grouping_col_index=mainTable->getColumnIndex(grouping_col_name);
    int operating_col_index=mainTable->getColumnIndex(operating_col_name);
    map<int,vector<int>> mapper;//key is the column value and vector is list of row number.
    mapper.clear();
    int num_buckets=mainTable->lh->numberOfBuckets;
    myoperations mop;
    mop.set_operation(operationType);
    for(int i=0;i<num_buckets;i++){
        mapper.clear();
        fillMapper(mapper,mainTable->lh->bucket[i]);
        for(auto itr:mapper){
            mop.clearValue();
            int col_value=itr.first;
            vector<int> row_ids=itr.second;
            vector<int> row;
            vector<int> res_row;
            res_row.clear();
            res_row.push_back(col_value);
            for(int i=0;i<row_ids.size();i++){
                //cout<<"start of the loop"<<endl;
                int block_id=row_ids[i]/mainTable->maxRowsPerBlock;
                int local_row_id=row_ids[i]%mainTable->maxRowsPerBlock;
                cursor.nextPage(block_id);
                row=cursor.page.getRow(local_row_id);
                mop.set_value(row[operating_col_index]);
                //cout<<"inside the loop"<<endl;
            }
            //cout<<"before pushing"<<endl;
            res_row.push_back(mop.get_value());
            //cout<<"after pushing"<<endl;
            //resultantTable->insert(res_row);//error is heppening during the insertion process.
            resultantTable->writeRow<int>(res_row);//lets see will this work.
            //cout<<"the problem is in insertion"<<endl;
        }
    }
    //leaving things with correct index.
    delete mainTable->lh;
    mainTable->lh=NULL;
    mainTable->indexUsed=NOTHING;
    mainTable->indexedColumn="";
    if(orignalStrategy!=NOTHING){
        mainTable->createIndex(orignal_col_name,HASH,2);
    }
    //cout<<"all set and done leaving the control"<<endl;
    
    
    //cout<<"blockifying things"<<endl;
    if(resultantTable->blockify())
        tableCatalogue.insertTable(resultantTable);
    else{
        //cout<<"Empty Table"<<endl;
        delete resultantTable;
    }
    return;
}