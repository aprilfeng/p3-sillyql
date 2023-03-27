// Project identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <cstdint>
#include <getopt.h>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "TableEntry.h"

using namespace std;

struct Modes {
    bool help = false;
    bool quiet = false;
}; //modes structure

void getMode(int argc, char * argv[], Modes &modes);


class Table{
  public:
    string name;
    vector<EntryType> colTypes;
    vector<string> colNames;
    unordered_map<string, int> colIndex;
    vector<vector<TableEntry>> data;
    
    Table() {}
    Table(string name, vector<EntryType> &cT, vector<string> &cN, unordered_map<string, int> &cI) : 
            name(name), colTypes(cT), colNames(cN), colIndex(cI){
      //data.resize(0, vector<TableEntry>(0));
    }

    
    void printAll();
    void printWhere();
    void generateIndex();
    void join();

    //table name
    //column names (with order)
    //column types (with order)
    //k
};

void create(unordered_map<string, Table*> &);
void remove(unordered_map<string, Table*> &);
void insert(unordered_map<string, Table*> &database);
void print(unordered_map<string, Table*> &database, Modes & modes);
void generate(unordered_map<string, Table*> &database);
void join(unordered_map<string, Table*> &database);


/*

INSERT

should be a 2D vector

vector[row num][col num]: table entry

table entry is a variant type

if table doesn't exist, parse out

if table exists:
resize vector
loop over columns for num of rows
add table entry containing that type to the vector

push_back vs emplace_back

for every column, read type, emplace back that type


*/

/*

PRINT ALL


*/

/*

Delete

use stl algorithm remove_if() using functor
functor needs to have where to look + 

4 way split to know what to read (type)
then helper to 3 way split for the comparison

needs to be templated (type doesn't matter)
look at set union helper funciton (template format)

3 functors:
1 for greater than, less than, equal

index sort: functor need sa constructor for member variables


Print where: 

4 way split of type ()

print where: what is column name, what is type? does it exist?
read variable of that type. call helper for table entry

needs to be same: helper function can do comparison (< > =)
use functor for comparison


print where:

after all of it, generate index

*/

/*
generate index

bst - for < > =
hash - for = only

if there is an index, and its helpful, edit other functions


*/

/*

  

*/