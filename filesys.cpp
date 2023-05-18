#include <iostream>
#include "sdisk.cpp"
#include "filesys.h"
#include "block.h"
#include <sstream>
#include <vector>
#include <string>

// INCOMPLETE : fsclose(), rmfile()

using namespace std;

//vector<string> block(string buffer, int b); // prototype of block

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname,numberofblocks, blocksize)
{
    //Sdisk(diskname, numberofblocks, blocksize);
    rootsize = getblocksize()/12; // 128/12 = 10
    fatsize = getnumberofblocks() * 6 / getblocksize() + 1; // 256*6/128+1 =  13 

    string buffer;
    int code = getblock(1,buffer);
    if (code == 0)
    {
        cout << "An error occurred\n";
        return;
    }
    if (buffer[0] == '#')   // first block is zero
    {
        cout << "Building file system\n";
        buildfs();
        
        //cout << " with rootsize = " << rootsize << " and fatsize = " << fatsize << endl;
        return;
    }
    else
    {
        cout << "Reading file system\n";
        readfs();
        return;
    }
    fssynch();
}

/*
vector<string> Filesys::block(string buffer, int b)
{
// blocks the buffer into a list of blocks of size b

vector<string> blocks;
int numberofblocks=0;

if (buffer.length() % b == 0) 
   { numberofblocks= buffer.length()/b;
   }
else 
   { numberofblocks= buffer.length()/b +1;
   }

string tempblock;
for (int i=0; i<numberofblocks; i++)
    { tempblock=buffer.substr(b*i,b);
      blocks.push_back(tempblock);
    }

int lastblock=blocks.size()-1;

for (int i=blocks[lastblock].length(); i<b; i++)
    { blocks[lastblock]+="#";
    }

return blocks;

}
*/

int Filesys::fssynch()
{
    //write root and fat to disk
    //cout << "Calling fssynch" << endl;
    // For ROOT
    ostringstream ostream;
    for(int i =0 ; i < rootsize; i++)
    {
        ostream << filename[i] << " " << firstblock[i] << " ";
    }
    string buffer1 = ostream.str();
    vector <string> blocks1 = block(buffer1, getblocksize());
    putblock(1, blocks1[0]); // starting at 1
    
    // For FAT
    ostringstream ostream2;
    for(int i = 0; i < fat.size(); i++)
    {
        ostream2 << fat[i] << " ";
    }
    string buffer2 = ostream2.str();
    vector <string> blocks2 = block(buffer2, getblocksize());
    
    for( int i = 0; i < blocks2.size(); i++) 
    {
        putblock(2+i, blocks2[i]);     
    }
    
    return 1;
}

string Filesys::getfirstfile()
{
    return filename[0];
}

int Filesys::readfs()
{
    // Ver 1

    //For Root
    istringstream istream;
    string buffer;
    getblock(1,buffer);
    istream.str(buffer);
    for(int i = 0; i < rootsize; i++)
    {
        string s; int t;
        istream >> s;
        istream >> t;
        filename.push_back(s);
        firstblock.push_back(t);
    }


    // For FAT
    istringstream istream2;
    string buffer2;
    for (int i = 0; i < fatsize; i++)
    {
        string b;
        getblock(i+2, b);
        buffer2 += b;
        
    }
    istream2.str(buffer2);
    for(int i = 0; i < getnumberofblocks(); i++)
    {
        int t;
        istream2 >> t;
        fat.push_back(t);
    }
    return 1;
    

    // Ver 2
    /*
    string buffer1, buffer2;
    getblock(1,buffer1); 
    getblock(1,buffer2);
    string tempbuffer;
    for (int i = 0; i < fatsize; i++)
    {
        getblock(2+i, tempbuffer);
        buffer2 += tempbuffer;
        tempbuffer.clear();
    }

    istringstream istream1, istream2;
    istream1.str(buffer1);
    istream2.str(buffer2);
    for(int i = 0; i < rootsize; i++)
    {
        string f;
        int n;
        istream1 << f << n;
        filename.push_back(f);
        firstblock.push_back(n);
    }


    int k;
    for(int i = 0; i < getnumberofblocks(); i++)
    {
        //istream2 << 
        fat.push_back(k);
    }
    */
    //return 1;
}

int Filesys::fsclose()
{
    return 0;
}

