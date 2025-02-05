# SimplifiedDES

## Description

A simplified version of DES encryption made for my cyber security class. It uses an input file and a hex key to encrypt the file and output both the output and the log files.

## Running

Included in the files is a make file that will compile the simple_des.cc.

When running the executable, you need to include what the key is in the format: 0xYYY. You then need to set the input file, output file, and log files, such as the following: ```./simple_des 0x36C < test_files/input.txt > out.txt 2> log.txt```

## File Structure

The `test_files` folder contains a sample input, as well as a sample output and log files for running the command ```./simple_des 0x36C < test_files/input.txt > out.txt 2> log.txt```