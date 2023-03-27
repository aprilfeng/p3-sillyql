// Project identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <cstdint>
#include <getopt.h>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "TableEntry.h"
#include "silly.h"

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
    unordered_map<string, Table*> database;

    string command;
    do{
        cout << "% ";
        
        //cout << "hello";
        cin >> command;
        //cout << command << "\n";
        //process command

        //quit
        if(command[0] == 'Q'){
            break;
        }
        else if(command[0] == '#'){
            //comment
            string temp;
            getline(cin, temp);
        }
        else if(command[0] == 'C'){
            create(database);
        }
        else if(command[0] == 'R'){
            remove(database);
        }
        else if(command[0] == 'I'){
            insert(database);
        }
        else if(command[0] == 'P'){
            print(database, modes);
        }
        else if(command[0] == 'D'){
            
        }
        else if(command[0] == 'G'){
            
        }
        else if(command[0] == 'J'){
            
        }
        else{
            cerr << "Error: unrecognized command\n";
        }

        //create
        //remove
        //insert
        //print all
        //delete rows
        //print where
        //generate index
        //join
         
    } while(command !="QUIT");

    string tmp;
    getline(cin, tmp);
    cout << tmp;

    cout << "Thanks for being silly!\n";
    
    return 0;
}

void create(unordered_map<string, Table*> &database){
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

        Table* table = new Table(name, colTypes, colNames, colIndex);
        database[table->name] = table;
        //emplace into unordered map
        //database.emplace(name, table);

        cout << "New table " << name << " with column(s) " 
                << colNamesTogether << "created\n";
        return;
    }
    else{
        //table already exists
        cerr << "Error during CREATE: Cannot create already existing table " << name << "\n";
        exit(1);
    }

}

void remove(unordered_map<string, Table*> &database){
    string name;
    cin >> name;

    auto it = database.find(name);
    if(it == database.end()){
        cerr << "Error during REMOVE: " << name << " does not name a table in the database\n";
        exit(1);
    }
    else{
        delete it->second;
        database.erase(name);
        cout << "Table " << name << " deleted\n";
        return;
    }
}

void insert(unordered_map<string, Table*> &database){
    string name, temp;
    cin >> temp >> name;
    

    auto it = database.find(name);
    if(it == database.end()){
        cerr << "Error during INSERT: " << name << " does not name a table in the database\n";
        exit(1);
    }
    else{
        auto & data = database.at(name)->data;
        auto & colTypes = database.at(name)->colTypes;
        size_t index = data.size();
        size_t added;
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
                        //cout << "string " << value << " ";
                        data[r].emplace_back(TableEntry(value));
                        break;
                    }
                    case EntryType::Bool:{
                        bool value;
                        cin >> boolalpha >> value;
                        //bool b = value[0]=='t'? true : false;
                        // cout << "bool: "<< b << " ";
                        TableEntry te(value);
                        data[r].emplace_back(te);
                        break;
                    }
                    case EntryType::Int:{
                        int value; 
                        cin >> value;
                        // cout << "int " << value << " ";
                        TableEntry te(value);
                        data[r].emplace_back(te);
                        break;
                    }
                    default:{
                        double value;
                        cin >> value;
                        // cout << "double " << value << " ";
                        TableEntry te(value);
                        data[r].emplace_back(te);
                        break;
                    }
                }
            }
            //cout << "\n";
        }

        cout << "Added " << added << " rows to " << name << " from position " << index << " to " << (index+added-1) << "\n";        
        // auto size = database.at(name).data.size();
        // cout << size << "is the size\n";
    }
    //cout << "done\n";
    return;
}

void print(unordered_map<string, Table*> &database, Modes & modes){
    string temp, name;
    cin >> temp >> name;

    auto it = database.find(name);
    auto size = database.at(name)->colNames.size();
    if(it == database.end()){
        cerr << "Error during PRINT: " << name << " does not name a table in the database\n";
        exit(1);
    }
    else{
        int numCols;
        cin >> numCols;
        //cout << size << " is the size \n";
        
        vector<int> colPrintFlag(size, false);

        //cout << boolalpha << colPrintFlag[0];
        
        //cols.resize(size);
        string currCol;
        unordered_map<string, int> &colIndex =  database.at(name)->colIndex;
        //auto & colNames = database.at(name).colNames;

        // string all;
        // getline(cin, all);
        // cout << all << "\n";
        // cout << numCols << "\n";
        // return;

        //for the number of columns, read in each column and set to true for printing
        for(int i = 0; i<numCols; i++){
            cin >> currCol;
                        
            auto itCol = colIndex.find(currCol);
            if(itCol == colIndex.end()){
                cerr << "Error during PRINT: " << currCol << " does not name a column in " << name << "\n";
                exit(1);
            }
            else{
                //cout << "setting\n";

                //cols[i] = col;
                // cout << "curr col is " << currCol << "\n";
                // cout << colIndex[currCol] << " is col index\n ";
                colPrintFlag[i] = colIndex[currCol];
            } 
        }

        string cmd;
        cin >> cmd;

        if(cmd[0] == 'W'){
            //where colname OP value
            string tmp;
            getline(cin, tmp);
        }
        else{
            //print all 

            if(!modes.quiet){
                auto & data = database.at(name)->data;
                auto & colNames = database.at(name)->colNames;
                
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
            cout << "Printed " << database.at(name)->data.size() << " matching rows from " << name << "\n";
        }
    }
}