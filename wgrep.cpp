//
// Created by Taher Rangoonwala on 4/6/21.
//
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    int file_desc;
    int max = 100000;
    int blockSize = 1; // blockSize is used to determine how many bytes are needed to write
    int bigNum; // will be blockSize * max. for increasing read block size
    int total = 0; // how many bytes have been read in total
    int temp; // will be how bytes have been read so far
    char* dynBuffer; // to store the characters that have been read
    string bigString; // to store the characters read as a string for easier manipulation

    // use if statements to see if a search term and any files have been passed through
    if(argc == 1){ // no search term has been given. return 1
        cout << "wgrep: searchterm [file ...]" << endl;
        return 1;
    }
    else if(argc == 2){ // only search term has been given no file, search from standard input
        string search = argv[1];
        file_desc = STDIN_FILENO;
        do{
            // use dynamic memory to create new buffer for each character added by blockSize
            dynBuffer = new char[blockSize];
            temp = read(file_desc, dynBuffer, blockSize);
            // use dynBuffer array and concatenate it to a single string
            string str = dynBuffer;
            bigString.append(str);
            // increment blockSize to move on to the next character
            blockSize++;
            // delete memory allocated to dynBuffer
            delete [] dynBuffer;
        }while(temp > 0); // if read returns 0, then that means end of file has been reached so stop loop
        // create substrings with \n as delimiter then search the substrings for the search term
        for(unsigned int i = 0; i < bigString.length();){
            // find the \n
            int pos = bigString.find("\n");
            // create the substring
            string tempStr = bigString.substr(i, pos);
            // cut the part that was used in bigString out
            bigString = bigString.substr(pos+1,bigString.size()-1);
            // check tempStr for search term. if it has it, then print it
            if(tempStr.find(search) != string :: npos){
                // copy string to char array
                dynBuffer = new char[tempStr.length()+1];
                for(unsigned int h=0;h<tempStr.length();h++){
                    dynBuffer[h] = tempStr[h];
                }
                dynBuffer[tempStr.length()] = '\n';
                write(STDOUT_FILENO,dynBuffer,tempStr.length()+1);
                // delete buffer
                delete [] dynBuffer;
            }
        }
    }
    else{ // 1 or more files were given so go through all files, search, then write to STD_OUT idk if that's what its called
        for (int i = 2; i < argc; i++) {
            string search = argv[1];
            file_desc = open(argv[i], O_RDONLY);
            if (file_desc == -1) { // bad file was given
                //cerr << "wgrep: cannot open file " << argv[i] << endl;
                cout << "wgrep: cannot open file" << endl;
                // this signifies an error
                return 1;
            }
            else{
                do{
                    // use dynamic memory to create new buffer
                    bigNum = blockSize * max;
                    dynBuffer = new char[bigNum];
                    temp = read(file_desc, dynBuffer, bigNum);
                    // add temp to total amount of bytes that have been read
                    total += temp;
                    // increment blockSize to increase new block size
                    blockSize++;
                    // append all the characters read into bigString
                    bigString = dynBuffer;
                    // delete buffer
                    delete [] dynBuffer;
                    // create substrings with \n as delimiter then search the substrings for the search term
                    for(unsigned int j = 0; j < bigString.length();){
                        // find the \n
                        int pos = bigString.find("\n");
                        // create the substring and put it into a char array character by character
                        string strTemp = bigString.substr(j, pos);
                        // cut the part that was used in bigString out
                        bigString = bigString.substr(pos+1,bigString.length()-1);
                        if(strTemp.find(search) != string :: npos){
                            // copy string to char array and for last element in array, add newline 
                            dynBuffer = new char[strTemp.length()+1];
                            for(unsigned int h=0;h<strTemp.length();h++){
                                dynBuffer[h] = strTemp[h];
                            }
                            dynBuffer[strTemp.length()] = '\n';
                            write(STDOUT_FILENO,dynBuffer,strTemp.length()+1);
                            // delete buffer
                            delete [] dynBuffer;
                        }
                    }
                }while(temp > 0); // if read returns 0, then that means end of file has been reached so stop loop
                // close the current file
                close(file_desc);
            }
        }
    }
    return 0;
}
