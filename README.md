# SimplifiedDES

## Description

A simplified version of DES encryption made for my cyber security class. It uses an input file and a hex key to encrypt the file and output both the output and the log files.

When the program runs, it does the following steps:
1. **S-DES key generation**
    - Rearrange the key's input bits to the correct output bits
        - Shows which of the bits from the input go into the 10-bit output
        - 3, 5, 2, 7, 4, 10, 1, 9, 8, 6
        - Example: input bit at position 1 goes into the 7th spot in the output bits
    - Bit shift to the left on the first 5 bits then bit shift the right 5 bits independently
    - Take 8 bits out of the 10 total bits
        - Shows which of the bits from the input go into the 8-bit output (loses positions 1 and 2)
        - 6, 3, 7, 4, 8, 5, 10, 9
        - Example: input bit 3 goes into the 2nd spot in the output bits
    - Repeat the previous steps to get k2
        - Start with the 10 bits that you get after the bit shift
        - Left shift the two halves again
        - Repeat the bit selection with the shifted bits
        - This gives you k2

2. **Initial Permutation**
    - Switch bits in the following order 
        - 2, 6, 3, 1, 4, 8, 5, 7
        - Example: input bit in position 1 becomes the output bit in position 4

3. **The Fk function**
    - Take the left and right sides of the 8-bit input separately as 2 inputs of 4 bits
    - From the right side 4 bits, expand and permutate in the following matrix
        - 4, 1, |2, 3
        - 2, 3, | 4, 1
    - Take the 8-bit key and do exclusive-or on each of the bits with the 8 bits from the expanded right side in order
        - 4 xor k1, 1 xor k2 |2 xor k3, 3 xor k4
        - 2 xor k5, 3 xor k6 | 4 xor k7, 1 xor k8
        - This turns into (⊕ = direct sum)
            - 4 ⊕ k1, 1 ⊕ k2 |2 ⊕ k3, 3 ⊕ k4
            - 2 ⊕ k5, 3 ⊕ k6 | 4 ⊕ k7, 1 ⊕ k8
    - Take the first 4 bits (the first row) and put them into s0, and the bottom row into s1
        - S0 = $\begin{bmatrix}1 & 0 & 3 & 2 \\ 3 & 2 & 1 & 0 \\ 0 & 2 & 1 & 3 \\ 3 & 1 & 3 & 2 \end{bmatrix}$
        - S1 = $\begin{bmatrix}0 & 1 & 2 & 3 \\ 2 & 0 & 1 & 3 \\ 3 & 0 & 1 & 0 \\ 2 & 1 & 0 & 3 \end{bmatrix}$
        - The 1st and 4th bits of the input (the row of the P matrix) are treated as a 2-bit number and define the row of the S matrix while the 2nd and 3rd bit are used as the column
        - Repeat that for the 2nd row of the P matrix
        - The 4 bits that the 2 numbers grabbed from the S tables are put together like this
            - 2, 4, 3, 1
            - 1 and 2 are the two bits from the s0 and 3 and 4 are the two bits from s1
    - The output of Fk is (left side of the original 8 bits xor the 4 bits from the S tables, original right side of the original 8 bits)
        - L xor S, R
        - Full expression is = L xor (s table stuff(R expansion and xor with key)), R

4. **Switch Function**
    - Switches the 4 bits on the left and the 4 bits on the right so Fk can be done on the other side of the 8 bits

5. **The Fk function (again)**
    - Repeat the Fk process but with k2 this time

6. **Final Permutation (reverse the initial)**
    - Switch bits in the following order 
        - 4, 1, 3, 5, 7, 2, 8, 6
        - Example: input bit in position 1 becomes the output bit in position 2


## Running

Included in the files is a make file that will compile the simple_des.cc.

When running the executable, you need to include what the key is in the format: 0xYYY. You then need to set the input file, output file, and log files, such as the following: ```./simple_des 0x36C < test_files/input.txt > out.txt 2> log.txt```

## File Structure

The `test_files` folder contains a sample input, as well as a sample output and log files for running the command ```./simple_des 0x36C < test_files/input.txt > out.txt 2> log.txt```