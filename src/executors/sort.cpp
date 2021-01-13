#include"global.h"
/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * 
 * sorting_order = ASC | DESC 
 */





bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");

//    else if(tokenizedQuery)

    if(tokenizedQuery.size()!= 8 || tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN"){
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortResultRelationName = tokenizedQuery[0];
    parsedQuery.sortColumnName = tokenizedQuery[5];
    parsedQuery.sortRelationName = tokenizedQuery[3];
    string sortingStrateg = tokenizedQuery[7];
    if(sortingStrateg == "ASC")
        parsedQuery.sortingStrategy = ASC;
    else if(sortingStrateg == "DESC")
        parsedQuery.sortingStrategy = DESC;
    else{
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    return true;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    if(tableCatalogue.isTable(parsedQuery.sortResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    return true;
}

bool comp(pair<int,string> &p1,pair<int,string> &p2){

    return p1.first>p2.first;

}


class Node{
public:
    int colNo; //key
    string row_str;
    int index; //value
};

class comp1
{
public:
    bool operator() (Node n1,Node n2)
    {
        return n1.colNo<n2.colNo;
    }
};



class comp2
{
public:
    bool operator() (Node n1,Node n2)
    {
        return n1.colNo>n2.colNo;
    }
};







void executeSORT(){
    logger.log("executeSORT");
    int k=5;
    if(!((parsedQuery.bufferSize).empty())){
            k=stoi(parsedQuery.bufferSize);
    }
    



    Table *table= tableCatalogue.getTable(parsedQuery.sortRelationName);
    int column_Index=0;
    
    vector<string> column_names=table->columns;
    
    long long int block_numbers=table->blockCount;
    int columnIndex=0;
    for(int i=0;i<column_names.size();i++){
        if(column_names[i]==parsedQuery.sortColumnName){
            columnIndex=i;
            break;
        }
    }

        int z=0;
        while(z<block_numbers){
            string old_file = "../data/temp/";
            old_file+=parsedQuery.sortRelationName;
            old_file+="_Page";
            old_file+=to_string(z);
            string new_file="../data/temp/";
            new_file+=parsedQuery.sortRelationName;
            new_file+= "_temp_Page";
            new_file+=to_string(z);
            
            string command = "cp ";
            command+=old_file;
            command+=" ";
            command+=new_file;
            system(command.c_str());
            string name_of_page=new_file;
            int flag_set=parsedQuery.sortingStrategy;
                
                string newline,newWord;
                ifstream fin(name_of_page,ios::in);
                vector<pair<int,string>> result_vec; 
                int i=0;

                for(;getline(fin,newline);i++){
                    stringstream str(newline);
                    vector<int> temp_vec;
                    while(getline(str,newWord,' ')){
                        temp_vec.push_back(stoi(newWord));
                    }
                    result_vec.push_back({temp_vec[columnIndex],newline});
                    
                }
                fin.close();

                switch(flag_set){
                    case 0:
                        sort(result_vec.begin(),result_vec.end());
                        break;                         
                    case 1:
                        sort(result_vec.begin(),result_vec.end(),comp);
                        break;
                }

                ofstream fout(name_of_page,ios::trunc);
                int r=0;
                while(r<result_vec.size()){
                    string sk=result_vec[r].second;
                    fout<<sk;
                    fout<<'\n';
                    r++;
                }

                fout.close();

            z++;
        }
    int blockCount=(int)block_numbers;
vector<string> all_files;
    int vamp=0;
    while(vamp<blockCount){

        string name_of_page="../data/temp/";

        name_of_page+=parsedQuery.sortRelationName;

        name_of_page+="_temp_Page";

        name_of_page+=to_string(vamp);

        all_files.push_back(name_of_page);
        vamp++;
    }

    int pass_val=1;
    for(;all_files.size()>1;){
        vector<string> all_files_ouput;
        int out=0;
        for(int i=0;i<all_files.size();i++){

            vector<string> inputs_files;
            for(int pp_val=0;pp_val<k-1 && i<all_files.size();pp_val++){
                inputs_files.push_back(all_files[i]);
                i++;
            }

            string output_name_of_file="../data/temp/";
            output_name_of_file+=parsedQuery.sortRelationName;
            output_name_of_file+="_temp_Page_";
            output_name_of_file+=to_string(pass_val);
            output_name_of_file+="_";
            output_name_of_file+=to_string(out);

// caller: //          merge(inputs_files,output_name_of_file,columnIndex,parsedQuery.sortingStrategy);
//function//            merge(vector<string> fileToSort, string mergedFile,int col_sort,int type)
    int k=inputs_files.size();
     ifstream input_ptr[inputs_files.size()];
     ofstream output_ptr;
     int ptx=0;
     while(ptx<inputs_files.size())
     {
         input_ptr[ptx].open(inputs_files[ptx]);
        ptx++;
     }

    cout<<"file_name:"<<output_name_of_file<<endl;

     output_ptr.open(output_name_of_file);

     if(output_ptr.is_open()){
        cout<<"AFTER Yes"<<endl;
     }

     else{
        cout<<"not open"<<endl;
     }
     int option=parsedQuery.sortingStrategy;
     switch(option){
            case 1:
                {
                        priority_queue<Node,vector<Node>,comp1> minq;
                        int i_val=0;
                        while(i_val<(inputs_files.size())){
                                     string newline,newword;
                                    getline(input_ptr[i_val],newline);
                                    stringstream str(newline);
                                    vector<int> temp_vec;
                                    for(;getline(str,newword,' ');){
                                         temp_vec.push_back(stoi(newword));
                                    }
                                    minq.push({temp_vec[columnIndex],newline,i_val});
                                    i_val++;
                                }

                                for(;minq.size()>0;){
                                    Node node=minq.top();
                                    minq.pop();
                                    output_ptr<<node.row_str;
                                    output_ptr<<"\n";
                                    string eachline,newword;
                                    if(true==true && getline(input_ptr[node.index],eachline)){
                                        //cout<<"inside"
                                        stringstream s(eachline);
                                        vector<int> temp_arr;
                                        for(;getline(s,newword,' ');){
                                            temp_arr.push_back(stoi(newword));
                                        }

                                        minq.push({temp_arr[columnIndex],eachline,node.index});
                                    }
                                }
                
                            break;
                }

            case 0:
                        priority_queue<Node,vector<Node>,comp2> maxq;
                        int i_val=0;
                        while(i_val<(inputs_files.size())){
                                     string newline,newword;
                                    getline(input_ptr[i_val],newline);
                                    stringstream str(newline);
                                    vector<int> temp;
                                    while(getline(str,newword,' ')){
                                         temp.push_back(stoi(newword));
                                    }
                                    maxq.push({temp[columnIndex],newline,i_val});
                                    i_val++;
                                }

                                for(;maxq.size()>0;){
                                    Node node=maxq.top();
                                    maxq.pop();
                                    output_ptr<<node.row_str;
                                    output_ptr<<"\n";
                                    string eachline,newword;
                                    if(true==true && getline(input_ptr[node.index],eachline)){
                                        //cout<<"inside"
                                        stringstream s(eachline);
                                        vector<int> temp_arr;
                                        while(getline(s,newword,' ')){
                                            temp_arr.push_back(stoi(newword));
                                        }

                                        maxq.push({temp_arr[columnIndex],eachline,node.index});
                                    }
                                }
                
                            break;

     }



            int k_close=0;
         while(k_close<inputs_files.size()){
            input_ptr[k_close].close();
            k_close++;
         }

         output_ptr.close();

            all_files_ouput.push_back(output_name_of_file);
            out++;
        }


        int kazam=0;
        while(kazam<all_files.size()){
            kazam++;
        }

        int z=0;
        while(z<all_files.size())
        {
            switch(remove(all_files[z].c_str())){
                case 1:
                    cout<<"file removed"<<endl;
                    break;
                case 0:
                    cout<<"error"<<endl;
                    break;
            }
            z++;
        }

        all_files.clear();

        int z1=0;
        while(z1<all_files_ouput.size()){
            all_files.push_back(all_files_ouput[z1]);
            z1++;            
        }

        all_files_ouput.clear();
        pass_val++;
    }
    
string file_Name_To_Return=all_files[0];



    cout<<"fileNameToReturn:"<<file_Name_To_Return<<endl;
    
    Table* outputTableObject = new Table(parsedQuery.sortResultRelationName,table->columns);
    string newLine,newWord;

    ifstream fin(file_Name_To_Return, ios::in);

    while(getline(fin,newLine))
    {
        stringstream ss(newLine);
        vector<int> row;
        while(getline(ss,newWord,' '))
            row.push_back(stoi(newWord));
        outputTableObject->writeRow(row);
    }
    fin.close();

    outputTableObject->blockify();
    tableCatalogue.insertTable(outputTableObject);

    switch(remove(file_Name_To_Return.c_str()))
    {
        case 1:
            logger.log("ERROR");
            cout<<" ERROR"<<endl;
            break;

        case 0:
            cout<<"file removed"<<endl;

    } 

           
    cout<<"sorting DONE"<<endl;
    return;

}