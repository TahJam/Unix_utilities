//
// Created by Taher Rangoonwala on 4/3/21.
//
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]){
    int file_desc;

    // see if the user passed in an argument
    if (argc == 1) {
        // no files were passed and so we exit
        return 0;
    } else {
        // we go through the arguments and print the contents
        for (int i = 1; i < argc; i++) {
            file_desc = open(argv[i], O_RDONLY);
            if (file_desc == -1) { // bad file was given
                //cerr << "wcat: cannot open file " << argv[i] << endl;
                cout << "wcat: cannot open file" << endl;
                // this signifies an error
                return 1;
            }
            else{
                // traverse is used to determine how many bytes are needed to write
                int temp;
                int traverse = 1;
                char* dynBuffer; // to store the characters that have been read
                do{
                    // use dynamic memory to create new buffer for each character added by traverse
                    dynBuffer = new char[traverse];
                    temp = read(file_desc, dynBuffer, traverse);
                    write(STDOUT_FILENO, dynBuffer, temp);
                    // increment traverse to move on to the next character
                    traverse++;
                    // delete memory allocated to dynBuffer
                    delete [] dynBuffer;
                }while(temp > 0); // if read returns 0, then that means end of file has been reached so stop loop
                // Close the file
                close(file_desc);
            }
        }
    }
    return 0;
}
