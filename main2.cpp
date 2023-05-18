// main.cpp
// links with sdisk.cpp, filesys.cpp, and block.cpp
// Montana Williams

// You can use this to test your Sdisk class 

//#include "sdisk.h"
#include "filesys.h"
//#include "block.cpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
//using namespace Filesys;
// You can use this to test your Filesys class 

vector<string> block(string buffer, int b); // prototype of block

// You can use this to test your Filesys class 

int main() // main
{
  Sdisk disk1("disk1",256,128);
  Filesys fsys("disk1",256,128);
  fsys.newfile("file1");
  fsys.newfile("file2");

  string bfile1;
  string bfile2;

  for (int i=1; i<=1024; i++)
     {
       bfile1+="1";
     }

  vector<string> blocks=block(bfile1,128); 

  int blocknumber=0;

  for (int i=0; i< blocks.size(); i++)
     {
       blocknumber=fsys.addblock("file1",blocks[i]);
     }

  fsys.delblock("file1",fsys.getfirstblock("file1"));

  for (int i=1; i<=2048; i++)
     {
       bfile2+="2";
     }

  blocks=block(bfile2,128); 

  for (int i=0; i< blocks.size(); i++)
     {
       blocknumber=fsys.addblock("file2",blocks[i]);
     }

  fsys.delblock("file2",blocknumber);

}

/*
int main() // for debugging
{
  Sdisk disk1("disk1",256,128); // makes an sdisk, a file called "disk1" since it doesn't exist yet
  Filesys fsys("disk1",256,128);  // Makes a filesys, also calls sdisk to make disk1, but already exists
  
  cout << "The first free block is at " << fsys.getfirstfreeblock() << endl;

  fsys.newfile("file1");  // makes a new file file1 on filesys, push_backed onto vector<int> filename

  cout << "Now the first free block is at " << fsys.getfirstfreeblock() << endl;
  cout << endl << "But " << fsys.getfirstfile() << " starts at " << fsys.getfirstblock(fsys.getfirstfile()) << endl;

  fsys.newfile("file2");  // makes a new file file2 on filesys, ''

  cout << endl;

  cout << "Now the first free block is at " << fsys.getfirstfreeblock() << endl;
  cout << endl << "But file2 starts at " << fsys.getfirstblock("file2") << endl;

  string bfile1; 
  string bfile2;

  for (int i = 1; i <= 1024; i++)
     {
       bfile1+="1";
     }

  //cout << "bfile1 is :" << bfile1 << endl; // 

  

  vector<string> blocks = block(bfile1,128); // should break bfile1 into chuncks 128 bits each

  //cout << "Blocks has " << blocks.size() << " many blocks.\n";
  //cout << "The first entry of Blocks is " << blocks[0].length() << endl;

  int blocknumber=0;

  //cout << "The first free block is at " << fsys.fat[0] << endl;

  for (int i=0; i< blocks.size(); i++)
     {
       cout << "\nI made it to the first loop! This is my " << i+1 << " time here! Going into Addblock()\n";
       //cout << "About to add blocks[i] = " << blocks[i] << " into file1\n"; 
       blocknumber=fsys.addblock("file1",blocks[i]); // go into addblock -> fssynch() -> 
     }


  cout << "I'm out of the loop!" << endl;

  cout << "Currently file1 starts at block " << fsys.getfirstblock("file1") << endl;

  cout << "Mind you the first free block is at " << fsys.getfirstfreeblock() << endl;

  fsys.delblock("file1",fsys.getfirstblock("file1"));


  cout << "I deleted a block! Now file 1 starts at " << fsys.getfirstblock("file1") << endl;


  for (int i=1; i<=2048; i++)
     {
       bfile2+="2";
     }

  cout << "I did another loop!" << endl;

  blocks=block(bfile2,128); 

  cout << "I blocked a file!" << endl;

  for (int i=0; i< blocks.size(); i++)
     {
       cout << "\nI made it here again! This is my " << i+1 << " time here! Going into Addblock()\n";
       blocknumber=fsys.addblock("file2",blocks[i]);
     }

  cout << "I'm out of the second loop! Going to delete block : " << blocknumber;

  fsys.delblock("file2",blocknumber);

  cout << "And I deleted the second file!\n";

  

  return 1;

}
*/