int Filesys::buildfs()
{
    //build root
    
    ostringstream ostream;
    for(int i = 0; i < rootsize; i++)
    {
        filename.push_back("XXXXX");    // File name available
        firstblock.push_back(0);    // filled with zero
        ostream << "XXXXX" << " " << 0 << " ";
    }
    
    string buffer = ostream.str();
    vector <string> blocks = block(buffer, getblocksize());
    putblock(1,blocks[0]);


    //build FAT
    ostringstream ostream2;
    fat.push_back(fatsize + 2); // first free block is at fatsize + 2 
                                // in this case 13 + 2 = 15
    //ostream2 << fatsize + 2 << " ";
    fat.push_back(-1); // second block 
    //ostream2 << -1 << " ";
    for(int i = 0; i < fatsize; i++) //fatsize = 13
    {
        fat.push_back(-1); //fill them all with empty blocks 
        //ostream2 << -1 << " ";
    }
    //for(int i = fatsize + 2; i < numberofblocks; i++)
    //for(int i = fatsize + 2; i < fat.size(); i++)
    for(int i = fatsize + 2; i < getnumberofblocks(); i++) 
    {
        fat.push_back(i+1);
        // // ostream2 << i+1 << " ";
    }
    //last block must be 0
    fat[fat.size()-1] = 0;

    for(int i = 0; i < fat.size(); i++)
    {
        ostream2 << fat[i] << " ";
    }
    string buffer2 = ostream2.str();
    vector <string> blocks2 = block(buffer2,getblocksize());
    for (int i = 0; i < blocks2.size(); i++)
    {
        putblock(2+i, blocks2[i]);
    }
    
    return fssynch();
}

int Filesys::newfile(string newname)
{
    
//    for(int i = 0; i < filename.size(); i++)
//    {
//        if(filename[i] == file)
//            cout << "file exists";
//        return 0
//    }

    for (int i = 0; i < filename.size(); i++)  // Check if file exists across entire root
    {
        if (filename[i] == newname)
        {
            cout << "file exists" << endl;
            return 0;
        }
    }
    // else
    cout << "Creating file " << newname << endl;

    for (int i = 0; i < rootsize; i++)
    {
        if (filename[i] == "XXXXX")
        {
            filename[i] = newname;
            firstblock[i] = 0;
            //firstblock[i] = fat[0]; // there is no data in file yet, points to fat[0], first free space
                                    // pointer is literally at 15
            //fat[0] =  fat[fat[0]]; // first free block is at the sequencial free block
            //cout << "The first free block is available at " << getfirstfreeblock() << " OR at " << fat[0] << endl;
            //cout << "I wrote over firstblock[i] " << firstblock[i] << endl;
            //fat[firstblock[i]] = 0; // 
            //cout << "The data in the file should be 0 : " << fat[firstblock[i]] << endl;
            
            // Now t
            //fat[0] = 
            fssynch();
            return 1;
        }
    }
    cout << "no space in Root" << endl;
    return 0;
}

int Filesys::rmfile(string file)
{
    for(int i = 0; i < filename.size(); i++)
    {
        if(filename[i] == file && firstblock[i] == 0)
        {
            filename[i] = "XXXXX";
            firstblock[i] = 0;
            fssynch();
            return 1;
        }
    }

	return 1;
}

int Filesys::getfirstblock(string file)
{
    // return first block number of file
    // returns 0 if file is empty
    // returns -1 if file doesn't exist
    for (int i = 0; i < rootsize; i++)
    {
        if(filename[i]==file)
        {
            return firstblock[i];
        }
    }
    cout << "No file exists" << endl;
    return -1;
}

bool Filesys::checkblock(string file, int block)
{
    // check if block is in file
    int blockid = getfirstblock(file);
    if (blockid == -1) // Doesn't exist
    {
        return false;
    }
    while(blockid != 0) // check the entire fat
    {
        //if(fat[blockid] == block) // <- bad way to do it
        if(blockid == block)
        {
            return true;
        }
        blockid = fat[blockid]; // go to next block number in FAT
    }
    return false; //otherwise
}

// TEMPORARY FUNCTION
int Filesys::getfirstfreeblock()
{
    return fat[0];
}

