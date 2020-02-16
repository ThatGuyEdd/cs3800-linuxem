#include "linuxem.h"

int main()
{
    bool found; //Used to determine if file or folder is found when using commands
    
    //Used to determine what the current folder (directory) the user is in
    Folder currF; 
    currF.SetName("/");
    Folder* currFPtr = &currF;

    //Loop that continually runs the simulation unless user exits
    while(true)
    {
        string command, c1, c2, c3; //Command string and possible queries
        
        cout << "user@hostname:~" << currFPtr->getPath() << "$ ";
        getline(cin, command);
        stringstream ss(command);
        found = false;

        ss >> c1 >> c2 >> c3;
        
        //Various if statements to determine what command (if valid) is input 
        //and to run command in current directory; error statements for invalid
        //queries included.
        if(c1 == "ls")
        {
            if(c2 == "-l")
                currFPtr->ls_l();
            else
                currFPtr->ls();
        }
        else if(c1 == "cd")
        {
            if(c2 == "..")
            {
                if(currFPtr->getParent() == NULL)
                    found = true;
            
                else
                {
                    if(currFPtr->getParent() != NULL)
                    {
                        currFPtr = currFPtr->getParent();
                        found = true;
                    }
                }
            }
            else
            {
                for(int i = 0; i < currFPtr->getSize(); i++)
                {
                    if(c2 == currFPtr->getFolderV()[i]->getName())
                    {
                        currFPtr = currFPtr->cd(c2, currFPtr);
                        found = true;
                    }
                }
            }
            if(found == false)
                cout << "cd: " << c2 << " no such directory" << endl;
        }
        else if(c1 == "pwd")
        {
            currFPtr->pwd();
        }
        else if(c1 == "mkdir")
        {
            currFPtr->mkdir(c2);
        }
        else if(c1 == "rmdir")
        {
            for(int i = 0; i < currFPtr->getSize(); i++)
            {
                if(currFPtr->getFolderV()[i]->getName() == c2)
                {
                    currFPtr->rmdir(c2);
                    found = true;
                }
            }
            if(found == false)
                cout << "rmdir: " << c2 << " no such directory" << endl;
        }
        else if(c1 == "rm")
        {
            found = false;
            for(int i = 0; i < currFPtr->getSize(); i++)
            {
                if(currFPtr->getFileV()[i].getName() == c2)
                {
                    currFPtr->rm(c2);
                    found = true;
                }
            }
            if(found == false)
                cout << "rm: " << c2 << " no such file" << endl;
        }
        else if(c1 == "chmod")
        {
            for(int i = 0; i < currFPtr->getSize(); i++)
            {
                if(c3.empty() == false)
                {
                    if(currFPtr->getFolderV()[i]->getName() == c3)
                    {
                        currFPtr->chmod(c2, c3);
                        found = true;
                    }
                }
            }
            for(int i = 0; i < currFPtr->getSize(); i++)
            {
                if(c3.empty() == false)
                {
                    if(currFPtr->getFileV()[i].getName() == c3)
                    {
                        currFPtr->chmod(c2, c3);
                        found = true;
                    }
                }
            }
            if(found == false && c3.empty() == false)
                cout << "chmod: " << c3 << " no such directory" << endl;
            else if(c3.empty() == true)
                cout << "chmod: Expects 2 arguments" << endl;
        }
        else if(c1 == "touch")
        {
            currFPtr->touch(c2);
        }
        else if(c1 == "exit" || c1 == "quit")
            break;
        else
            cout << "Invalid command: " << c1 << endl;
    }

    return 0;
}
