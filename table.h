#ifndef TABLE_H
#define TABLE_H

#include "filesys.h"
//#include "shell.h"

class Table : public Filesys
{
public :
Table(string diskname,int numberofblocks,int blocksize, string flatfile, string indexfile);
int Build_Table(string input_file);
int Search(string value);
private :
string flatfile;  // from filesys
string indexfile;   // from filesys
int numberofrecords;
Filesys filesystem;  
int IndexSearch(string value); //find from index
};

#endif