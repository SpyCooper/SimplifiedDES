/*
    Name: Josh Myers
    Date: 9/19/2024
*/
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// keygen functions declarations
int keygen_split_1(int inputted_key);
int keygen_get_key_1(int ten_bit_key);
int keygen_get_key_2(int ten_bit_key);
int keygen_permutation(int recombined_sides);
int keygen_bit_shift(int ten_bits);

// other function declarations
int initial_permutation(int character);
int fk(int character, int key);
int reverse_permutation(int character);
int switch_halves(int character);

// main function
int main(int argc, char *argv[])
{
    // setsa default key value
    int original_key = 0x36c;

    // check if a key was entered
    if (argc != 2)
    {
        // if no key was entered, return an error
        cout << "Error: no key value detected" << endl;
        // exit the program
        return 1;
    }
    // if a key was entered
    else
    {
        // convert the key to a number from hexidecimal
        original_key = strtol(argv[1], NULL, 16);
        // check if the key is 10 bits (greater than 0x3ff)
        if (original_key > 0b1111111111)
        {
            cout << "Error: key value is not 10 bits" << endl;
            // exit the program
            return 1;
        }
    }

    // run keygen functions to get the 2 keys
    int ten_bits = keygen_split_1(original_key);
    fprintf(stderr, "p10: %i\n", ten_bits);
    int key_1 = keygen_get_key_1(ten_bits);
    fprintf(stderr, "k1: %i\n", key_1);
    int key_2 = keygen_get_key_2(ten_bits);
    fprintf(stderr, "k2: %i\n", key_2);

    // read in text from a standard input
    int c;
    
    // read in the first character
    c = getchar();
    
    // set up a counter for the bytes
    int counter = 0;
    // loop through the text until the end of the file is reached
    while(c != EOF)
    {
        // convert the character to a number
        int value = (int)c;

        // print debugging tests
        fprintf(stderr, "encrypting byte #%i with value %i\n", counter, value);

        // run initial_permutation and output it
        int ip = initial_permutation(value);
        fprintf(stderr, "ip: %i\n", ip);

        // run fk with key #1
        int fk1 = fk(ip, key_1);

        // run switch_halves
        int switched = switch_halves(fk1);
        fprintf(stderr, "fk1: %i\n", switched);

        // run fk with key #2
        int fk2 = fk(switched, key_2);
        fprintf(stderr, "fk2: %i\n", fk2);

        // run reverse_permutation
        int encrypted_char = reverse_permutation(fk2);
        
        // print the text out in a standard output in binary mode
        cout << (char)encrypted_char;

        // increment the counter for the bytes
        counter++;

        // read in the next character
        c = getchar();
    }

    // exit the program
    return 0;
}

// takes in a 10 bit key and rearranges the bits to the correct order
int keygen_split_1(int inputted_key)
{
    // rearrange the bits in the following order
    // ex: 3, 5, 2, 7, 4, 10, 1, 9, 8, 6
    int rearraged_key = 0b0000000000;
    if (inputted_key & 0b1000000000)
    {
        rearraged_key = rearraged_key + 0b0000001000;
    }
    if (inputted_key & 0b0100000000)
    {
        rearraged_key = rearraged_key + 0b0010000000;
    }
    if (inputted_key & 0b0010000000)
    {
        rearraged_key = rearraged_key + 0b1000000000;
    }
    if (inputted_key & 0b0001000000)
    {
        rearraged_key = rearraged_key + 0b0000100000;
    }
    if (inputted_key & 0b0000100000)
    {
        rearraged_key = rearraged_key + 0b0100000000;
    }
    if (inputted_key & 0b0000010000)
    {
        rearraged_key = rearraged_key + 0b0000000001;
    }
    if (inputted_key & 0b0000001000)
    {
        rearraged_key = rearraged_key + 0b0001000000;
    }
    if (inputted_key & 0b0000000100)
    {
        rearraged_key = rearraged_key + 0b0000000010;
    }
    if (inputted_key & 0b0000000010)
    {
        rearraged_key = rearraged_key + 0b0000000100;
    }
    if (inputted_key & 0b0000000001)
    {
        rearraged_key = rearraged_key + 0b00000010000;
    }

    // return the rearraged key
    return rearraged_key;
}

