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
    int temp;
    int howMany; // how many characters in a row
    char whichChar;
    char *dynBuffer; // to store the characters that need to be written out
    // use if statements to see if any files have been passed through
    if(argc == 1){
        cout << "wunzip: file1 [file2 ...]" << endl;
        return 1;
    }
    else{
        // go through the files using for loop
        for(int i = 1; i < argc; i++){
            file_desc = open(argv[i], O_RDONLY);
            if (file_desc == -1) { // bad file was given
                cout << "wunzip: cannot open file" << endl;
                // this signifies an error
                return 1;
            }
            else{
                do {
                    // format of file is 4 bytes int 1 byte char
                    // read 4 bytes int
                    temp = read(file_desc,&howMany,sizeof(int));
                    //cout << "how many is " << howMany << " and temp is " << temp << endl;
                    // read 1 byte char and use temp to check if program has reached EOF
                    temp = read(file_desc,&whichChar,sizeof(char));
                    //cout << "whichChar is " << whichChar << " and temp is " << temp << endl;
                    if(temp != 0){
                        // create the buffer of size howMany and populate with whichChar
                        dynBuffer = new char[howMany];
                        for(int j = 0; j < howMany;j++){
                            dynBuffer[j] = whichChar;
                        }
                        // write out the buffer
                        write(STDOUT_FILENO,dynBuffer,howMany);
                        // delete memory allocated to buffer
                        delete [] dynBuffer;
                    }
                }while (temp > 0);
                // close the file
                close(file_desc);
            }
        }
    }
    return 0;
}

