// Project identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <cstdint>
#include <functional>
#include <getopt.h>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "TableEntry.h"
#include "silly.h"
#include <algorithm>

using namespace std;

void getMode(int argc, char * argv[], Modes &modes) {

    opterr = false;
    int choice;
    int index = 0;
    option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"quiet", no_argument, nullptr, 'q'}
    };

    while ((choice = 
                getopt_long(argc, argv, "hq", long_options, &index)) != -1){
        switch (choice) {
            case 'h': {
                modes.help = true;
                break;            
            }

            case 'q': {
                modes.quiet = true;
                break;   
            }       

            default: 
                cerr << "Error: invalid option\n";
                exit(1);
        } //switch choice
    } //while loop
} //getMode()

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);

    Modes modes;
    getMode(argc, argv, modes); //know output mode
    
    //unordered map: string to table object
    unordered_map<string, Table> database;

    string command;
    do{
        cout << "% ";
        
        //cout << "hello";
        cin >> command;
        //cout << command << "\n";
        //process command

        //quit
        char cmd = command[0];

        switch(cmd){
            case 'Q':{
                break;
            }
            case '#':{
                string temp;
                getline(cin, temp);
                break;
            }
            case 'C':{
                create(database);
                break;
            }
            case 'R':{
                remove(database);
                break;
            }
            case 'I':{
                insert(database);
                break;
            }
            case 'P':{
                print(database, modes);
                break;
            }
            case 'D':{
                deleteFrom(database);
                break;
            }
            case 'G':{
                generate(database);
                break;
            }
            case 'J':{
                join(database, modes);
                break;
            }
            default:{
                cout << "Error: unrecognized command\n";
                string temp;
                getline(cin, temp);
                //exit(1);
                break;
            }
        }         
    } while(command !="QUIT");

    string tmp;
    getline(cin, tmp);
    cout << tmp;

    cout << "Thanks for being silly!\n";
    
    return 0;
}