// takes in a 10 bit key and returns the first 8 bit key
int keygen_get_key_1(int ten_bit_key)
{
    // bit shift the 10 bit key
    int shifted_key = keygen_bit_shift(ten_bit_key);
    
    // take 8 bits from the combined value
    int final_key_1 = keygen_permutation(shifted_key);

    // return the final key
    return final_key_1;
}

// takes in a 10 bit key and returns the second 8 bit key
int keygen_get_key_2(int ten_bit_key)
{
    // bit shift the 10 bit key 3 times (1 for the 1st key and 2 for the 2 bit shifts for key 2)
    int shifted_key = keygen_bit_shift(ten_bit_key);
    shifted_key = keygen_bit_shift(shifted_key);
    shifted_key = keygen_bit_shift(shifted_key);

    // take 8 bits from the combined value
    int final_key_2 = keygen_permutation(shifted_key);

    // return the final key
    return final_key_2;
}

// takes in a 10 bit key and returns the rearranged into an 8 bit key
int keygen_permutation(int recombined_sides)
{
    // rearrange the bits to the correct 8 bit order
    // ex: 6, 3, 7, 4, 8, 5, 10, 9
    int permutated_key = 0b00000000;
    if (recombined_sides & 0b0010000000)
    {
        permutated_key = permutated_key + 0b01000000;
    }
    if (recombined_sides & 0b0001000000)
    {
        permutated_key = permutated_key + 0b00010000;
    }
    if (recombined_sides & 0b0000100000)
    {
        permutated_key = permutated_key + 0b00000100;
    }
    if (recombined_sides & 0b0000010000)
    {
        permutated_key = permutated_key + 0b10000000;
    }
    if (recombined_sides & 0b0000001000)
    {
        permutated_key = permutated_key + 0b00100000;
    }
    if (recombined_sides & 0b0000000100)
    {
        permutated_key = permutated_key + 0b00001000;
    }
    if (recombined_sides & 0b0000000010)
    {
        permutated_key = permutated_key + 0b00000001;
    }
    if (recombined_sides & 0b0000000001)
    {
        permutated_key = permutated_key + 0b00000010;
    }

    // return the permuated key
    return permutated_key;
}

// takes in a 10 bit key and returns the bit shifted value
int keygen_bit_shift(int ten_bits)
{
    // split the 10 bit key into 2 5 bit values
    int left_side_bits = ten_bits >> 5;
    int right_side_bits = ten_bits & 0b00011111;

    // rotate the 1st 5 bits to the right
    bool rotate = false;
    // check if the left most bit is set
    if (left_side_bits & 0b10000)
    {
        rotate = true;
    }
    // shift the bits to the left
    left_side_bits = left_side_bits << 1;
    // if the left most bit was set, rotate that specific bit
    if (rotate)
    {
        left_side_bits = left_side_bits - 0b100000;
        left_side_bits = left_side_bits + 0b00001;
    }

    // rotate the 2nd 5 bits to the right
    rotate = false;
    if (right_side_bits & 0b10000)
    {
        rotate = true;
    }
    // shift the bits to the left
    right_side_bits = right_side_bits << 1;
    // if the left most bit was set, rotate that specific bit
    if (rotate)
    {
        right_side_bits = right_side_bits - 0b100000;
        right_side_bits = right_side_bits + 0b00001;
    }

    // combine the two bit shifted values
    int recombined_left_and_right = left_side_bits * 0b100000 + right_side_bits;
    
    // return the recombined value
    return recombined_left_and_right;
}

