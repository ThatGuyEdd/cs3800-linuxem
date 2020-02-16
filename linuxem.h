#ifndef LINUXEM_H
#define LINUXEM_H

#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include<vector>
#include<ctime>
#include<chrono>

using namespace std;

//File class
class File
{
    private:
    string name;
    string path;
    string user;
    string timeStamp;
    string permissions;
    int fileSize;

    public:
    //Constructor
    File(string n);

    string getName() const{return name;}
    string getPath() const{return path;}
    string getUser() const{return user;}
    string getTime() const{return timeStamp;}
    string getPermissions() const{return permissions;}
    int getFileSize() const{return fileSize;}

    void setTime();
    void setPermissions(string p);
};

//Folder (directory) class
class Folder
{
    private:
    string name;
    string path;
    string user;
    string timeStamp;
    string permissions;
    int fileSize;

    Folder* parent;

    vector<Folder*> subFolders; //Vector of subdirectories
    vector<File> subFiles; //Vector of files in directory

    public:
    //Constructor and destructor
    Folder(): parent(NULL) {};
    Folder(string n, Folder &newP);
    ~Folder();

    Folder &operator=(const Folder &newF);

    string getName() const{return name;}
    string getPath() const{return path;}
    string getUser() const{return user;}
    string getTime() const{return timeStamp;}
    string getPermissions() const{return permissions;}
    int getFileSize() const{return fileSize;}

    Folder* getParent() const{return parent;}

    int getSize() const{return subFolders.size();}
    int getSubFilesSize() const{return subFiles.size();}
    vector<Folder*> getFolderV() const{return subFolders;}
    vector<File> getFileV() const{return subFiles;}

    void SetName(string n) {name = n;}
    void setParent(Folder* newP) {parent = newP;}
    void setTime();
    void setPermissions(string p);

    //Functions to run commands in current folder (directory)
    void ls();
    void ls_l();
    Folder* cd(string n, Folder* newP);
    void pwd();
    void mkdir(string n);
    void rmdir(string n);
    void rm(string n);
    void touch(string n);
    void chmod(string p, string n);

};

/////////////////////////////FOLDER FUNCTIONS/////////////////////////////

Folder::Folder(string n, Folder &newP)
{
    name = n;
    parent = &newP;
    
    if(parent != NULL)
        path = parent->getPath() + '/' + name;
    else
        path = name;
    
    setTime();
    user = "user";
    fileSize = 1024;
    permissions = "rwxrwxrwx";
}

Folder::~Folder()
{
    for(int i = 0; i < subFolders.size(); i++)
        delete subFolders[i];
}

Folder &Folder::operator=(const Folder &newF)
{
    name = newF.getName();
    path = newF.getPath();
    user = newF.getUser();
    timeStamp = newF.getTime();
    permissions = newF.getPermissions();
    fileSize = newF.getFileSize();

    return *this;
}

void Folder::setTime()
{
    std::time_t result = std::time(NULL);
    char *temp = ctime(&result);

    if(temp[strlen(temp) - 1] == '\n')
        temp[strlen(temp) - 1] = '\0';

    timeStamp = temp;
}

void Folder::setPermissions(string p)
{
    string tempP;

    for(int i = 0; i < p.length(); i++)
    {
        if(p[i] > 55 || p[i] < 48)
        {
            cout << "chmod: Invalid permission setting: " << p << endl;
            return;
        }
    }

    for(int i = 0; i < p.length(); i++)
    {
        if(p[i] == '0')
            tempP += "---";
        else if(p[i] == '1')
            tempP += "--x";
        else if(p[i] == '2')
            tempP += "-w-";
        else if(p[i] == '3')
            tempP += "-wx";
        else if(p[i] == '4')
            tempP += "r--";
        else if(p[i] == '5')
            tempP += "r-x";
        else if(p[i] == '6')
            tempP += "rw-";
        else if(p[i] == '7')
            tempP += "rwx";
    }

    permissions = tempP;
}

//Prints every file in current directory
void Folder::ls()
{
    for(int i = 0; i < subFolders.size(); i++)
        cout << subFolders[i]->getName() << '\t';
    for(int i = 0; i < subFiles.size(); i++)
        cout << subFiles[i].getName() << '\t';
    cout << endl;
}

