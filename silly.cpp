// Project identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <cstdint>
#include <getopt.h>
#include <string>
#include <sys/types.h>
#include <type_traits>
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
    unordered_map<string, Table> database;

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
            print(database);
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

void create(unordered_map<string, Table> &database){
    string name;
    cin >> name;

    auto it = database.find(name);
    if(it == database.end()){
        //create table and add to database
        int numCols;
        cin >> numCols;
        vector<EntryType> colTypes;
        colTypes.reserve(numCols);
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
            
        }

        Table table{name, colTypes, colNames};
        //emplace into unordered map
        database.emplace(name, table);

        cout << "New table " << name << " with column(s) " << colNamesTogether << "created\n";
        return;
    }
    else{
        //table already exists
        cerr << "Error during CREATE: Cannot create already existing table " << name << "\n";
        exit(1);
        return;
    }
}

void remove(unordered_map<string, Table> &database){
    string name;
    cin >> name;

    auto it = database.find(name);
    if(it == database.end()){
        cerr << "Error during REMOVE: " << name << " does not name a table in the database\n";
        exit(1);
        return;
    }
    else{
        database.erase(name);
        cout << "Table " << name << " deleted\n";
        return;
    }
}

void insert(unordered_map<string, Table> &database){
    string name, temp;
    cin >> temp >> name;
    

    auto it = database.find(name);
    if(it == database.end()){
        cerr << "Error during INSERT: " << name << " does not name a table in the database\n";
        exit(1);
        return;
    }
    else{
        auto & data = database.at(name).data;
        auto & colTypes = database.at(name).colTypes;
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
                        TableEntry te(value);
                        data[r].emplace_back(te);
                        break;
                    }
                    case EntryType::Bool:{
                        string value;
                        cin >> value;
                        bool b = value[0]=='t'? true : false;
                        // cout << "bool: "<< b << " ";
                        TableEntry te(b);
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

        cout << "Added " << added << " rows to " << name << " from position " << index << " to " << (index+added) << "\n";        
    }
    //cout << "done\n";
    return;
}

void print(unordered_map<string, Table> &database){
    string temp, name;
    cin >> temp >> name;

    auto it = database.find(name);
    if(it == database.end()){

        cerr << "Error during PRINT: " << name << " does not name a table in the database\n";
        exit(1);
        return;

    }
    else{
        int numCols;
        cin >> numCols;
        
        vector<string> cols;
        cols.resize(numCols);
        string col;
        for(int i = 0; i<numCols; i++){
            cin >> col;
            // if(find(database.at(name).colNames.begin(), database.at(name).colNames.end(), col)==database.at(name).colNames.end()){
            //     cerr << "Error during INSERT: " << col << " does not name a column in " << name << "\n";
            // }
            cols[i] = col;
        }

        string cmd;
        cin >> cmd;

        if(cmd[0] == 'W'){
            //where colname OP value
        }
        else{
            //print all 

            auto & data = database.at(name).data;
            

            for(size_t r = 0; r<data.size(); r++){
                for(size_t c = 0; c<data[r].size(); c++){
                    //if(databas
                }
            }
        }
    }
}