// takes in a character and rearranges the bits to the initial permutation order
int initial_permutation(int character)
{
    // rearrange the bits to the correct order
    // ex: 2, 6, 3, 1, 4, 8, 5, 7
    int new_character = 0b00000000;
    if (character & 0b10000000)
    {
        new_character = new_character + 0b00010000;
    }
    if (character & 0b01000000)
    {
        new_character = new_character + 0b10000000;
    }
    if (character & 0b00100000)
    {
        new_character = new_character + 0b00100000;
    }
    if (character & 0b00010000)
    {
        new_character = new_character + 0b00001000;
    }
    if (character & 0b00001000)
    {
        new_character = new_character + 0b00000010;
    }
    if (character & 0b00000100)
    {
        new_character = new_character + 0b01000000;
    }
    if (character & 0b00000010)
    {
        new_character = new_character + 0b00000001;
    }
    if (character & 0b00000001)
    {
        new_character = new_character + 0b00000100;
    }

    // return the rearranged value
    return new_character;
}

// takes in a character and a key and returns the fiestal value
int fk(int character, int key)
{
    // take the 4 bits from the left side pf the character
    int left_side = character >> 4;

    // take the 4 bits from the right side of the character
    int right_side = character & 0b00001111;

    // expand the right side to 8 bits
    int expanded_right_side_top = 0b0000;
    int expanded_right_side_bottom = 0b0000;
    // manually expand the bits to the correct order for the top and bottom of the matrix
    if (right_side & 0b1000)
    {
        expanded_right_side_top = expanded_right_side_top + 0b0100;
        expanded_right_side_bottom = expanded_right_side_bottom + 0b0001;
    }
    if (right_side & 0b0100)
    {
        expanded_right_side_top = expanded_right_side_top + 0b0010;
        expanded_right_side_bottom = expanded_right_side_bottom + 0b1000;
    }
    if (right_side & 0b0010)
    {
        expanded_right_side_top = expanded_right_side_top + 0b0001;
        expanded_right_side_bottom = expanded_right_side_bottom + 0b0100;
    }
    if (right_side & 0b0001)
    {
        expanded_right_side_top = expanded_right_side_top + 0b1000;
        expanded_right_side_bottom = expanded_right_side_bottom + 0b0010;
    }

    // split the key into two 4 bit values
    int key_top = key >> 4;
    int key_bottom = key & 0b00001111;

    // manually xor each expanded character bit with the corresponding key bit for the left side
    int xor_value_top = 0b0000;
    // check if the bits are different and add the xor value
    if ((expanded_right_side_top & 0b1000) != (key_top & 0b1000))
    {
        xor_value_top = xor_value_top + 0b1000;
    }
    if ((expanded_right_side_top & 0b0100) != (key_top & 0b0100))
    {
        xor_value_top = xor_value_top + 0b0100;
    }
    if ((expanded_right_side_top & 0b0010) != (key_top & 0b0010))
    {
        xor_value_top = xor_value_top + 0b0010;
    }
    if ((expanded_right_side_top & 0b0001) != (key_top & 0b0001))
    {
        xor_value_top = xor_value_top + 0b0001;
    }

    // manually xor each expanded character bit with the corresponding key bit for the right side
    int xor_value_bottom = 0b0000;
    if ((expanded_right_side_bottom & 0b1000) != (key_bottom & 0b1000))
    {
        xor_value_bottom = xor_value_bottom + 0b1000;
    }
    if ((expanded_right_side_bottom & 0b0100) != (key_bottom & 0b0100))
    {
        xor_value_bottom = xor_value_bottom + 0b0100;
    }
    if ((expanded_right_side_bottom & 0b0010) != (key_bottom & 0b0010))
    {
        xor_value_bottom = xor_value_bottom + 0b0010;
    }
    if ((expanded_right_side_bottom & 0b0001) != (key_bottom & 0b0001))
    {
        xor_value_bottom = xor_value_bottom + 0b0001;
    }

    // set up the s0 and s1 matrix
    int s0_matrix[4][4] = {
        {1, 0, 3, 2},
        {3, 2, 1, 0},
        {0, 2, 1, 3},
        {3, 1, 3, 2}};
    
    int s1_matrix[4][4] = {
        {0, 1, 2, 3},
        {2, 0, 1, 3},
        {3, 0, 1, 0},
        {2, 1, 0, 3}};

    // take the 4 bits from the xor value and get s0 row and column
    int s0_row  = 0b00;
    if (xor_value_top & 0b1000)
    {
        s0_row = s0_row + 0b10;
    }
    if (xor_value_top & 0b0001)
    {
        s0_row = s0_row + 0b01;
    }
    int s0_column  = 0b00;
    if (xor_value_top & 0b0100)
    {
        s0_column = s0_column + 0b10;
    }
    if (xor_value_top & 0b0010)
    {
        s0_column = s0_column + 0b01;
    }
    // get the s0 value from the matrix
    int s0_value = s0_matrix[s0_row][s0_column];

    // take the 4 bits from the xor value and get s1 row and column
    int s1_row = 0b00;
    if (xor_value_bottom & 0b1000)
    {
        s1_row = s1_row + 0b10;
    }
    if (xor_value_bottom & 0b0001)
    {
        s1_row = s1_row + 0b01;
    }
    int s1_column = 0b00;
    if (xor_value_bottom & 0b0100)
    {
        s1_column = s1_column + 0b10;
    }
    if (xor_value_bottom & 0b0010)
    {
        s1_column = s1_column + 0b01;
    }
    // get the s1 value from the matrix
    int s1_value = s1_matrix[s1_row][s1_column];

    // rearrange the 4 bit value to the correct order
    int new_4_bit_value = 0b0000;
    if (s0_value & 0b10)
    {
        new_4_bit_value = new_4_bit_value + 0b0001;
    }
    if (s0_value & 0b01)
    {
        new_4_bit_value = new_4_bit_value + 0b1000;
    }
    if (s1_value & 0b10)
    {
        new_4_bit_value = new_4_bit_value + 0b0010;
    }
    if (s1_value & 0b01)
    {
        new_4_bit_value = new_4_bit_value + 0b0100;
    }

    // xor the 4 bit value with the left side
    int xor_value = new_4_bit_value ^ left_side;

    // combine the xor value with the original right side
    int new_character = xor_value * 0b10000 + right_side;

    // return the combined value
    return new_character;
}