int Filesys::addblock(string file, string block)
{
    // check if File exists
    // allocate block
    // update root or fat
    int first = getfirstblock(file);  // WE know where in fat the file starts at
    if (first == -1) // File doesn't exist
    {
        cout << "File does not exist" << endl;
        return 0;
    }

    int allocate = fat[0]; // first free block


    //cout << "File Exists\n"; 

    if (allocate == 0) // No space on disk
    {
        //cout << "No space on disk" << endl;
        return 0;
    }

    //cout << "Allocating space. Using first free block at block " <<  allocate << endl;

    //cout << "My fat[0] is " << fat[0] << endl;
    fat[0] = fat[fat[0]];  // go to next free block
    fat[allocate] = 0;  // the firstblock should now 

    //cout << "First = " << first << endl;

    if (first == 0)   // no blocks in file ; update root directory
    {
        //for(int i = 0; i < filename.size(); i++) // go down every file in filename
        for(int i = 0; i < rootsize; i++) // go down every file in filename
        {
            if (filename[i] == file) // if we found the file
            {

                firstblock[i] = allocate; //filename[i] finds the file, at same index i // 17
                //fat[0] = fat[allocate]; // 
                //fat[allocate] = 0;
                //break;
            }
        }
    }

    else    // blocks in file
    {
        int b = first; //first = getfirstblock(file)
        while (fat[b] != 0) // follow the fat until we get to 0
        {
            //cout << " Following chain. Following blocks of file 1. At block " << fat[b] << endl;
            b = fat[b]; //go to next block
            //fat[b] = b; //
            //fat[0] = fat[allocate];
            //fat[allocate] = 0;
            
        }
        //cout << "The end of the file is at block " << b << endl; 
        fat[b] = allocate;
    }
    
    /*

    int value = getfirstblock(file);
    //cout << "\nALL THE BLOCKS OF FILE " << file << " ARE BLOCKS: ";
    while (fat[value] != 0)
    {
        cout << value << " ";
        value = fat[value];
    }
    */

    //cout << "\nSo the last block of file " << file << " is at block " << fat[value] << endl;
    

    putblock(allocate,block); // write over the 0
    fssynch(); // update root and fat
    
    //cout << 

    //cout << "First free block is now at " << getfirstfreeblock() << endl;
    //cout << "Everything worked out okay!" << endl;
    return allocate;
}

int Filesys::delblock(string file, int blocknumber)
{
    
    int b = getfirstblock(file); 

    if(!checkblock(file, blocknumber)) // if it returns false, or we cannot find the file
    {
        cout << "File " << file << " does not exist.\n";
        return 0;
    }



    //if(checkblock(file, blocknumber)) //If it is the first block of the file
    if(blocknumber == b)
    {
        for(int i = 0; i < filename.size(); i++)
        {
            if(filename[i]==file)
            {
                //cout << "Found file " << file << ". Deleting block." << endl;
                firstblock[i] = fat[blocknumber];

            }
        }
    }




    else // remove in middle of chain; del 6 when fat[5] = 6, fat[6] = 7; fat[5] -> 7
    {
        int current = 0;
        while(fat[b] != blocknumber)
        {
            b = fat[b]; // find it
            //cout << "Trying to delete block. On try # " << current << " and fat is also of size " << fat.size() << endl;
            current++;
        }
        fat[b] = fat[blocknumber];
    }

    // update free list
    fat[blocknumber] = fat[0];
    fat[0] = blocknumber;

    fssynch();

    return 1;
}

int Filesys::readblock(string file, int blocknumber, string& buffer) // maybe string& buffer instead
{
    if(checkblock(file,blocknumber)) // If block exists
    {
        //putblock()
        getblock(blocknumber,buffer);   // read block into buffer, by reference
        return 1;
    }
    else
    {
        return 0;
    }
}

int Filesys::writeblock(string file, int blocknumber, string buffer)    //maybe string& buffer instead
{
    if(checkblock(file,blocknumber))
    {
        putblock(blocknumber,buffer);
        //getblock(blocknumber,buffer);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Filesys::nextblock(string file, int blocknumber)
{
    if(checkblock(file,blocknumber))
    {
        return fat[blocknumber]; //found next block of FAT
    }
    else
    {
        return -1;
    }
}

vector <string> Filesys::ls() // for shell later
{
    vector <string> flist;
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename[i] != "XXXXX")
        {
            flist.push_back(filename[i]);
        }
    }
    return flist;
}