//Prints every file in current directory in list form with details
void Folder::ls_l()
{
    for(int i = 0; i < subFolders.size(); i++)
    {
        cout << subFolders[i]->getPermissions() << '\t'
             << subFolders[i]->getUser() << '\t'
             << subFolders[i]->getFileSize() << '\t'
             << subFolders[i]->getTime() << '\t'
             << subFolders[i]->getName() << endl;
    }
    for(int i = 0; i < subFiles.size(); i++)
    {
        cout << subFiles[i].getPermissions() << '\t'
             << subFiles[i].getUser() << '\t'
             << subFiles[i].getFileSize() << '\t'
             << subFiles[i].getTime() << '\t'
             << subFiles[i].getName() << endl;
    }  
}

//Returns directory to become new current directory
Folder* Folder::cd(string n, Folder* newP)
{
    for(int i = 0; i < subFolders.size(); i++)
    {
        if(subFolders[i]->getName() == n)
            return subFolders[i];
    }
}

//Prints current working directory
void Folder::pwd()
{
    cout << getPath() + '/' << endl;
}

//Creates new directory (pushed onto vector)
void Folder::mkdir(string n)
{
    for(int i = 0; i < subFolders.size(); i++)
    {
        if(subFolders[i]->getName() == n)
        {
            cout << "mkdir: Error, " << n << " already exists" << endl;
            return;
        }
    }

    Folder* newF = new Folder(n, *this);
    subFolders.push_back(newF);
}

//Removes directory (removing from vector)
void Folder::rmdir(string n)
{
    for(int i = 0; i < subFolders.size(); i++)
    {
        if(subFolders[i]->getName() == n)
        {
            delete subFolders[i];
            subFolders.erase(subFolders.begin() + i);
        }
    }
}

//Removes file (removing from vector)
void Folder::rm(string n)
{
    for(int i = 0; i < subFiles.size(); i++)
    {
        if(subFiles[i].getName() == n)
            subFiles.erase(subFiles.begin() + i);
    }
}

//Creates new file (pushed onto vector)
void Folder::touch(string n)
{
    for(int i = 0; i < subFiles.size(); i++)
    {
        if(subFiles[i].getName() == n)
        {
            subFiles[i].setTime();
            return;
        }
    }
    File newF(n);
    subFiles.push_back(newF);
}

//Sets permissions for a directory or file
void Folder::chmod(string p, string n)
{
    for(int i = 0; i < subFolders.size(); i++)
    {
        if(subFolders[i]->getName() == n)
        {
            subFolders[i]->setPermissions(p);
            subFolders[i]->setTime();
        }
    }
    for(int i = 0; i < subFiles.size(); i++)
    {
        if(subFiles[i].getName() == n)
        {
            subFiles[i].setPermissions(p);
            subFiles[i].setTime();
        }
    }
}

//======================================================================//
//////////////////////////////FILE FUNCTIONS//////////////////////////////

File::File(string n)
{
    name = n;
    path = path + name + '/';
    user = "user";
    setTime();
    permissions = "rwxrwxrwx";
    fileSize = 1024;
}

void File::setTime()
{
    std::time_t result = std::time(NULL);
    char *temp = ctime(&result);

    if(temp[strlen(temp) - 1] == '\n')
        temp[strlen(temp) - 1] = '\0';

    timeStamp = temp;
}

void File::setPermissions(string p)
{
    string tempP;

    for(int i = 0; i < p.length(); i++)
    {
        if(p[i] > 55 || p[i] < 48)
        {
            cout << "chmod: Invalid permission setting: " << p << endl;
            return;
        }
    }

    for(int i = 0; i < p.length(); i++)
    {
        if(p[i] == '0')
            tempP += "---";
        else if(p[i] == '1')
            tempP += "--x";
        else if(p[i] == '2')
            tempP += "-w-";
        else if(p[i] == '3')
            tempP += "-wx";
        else if(p[i] == '4')
            tempP += "r--";
        else if(p[i] == '5')
            tempP += "r-x";
        else if(p[i] == '6')
            tempP += "rw-";
        else if(p[i] == '7')
            tempP += "rwx";
    }

    permissions = tempP;
}

#endif