// takes in a character and rearranges the bits to the reverse permutation order
int reverse_permutation(int character)
{
    int new_character = 0b00000000;
    // rearrange the bits to the following order
    // ex: 4, 1, 3, 5, 7, 2, 8, 6
    if (character & 0b10000000)
    {
        new_character = new_character + 0b01000000;
    }
    if (character & 0b01000000)
    {
        new_character = new_character + 0b00000100;
    }
    if (character & 0b00100000)
    {
        new_character = new_character + 0b00100000;
    }
    if (character & 0b00010000)
    {
        new_character = new_character + 0b10000000;
    }
    if (character & 0b00001000)
    {
        new_character = new_character + 0b00010000;
    }
    if (character & 0b00000100)
    {
        new_character = new_character + 0b00000001;
    }
    if (character & 0b00000010)
    {
        new_character = new_character + 0b00001000;
    }
    if (character & 0b00000001)
    {
        new_character = new_character + 0b00000010;
    }

    // return the rearranged value
    return new_character;
}

// takes in a character and switches the left and right 4 bits
int switch_halves(int character)
{
    // take the 4 bits from the left side
    int left_side = character >> 4;

    // take the 4 bits from the right side
    int right_side = character & 0b00001111;

    // switch the left and the right sides
    int switched = right_side * 0b10000 + left_side;

    // return the combined value
    return switched;
}