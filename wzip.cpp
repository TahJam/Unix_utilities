//
// Created by Taher Rangoonwala on 4/7/21.
//
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    int file_desc;
    int temp; // will be how bytes have been read in current iteration
    int total = 0; // how many bytes have been read in total
    int traverse = 1; // traverse is used to determine how many bytes are needed to write
    char* dynBuffer; // to store the characters that have been read
    string bigString; // to store the characters read as a string for easier manipulation
    // use if statements to see if any files have been passed through
    if(argc == 1){
        cout << "wzip: file1 [file2 ...]" << endl;
        return 1;
    }
    else{
        // go through the files using for loop
        for(int i = 1; i < argc; i++){
            file_desc = open(argv[i], O_RDONLY);
            if (file_desc == -1) { // bad file was given
                cout << "wzip: cannot open file" << endl;
                // this signifies an error
                return 1;
            }
            else{ // file is valid and so we can do the stuff
                do{
                    // use dynamic memory to create new buffer for each character added by traverse
                    dynBuffer = new char[traverse];
                    temp = read(file_desc, dynBuffer, traverse);
                    // add temp to total amount of bytes that have been read
                    total+=temp;
                    // delete memory allocated to dynBuffer
                    delete [] dynBuffer;
                }while(temp > 0); // if read returns 0, then that means end of file has been reached so stop loop
                // create fresh buffer to store the total bytes that have been read
                dynBuffer = new char[total];
                // close and then reopen file because file_desc has been moved
                close(file_desc);
                file_desc = open(argv[i],O_RDONLY);
                read(file_desc, dynBuffer, total);
                bigString.append(dynBuffer);
                // delete memory allocated to dynBuffer
                delete [] dynBuffer;
                // close the current file
                close(file_desc);
            }
        }
        // after all files have been read and everything is in bigString, program will now compress
        // go through bigString using for loop and count occurrence with while loop
        for (unsigned int whereInBigStr = 0; whereInBigStr < bigString.length(); whereInBigStr++) {
            // Count occurrences of current character
            int count = 1;
            while (whereInBigStr < bigString.length() - 1 && bigString[whereInBigStr] == bigString[whereInBigStr + 1]){
                count++;
                whereInBigStr++;
            }
            // write the integer in binary format
            write(STDOUT_FILENO,&count,sizeof(int));
            // write the current character
            char my_char[1] = {bigString[whereInBigStr]};
            write(STDOUT_FILENO, my_char, sizeof(char));
        }
    }

    return 0;
}

