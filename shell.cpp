#include <iostream>
#include "sdisk.h"
#include "filesys.h"
#include "shell.h"
#include "filesys.h"
#include "block.h"
#include <vector>
#include <string>

using namespace std;

// INCOMPLETE: Shell(),
vector<string> block(string buffer, int b); // prototype of block


Shell::Shell(string filename, int numberofblocks, int blocksize):Filesys(filename, numberofblocks, blocksize)
{
    //Filesys 
    cout << "Shell was created" << endl;

}

int Shell::dir()
{
    // implement error codes

    vector <string> flist=ls();
    for (int i=0; i<flist.size(); i++)
        {
            cout << flist[i] << endl;
        }
    return 1;
}

int Shell::add(string file, string buffer)
{
    // implement error codes
    int code = getfirstblock(file);
    if(code >= 0)
    {
        cout << "File exists";
        return 0;
    }
    code = newfile(file); //newfile(filename)
    if (code == 0)
    {
        cout << "Failed";
        return 0;
    }
    vector <string> blocks = block(buffer, getblocksize());
    for(int i = 0; i < blocks.size(); i++)
    {
        code = addblock(file,blocks[i]); //addblock(file,blocks[i])
        if(code == 0)
        {
            cout << "Error Occurred";
            return 0;
        }
    }
    return 1;
}

int Shell::del(string file)
{
    //implement error codes
    // delete all blocks
    int block = getfirstblock(file);    // first block of file
    while(block != 0)
    {
        int code = delblock(file,block);    // delete current block
        //block = nextblock(block);         // block now on free list, cannot find next block
        block = getfirstblock(file);        // 
    }
    rmfile(file);
    
    return 1;
}

int Shell::type(string file)
{
    // basically same as copy
    int block = getfirstblock(file);
    
    // check if file exists, block = -1 -> no file1

    if (block == -1)
    {
        cout << "File does not exist " << endl;
        return -1;
    }

    while(block != 0)
    {
        string buffer;
        int code = readblock(file,block,buffer);
        if(code == 0) 
        {
            cout << "Error Occurred";
            return 0;
        }

        // readblock() updates buffer, print buffer,
        cout << buffer << " ";
        
        block = nextblock(file,block);
    }
    cout << endl;
    return 1;
}

int Shell::read(string file)
{
    int block = getfirstblock(file);
    string buffer;
         if(block < 0)
         {
           cout << "no such file";
           return 0;
         }
         if(block == 0)
         {
             cout << "Empty file";
             return 1;
         }
         while (block != 0)
         {
             string b;
             readblock(file,block, b);
             buffer+= b;
             block = nextblock(file,block);
         }

    cout << buffer << endl;

    return 1;
}

int Shell::copy(string file1, string file2) 
{
    int block = getfirstblock(file1);
    // check if file exists, block = -1 -> no file1
    int code = newfile(file2);
    // check if file2 already exists or no room for file (code = 0)
    while(block != 0)
    {
        string buffer;
        readblock(file1,block,buffer);
        code = addblock(file2,buffer);
        if (code == 0) // no space
        {
            cout << "Error Occurred, no space";
            return 0;
        }
        //writeblock()
        block = nextblock(file1,block);
    }
    return 1;
}
