#pragma once
#include<iostream>
#include <string>

using namespace std;
class Radix {
public:
	int binaryToDecimal(string n);
	string twosComplement(string binary);
	string decimalToBinary(int decimal, int digits);
	string decimalTohexa(int decimal);
	int hexaToDecimal(string hex);

private:

};


int Radix::binaryToDecimal(string n) // converts binary to decimal
{
    string num = n;
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int len = num.length();
    for (int i = len - 1; i >= 0; i--)
    {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }

    return dec_value;
}

string Radix::twosComplement(string binary) {
    // Determine the number of bits in the binary representation
    int numBits = binary.length();

    // Find the position of the most significant bit set to 1
    bool  flag = true;
    if (binary[numBits - 1] == '1')
    {
        flag = false;
    }


    // If there are no bits set to 1, the twos complement is just the original number
    if (flag) {
        return binary;
    }

    // Create a string to store the ones complement
    string onesComplement = binary;

    // Flip all bits in the ones complement
    for (int i = 0; i < numBits; i++) {
        if (onesComplement[i] == '0') {
            onesComplement[i] = '1';
        }
        else {
            onesComplement[i] = '0';
        }
    }

    // Create a string to store the twos complement
    string twosComplement = onesComplement;

    // Add one to the twos complement
    for (int i = numBits - 1; i >= 0; i--) {
        if (twosComplement[i] == '0') {
            twosComplement[i] = '1';
        }
        else {
            twosComplement[i] = '0';
        }
    }

    return twosComplement;
}

string Radix::decimalToBinary(int decimal, int digits) // ddecimal to binary
{
    bool flag = false;
    if (decimal < 0)
    {
        flag = true;
        decimal *= -1;
    }
    // array to store binary number
    int binaryNum[32] = { 0 };
    string bin = "";
    // counter for binary array
    int i = 0;


    while (decimal > 0) {

        // storing remainder in binary array
        binaryNum[i] = decimal % 2;
        decimal = decimal / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = digits - 1; j >= 0; j--)
        bin += to_string(binaryNum[j]);

    if (flag)
    {
        bin = twosComplement(bin);
    }
    return bin;
}

string Radix::decimalTohexa(int decimal) // decimal to hex
{
    bool flag = false;
    if (decimal < 0)
    {
        flag = true;
        decimal *= -1;
    }
    // array to store binary number
    string hexNum[8] = { "0","0","0","0","0","0","0","0" };
    string hex = "0x";
    if (flag)
    {
        hex = "-0x";
    }

    int i = 0;
    while (decimal != 0)
    {
        // remainder variable to store remainder
        int rem = 0;

        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = decimal % 16;

        // check if temp < 10
        if (rem < 10) {
            ch = rem + 48;
        }
        else
        {
            ch = rem + 55;
        }
        hexNum[i] = ch;
        decimal = decimal / 16;
        i++;
    }
    // printing binary array in reverse order
    for (int j = 7; j >= 0; j--)
        hex += hexNum[j];
    return hex;
}

int Radix::hexaToDecimal(string hex) // hex
{
    int r, len = 8, sum = 0;
    for (int i = 2; i < 10; i++)
    {
        r = -1;
        len--;
        if (hex[i] >= '0' && hex[i] <= '9')
            r = hex[i] - 48;
        else if (hex[i] >= 'a' && hex[i] <= 'f')
            r = hex[i] - 87;
        else if (hex[i] >= 'A' && hex[i] <= 'F')
            r = hex[i] - 55;
        if (r == -1)
        {
            return -1;
        }
        sum += r * pow(16, len);
    }
    return sum;
}