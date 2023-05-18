#include <iostream>
//#include "sdisk.h"
#include "filesys.cpp"
//make #include "shell.h"
#include "table.h"
#include "block.h"
#include <sstream>
#include <fstream>
//#include <vector>
//#include <string>
//#include <string.h>

using namespace std;

Table::Table(string diskname,int numberofblocks,int blocksize, string flatfile, string indexfile):Filesys(diskname, numberofblocks, blocksize)
{
    this-> flatfile = flatfile;
    this-> indexfile = indexfile;
    newfile(flatfile);
    newfile(indexfile);
}

int Table::Build_Table(string input_file)
{
    ostringstream indexstream, flatstream;
    ifstream infile;
    infile.open(input_file.c_str());
    if (infile.bad())
    {
        return 0;
    }

    string s;
    getline(infile, s);
    while (infile.good())
    {
        //process line write the record to flatfile, add 

        string key = s.substr(05); // change key
        vector <string> blocks = block(s, getblocksize());
        int b = addblock(flatfile,blocks[0]);
    
        indexstream << key << " " << b << " ";
        numberofrecords++;

        getline(infile, s);
    }

    string buffer = indexstream.str();
    vector <string> blocks = block(buffer, getblocksize());
    for ( int i  = 0; i < blocks.size(); i++)
    {
        int b = addblock(indexfile,blocks[i]);
    }

    //process indexfile;

    return 1;
}

int Table::Search(string value)
{

    int b = IndexSearch(value);
    if (b <= 0)
    {
        return -1;
    }

    string buffer;
    b = readblock(flatfile, b, buffer);

    cout << buffer;
    return b;

}

int Table::IndexSearch(string value)
{
    istringstream instream;
    int b = getfirstblock(indexfile);
    if(b <= 0)
    {
        return 0;
    }

    string bigbuffer;

    while(b != 0)
    {
        string buffer;
        int error = readblock(indexfile, b, buffer);
        bigbuffer += buffer;
        b = nextblock(indexfile,b);
    }
    // index file is a string in bigbuffer;
    //process bigbuffer
    instream.str(bigbuffer);
    for(int i = 0; i < numberofrecords; i++)
    {
        string key;
        int block;
        instream >> key >> block;
        if (key == value)
        {
            return block;
        }

    }
    return -1 ;

}