//done
void create(unordered_map<string, Table> &database){
    string name;
    cin >> name;

    auto it = database.find(name);
    if(it == database.end()){
        //create table and add to database
        int numCols;
        cin >> numCols;
        //cout << "num cols is " << numCols << "\n";
        vector<EntryType> colTypes;
        colTypes.reserve(numCols);
        
        //insert column types
        unordered_map<string, int> colIndex;
        string type;
        for(int i = 0; i<numCols; i++){
            cin >> type;
            EntryType t;
            if(type[0]=='s'){
                t = EntryType::String;
            }
            else if(type[0]=='d'){
                t = EntryType::Double;
            }
            else if(type[0]=='b'){
                t = EntryType::Bool;
            }   
            else{
                t = EntryType::Int;
            }
            colTypes.emplace_back(t); 
        }

        //insert column names AND column index vector
        vector<string> colNames;
        colNames.resize(numCols);
        string colNamesTogether;
        string colName;
        for(int i = 0; i<numCols; i++){
            cin >> colName;
            colNamesTogether += colName + " ";
            colNames[i] = colName; 
            colIndex[colName] = i;
            //cout << "I is " << i << "\n";
        }

        // Table* table = new Table(name, colTypes, colNames, colIndex);
        // database[table->name] = table;
        //emplace into unordered map
        Table table{name, colTypes, colNames, colIndex};
        database.emplace(name, table);

        cout << "New table " << name << " with column(s) " 
                << colNamesTogether << "created\n";
        return;
    }
    else{
        //table already exists
        cout << "Error during CREATE: Cannot create already existing table " << name << "\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
}

//done
void remove(unordered_map<string, Table> &database){
    string name;
    cin >> name;

    auto it = database.find(name);
    if(it == database.end()){
        cout << "Error during REMOVE: " << name << " does not name a table in the database\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
    else{
        //delete it->second;
        database.erase(name);
        cout << "Table " << name << " deleted\n";
        return;
    }
}

//done
void insert(unordered_map<string, Table> &database){
    string name, temp;
    cin >> temp >> name;
    

    auto it = database.find(name);
    if(it == database.end()){
        cout << "Error during INSERT: " << name << " does not name a table in the database\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
    else{
        it->second.insertTable();
    }

    return;
}

void Table::insertTable(){
    size_t index = data.size();
    size_t added;
    string temp;
    cin >> added >> temp; 

    data.resize(index+added);

    //string value;
    // bool bVal;
    // double dVal;

    //cout << "ready to insert\n";
    //cout << "index is " << index << " added is " << added << " size is " << colTypes.size() << "\n";
    
    for(size_t r = index; r<index+added; r++){
        data[r].reserve(colTypes.size());
        for(size_t c = 0; c<colTypes.size(); c++){
            //cout << "in col loop\n";
            
            EntryType & et = colTypes[c];
            
            switch(et){
                case EntryType::String:{
                    string value;
                    cin >> value;
                    data[r].emplace_back(value);
                    break;
                }
                case EntryType::Bool:{
                    bool value;
                    cin >> boolalpha >> value;
                    data[r].emplace_back(value);
                    break;
                }
                case EntryType::Int:{
                    int value; 
                    cin >> value;
                    data[r].emplace_back(value);
                    break;
                }
                default:{
                    double value;
                    cin >> value;
                    data[r].emplace_back(value);
                    break;
                }
            }
        }

        //take care of indices here
        //if a bst OR hash index exists
        // if(index != ' ' && !fixIndex){
        // if(index != ' '){
        //     int indexNum = colIndex[indexCol];
        //     if(index == 'b'){
        //         bstIndex[data[r][indexNum]].emplace_back(r);
        //     }
        //     else{
        //         hashIndex[data[r][indexNum]].emplace_back(r);
        //     }
        // }
    }
    updateIndex();

    cout << "Added " << added << " rows to " << name << " from position " << index << " to " << (index+added-1) << "\n";        
    // auto size = database.at(name).data.size();
    // cout << size << "is the size\n";
}

//done
void print(unordered_map<string, Table> &database, Modes & modes){
    string temp, name;
    cin >> temp >> name;

    auto it = database.find(name);
    if(it == database.end()){
        cout << "Error during PRINT: " << name << " does not name a table in the database\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
    else{
        it->second.printTable(modes);
    }
}

void Table::printTable(Modes & modes){
    int numCols;
    cin >> numCols;

    auto size = colNames.size();

    vector<int> colPrintFlag(size);
    string currCol;

    //for the number of columns, read in each column and set to true for printing
    for(int i = 0; i<numCols; i++){
        cin >> currCol;
                    
        auto itCol = colIndex.find(currCol);
        if(itCol == colIndex.end()){
            cout << "Error during PRINT: " << currCol << " does not name a column in " << name << "\n";
            string temp;
            getline(cin, temp);
            //exit(1);
        }
        else{
            colPrintFlag[i] = colIndex[currCol];
        } 
    }

    string cmd;
    cin >> cmd;

    if(cmd[0] == 'W'){
        //where colname OP value
        string compareCol;
        cin >> compareCol;
        char op;
        cin >> op;

        auto itCol = colIndex.find(currCol);
        if(itCol == colIndex.end()){
            cout << "Error during PRINT: " << compareCol << " does not name a column in " << name << "\n";
            string temp;
            getline(cin, temp);
            //exit(1);
        }

        if(!modes.quiet){            
            //print out the columns
            for(int i  = 0; i<numCols; i++){                    
                cout << colNames[colPrintFlag[i]] << " ";
            }
            cout << "\n";
        }

        //check indices
        //for conditional column,
        //if no index or hash, print in order of insertion
        //if bst, print in order on bst, tie break by order of insertion

        auto colType = colTypes[colIndex[compareCol]];
        int count;

        switch(colType){
            case EntryType::Bool :{
                bool value;
                cin >> value;
                TableEntry x(value);
                count = printHelper(modes, x, op, compareCol, colPrintFlag, numCols);
                break;
            }
            case EntryType::Double : {
                double value;
                cin >> value;
                TableEntry x(value);
                count = printHelper(modes, x, op, compareCol, colPrintFlag, numCols);
                break;
            }
            case EntryType::Int : {
                int value;
                cin >> value;
                TableEntry x(value);
                count = printHelper(modes, x, op, compareCol, colPrintFlag, numCols);
                break;
            }
            case EntryType::String : {
                string value;
                cin >> value;
                TableEntry x(value);
                count = printHelper(modes, x, op, compareCol, colPrintFlag, numCols);
                break;
            }
        }

        //change # rows to count
        cout << "Printed " << count << " matching rows from " << name << "\n";
        
    }
    else{
        //print all 

        if(!modes.quiet){            
            //print out the columns
            for(int i  = 0; i<numCols; i++){                    
                cout << colNames[colPrintFlag[i]] << " ";
            }
            cout << "\n";

            //print out values for each row for correct columns
            for(size_t r = 0; r<data.size(); r++){
                for(int c = 0; c<numCols; c++){
                        cout << boolalpha << data[r][colPrintFlag[c]] << " ";
                }
                cout << "\n";
            }
        }
        cout << "Printed " << data.size() << " matching rows from " << name << "\n";
    }
}

int Table::printHelper(Modes & modes, TableEntry & compareVal, char op, string compareCol, vector<int> & colPrintFlag, int numCols){
    switch(op) {
        case '>' : {
            if(index == 'b' && indexCol == compareCol){
                return printBst(modes, op, compareVal, colPrintFlag, numCols);
            }
            else{
                return printNoIndex<GreaterThan>(modes, compareCol, compareVal, colPrintFlag, numCols);
            }
        }
        case '<' : {
            if(index == 'b' && indexCol == compareCol){
                return printBst(modes, op, compareVal, colPrintFlag, numCols);
            }
            else{
                return printNoIndex<LessThan>(modes, compareCol, compareVal, colPrintFlag, numCols);
            }
        }
        case '=' : {
            if(index == 'b' && indexCol == compareCol){
                return printBst(modes, op, compareVal, colPrintFlag, numCols);
            }
            else if(index == 'h' && indexCol == compareCol){
                return printHash(modes, compareVal, colPrintFlag, numCols);
            }
            else{
                return printNoIndex<EqualTo>(modes, compareCol, compareVal, colPrintFlag, numCols);
            }
        }
    }
    return 0;
}

template <typename Comparator>
int Table::printNoIndex(Modes & modes, string colName, TableEntry & compareVal, vector<int> & colPrintFlag, int numCols){
    int count = 0;
    Comparator cmp(compareVal, colIndex.at(colName));

    for(size_t r = 0; r<data.size(); r++){
        if(cmp(data[r])){
            if(!modes.quiet){
                for(int c = 0; c<numCols; c++){
                    cout << boolalpha << data[r][colPrintFlag[c]] << " ";
                }
                cout << "\n";
                count += 1;
            }
        }
    }

    return count;
}

int Table::printBst(Modes & modes, char op, TableEntry & compareVal, vector<int> & colPrintFlag, int numCols){

    int count = 0;

    map<TableEntry, vector<size_t>>::iterator it;
    if(op == '>')
        it = bstIndex.upper_bound(compareVal);
    else if(op == '<')
        it = bstIndex.begin();
    else{
        it = bstIndex.find(compareVal);

        if(it != bstIndex.end()){
            if(!modes.quiet){
                for(auto tmp : it->second){
                    for(int c = 0; c<numCols; c++){
                        cout << boolalpha << data[tmp][colPrintFlag[c]] << " ";
                    }
                    cout << "\n";
                    count += 1;
                }
                return count;
            }
            return int(it->second.size());
        }
        return 0;
    }    

    while(it != bstIndex.end()){
        if(op == '<' && (it->first > compareVal || it->first == compareVal)) break;

        if(!modes.quiet){
            for(int r = 0; r<int(it->second.size()); r++){
                for(int c = 0; c<numCols; c++){
                    cout << boolalpha << data[it->second[r]][colPrintFlag[c]] << " ";
                }
                cout << "\n";
            }
        }

        count += int(it->second.size());
        ++it;        
    }

    return count;
}

int Table::printHash(Modes & modes, TableEntry & compareVal, vector<int> & colPrintFlag, int numCols){
    auto it = hashIndex.find(compareVal);
    int count = 0;

    if(it != hashIndex.end()){
        if(!modes.quiet){
            for(auto tmp : it->second){
                for(int c = 0; c<numCols; c++){
                    cout << boolalpha << data[tmp][colPrintFlag[c]] << " ";
                }
                cout << "\n";
                count += 1;
            }
            return count;
        }
        return int(it->second.size());
    }

    return 0;
}

void deleteFrom(unordered_map<string, Table> &database){
    string name, temp;
    cin >> temp >> name;

    auto it = database.find(name);
    if(it == database.end()){
        cout << "Error during DELETE: " << name << " does not name a table in the database\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
    else{
        string colName;
        char op;
        //"WHERE"
        cin >> temp >> colName >> op;

        auto & colIndex = it->second.colIndex;
        auto & colTypes = it->second.colTypes;

        auto it2 = colIndex.find(colName);
        if(it2 == colIndex.end()){
            cout << "Error during DELETE: " << colName << " does not name a column in " << name << "\n";
            string temp;
            getline(cin, temp);
            //exit(1);
        }
        else{

            auto colType = colTypes[colIndex[colName]];

            switch(colType){
                case EntryType::Bool :{
                    bool value;
                    cin >> value;
                    TableEntry x(value);
                    it->second.deleteCompareHelper(op, colName, x);
                    break;
                }
                case EntryType::Double : {
                    double value;
                    cin >> value;
                    TableEntry x(value);
                    it->second.deleteCompareHelper(op, colName, x);
                    break;
                }
                case EntryType::Int : {
                    int value;
                    cin >> value;
                    TableEntry x(value);
                    it->second.deleteCompareHelper(op, colName, x);
                    break;
                }
                case EntryType::String : {
                    string value;
                    cin >> value;
                    TableEntry x(value);
                    it->second.deleteCompareHelper(op, colName, x);
                    break;
                }
            }
        }        
    }
}

void Table::deleteCompareHelper(char op, string colName, TableEntry & compareVal){
    switch(op){
        case '>' : {
            deleteWhere<GreaterThan>(colName, compareVal);
            break;
        }
        case '<' : {
            deleteWhere<LessThan>(colName, compareVal);
            break;
        }
        case '=' : {
            deleteWhere<EqualTo>(colName, compareVal);
            break;
        }
    }
}

template<typename Comparator>
void Table::deleteWhere(string colName, TableEntry & compareVal){
    Comparator cmp(compareVal, colIndex[colName]);
    auto it = remove_if(data.begin(), data.end(), cmp);

    int origRows = int(data.size());
    data.erase(it, data.end());
    int removed = origRows - int(data.size());

    cout << "Deleted " << removed << " rows from " << name << "\n";

    //if(removed > 0) 
    updateIndex();
}

void generate(unordered_map<string, Table> &database){
    string name, temp;
    cin >> temp >> name;

    auto it = database.find(name);
    if(it == database.end()){
        cout << "Error during GENERATE: " << name << " does not name a table in the database\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
    else{
        string indexType, colName;
        cin >> indexType >> temp >> temp >> colName;

        auto colIt = it->second.colIndex.find(colName);

        if(colIt == it->second.colIndex.end()){
            cout << "Error during GENERATE: " << colName << " does not name a column in " << name << "\n";
            string temp;
            getline(cin, temp);
            //exit(1);
        }
        else{
            it->second.index = indexType[0];
            it->second.indexCol = colName;

            it->second.updateIndex();

            cout << "Created " << indexType << " index for table " << name << " on column " << colName << "\n";
        }        
    }
}

void Table::updateIndex(){
    if(!bstIndex.empty()) bstIndex.clear();
    if(!hashIndex.empty()) hashIndex.clear();
    
    int indexNum = colIndex[indexCol];

    if(index == 'b'){
        for(size_t r = 0; r<data.size(); r++)
            bstIndex[data[r][indexNum]].emplace_back(r);
    }
    else{
        hashIndex.reserve(data.size());
        for(size_t r = 0; r<data.size(); r++)
            hashIndex[data[r][indexNum]].emplace_back(r);
    }
}

void join(unordered_map<string, Table> &database, Modes & modes){
    if(!modes.quiet){}
    string name1, name2, temp;
    cin >> name1 >> temp >> name2;

    auto it1 = database.find(name1);
    auto it2 = database.find(name2);
    if(it1 == database.end()){
        cout << "Error during JOIN: " << name1 << " does not name a table in the database\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
    else if(it2 == database.end()){
        cout << "Error during JOIN: " << name2 << " does not name a table in the database\n";
        string temp;
        getline(cin, temp);
        //exit(1);
    }
    else{
        auto & table1 = it1->second;
        auto & table2 = it2->second;
        string col1, col2;
        //WHERE col1 = col2 AND PRINT
        cin >> temp >> col1 >> temp >> col2 >> temp >> temp;
        auto colIt1 = table1.colIndex.find(col1);
        auto colIt2 = table2.colIndex.find(col2); 

        if(colIt1 == table1.colIndex.end()){
            cout << "Error during JOIN: " << name1 << " does not name a column in " << name1 << "\n";
            string temp;
            getline(cin, temp);
            //exit(1);
        }
        else if(colIt2 == table2.colIndex.end()){
            cout << "Error during JOIN: " << name2 << " does not name a column in " << name2 << "\n";
            string temp;
            getline(cin, temp);
            //exit(1);
        }
        else{
            //check all columns
            
            int numCols;
            string colName;
            int tableNum;
            cin >> numCols;

            //record all columns down in vector
            vector<pair<string, int>> joinCols;
            joinCols.reserve(numCols);

            string colsTogether = "";

            for(int i = 0; i<numCols; i++){
                cin >> colName >> tableNum;
                if(tableNum==1){
                    auto colIt = table1.colIndex.find(colName);
                    if(colIt == table1.colIndex.end()){
                        cout << "Error during JOIN: " << colName << " does not name a column in " << table1.name << "\n";
                        string temp;
                        getline(cin, temp);
                        //exit(1);
                    }
                }
                else{
                    auto colIt = table2.colIndex.find(colName);
                    if(colIt == table2.colIndex.end()){
                        cout << "Error during JOIN: " << colName << " does not name a column in " << table2.name << "\n";
                        string temp;
                        getline(cin, temp);
                        //exit(1);
                    }
                }

                joinCols.emplace_back(pair<string, int>(colName, tableNum));
                colsTogether += colName + " ";
            }

            if(!modes.quiet){
                cout << colsTogether << "\n";
            }

            //iterate throguh first table from beginning to end
            //for each col1 in tab1, find matching col2 in tab2 if it exists
            //need to check for bst, for hash, and for no indexing

            //for hash
            int numRows = 0;
            if(table2.index == 'h' && table2.indexCol == col2){
                numRows = joinHelper(table1, table2, joinCols, col1, table2.hashIndex, modes);
            }
            else if(table2.index == 'b' && table2.indexCol == col2){ //for bst
                numRows = joinHelper(table1, table2, joinCols, col1, table2.bstIndex, modes);
            }
            else{//for none; generate index
                unordered_map<TableEntry, vector<size_t>> tempHashIndex;
                tempHashIndex.reserve(table2.data.size());
                for(size_t i = 0; i<table2.data.size(); i++){
                    tempHashIndex[table2.data[i][table2.colIndex[col2]]].emplace_back();
                }
                numRows = joinHelper(table1, table2, joinCols, col1, tempHashIndex, modes);
            }

            cout << "Printed " << numRows << " rows from joining " << name1 << " to " << name2 << "\n";
        }
    }
}

template <typename T>
int joinHelper(Table & table1, Table & table2, vector<pair<string, int>> joinCols, string col1, T & index, Modes & modes){
    auto & colIndex = table1.colIndex;
    int numRows = 0;
    int col1Num = colIndex[col1];

    for(size_t r = 0; r<table1.data.size(); r++){
        TableEntry temp = table1.data[r][col1Num]; //table entry for each row at colname1
        auto it = index.find(temp);
        if(it != index.end()){
            if(!modes.quiet){
                auto startCond = index.find(temp)->second.begin();
                auto endCond = index.find(temp)->second.end();
                while(startCond != endCond){             
                    for(auto c: joinCols){
                        if(c.second==1){
                            cout << table1.data[r][table1.colIndex[c.first]] << " ";
                        }
                        else{
                            cout << table2.data[r][table2.colIndex[c.first]] << " ";
                        }
                    }
                    cout << "\n";
                    ++startCond;
                }
                numRows += 1;
            }
        }
    }

    return numRows;
}