#include <iostream>
#include <fstream>
#include "sdisk.h"
#include <vector>
#include <string>

using namespace std;

//initialize sdisk construstor
Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)
{
    this->diskname = diskname;
    this->numberofblocks = numberofblocks;
    this->blocksize = blocksize;
    fstream iofile; // Creating file we can write to
    iofile.open(diskname.c_str(), ios::in); // Specifying file name as diskname
    if(iofile.good())
    {
        cout <<"File exists" << endl;
    }
    else
    {
        cout << "File does not exist. Creating file " << diskname << " " << endl;
        iofile.open(diskname.c_str(), ios::out);
        for(int i = 0; i<(blocksize*numberofblocks); i++)
        {
            iofile.put('#');    // Fill it with # for all blocks
        }
    }
    iofile.close();
    return;
}


//getblock(int blocknumber, string& buffer);
int Sdisk::getblock(int blocknumber, string& buffer)
{
    fstream iofile;
    iofile.open(diskname.c_str(), ios::in | ios::out);
    buffer.clear();

    if(blocknumber > 0 && blocknumber < blocksize * numberofblocks)
    {
        char x; // the character we read from the file
        iofile.seekg(blocknumber * blocksize);
        for(int i = 0; i < blocksize; i++)
        {
            iofile.get(x); // get the characters from the file
            //cout << "The current character is " << x << endl;
            buffer.push_back(x);    // push the characters on the buffer
            //cout << buffer[i] << endl;
            //cout << "I  was called here! This is my " << i << " time here!\n";
        }
        iofile.close();
        return 1;
    }
    else 
    {
        iofile.close();
        return 0;
    }
}


//putblock(int blocknumber, string buffer);
int Sdisk::putblock(int blocknumber, string buffer)
{
    fstream outfile;
    outfile.open(diskname.c_str(), ios::in | ios::out);
    outfile.seekg(blocknumber*blocksize);
    if(outfile.good())
    {
        for(int i=0; i<buffer.length() && i < blocksize; i++)
        {
            outfile.put(buffer[i]);
        }
        outfile.close();
        return 1;
    }
    else
    {
        cout << "Failed to write to file " << diskname << " " << endl;
        return 0;
    }
}

//getnumberofblocks(); // accessor function
int Sdisk::getnumberofblocks()
{
    return Sdisk::numberofblocks;
}

//getblocksize(); // accessor function
int Sdisk::getblocksize()
{
    return Sdisk::blocksize;
}