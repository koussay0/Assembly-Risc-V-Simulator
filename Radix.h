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

int Radix::binaryToDecimal(string bin) // converts binary to decimal
{
    int dec = 0;
    int base = 1;

    for (int i = bin.length() - 1; i >= 0; i--) {
        if (bin[i] == '1')
            dec += base;
        base *= 2;
    }

    return dec;
}

string Radix::twosComplement(string bin) {
    int len = bin.length();
    bool pos = true;

    if (bin[len - 1] == '1') {
        pos = false;
    }

    if (pos) {
        return bin;
    }

    string ones = bin;

    for (int i = 0; i < len; i++) {
        ones[i] = (ones[i] == '0') ? '1' : '0';
    }

    string twos = ones;

    for (int i = len - 1; i >= 0; i--) {
        if (twos[i] == '0') {
            twos[i] = '1';
            break;
        }
        else {
            twos[i] = '0';
        }
    }

    return twos;
}

string Radix::decimalToBinary(int dec, int bits) // decimal to binary
{
    bool neg = false;
    if (dec < 0) {
        neg = true;
        dec = -dec;
    }

    int bin[32] = { 0 };
    string result = "";

    int i = 0;
    while (dec > 0) {
        bin[i++] = dec % 2;
        dec /= 2;
    }

    for (int j = bits - 1; j >= 0; j--)
        result += to_string(bin[j]);

    if (neg) {
        result = twosComplement(result);
    }

    return result;
}

string Radix::decimalTohexa(int dec) // decimal to hex
{
    bool neg = false;
    if (dec < 0) {
        neg = true;
        dec = -dec;
    }

    string hex[8] = { "0","0","0","0","0","0","0","0" };
    string result = neg ? "-0x" : "0x";

    int i = 0;
    while (dec != 0) {
        int rem = dec % 16;
        char ch = (rem < 10) ? (rem + '0') : (rem + 'A' - 10);
        hex[i++] = ch;
        dec /= 16;
    }

    for (int j = 7; j >= 0; j--)
        result += hex[j];

    return result;
}

int Radix::hexaToDecimal(string hex) // hex to decimal
{
    int dec = 0, len = 8;
    for (int i = 2; i < 10; i++) {
        int r = -1;
        len--;
        if (hex[i] >= '0' && hex[i] <= '9')
            r = hex[i] - '0';
        else if (hex[i] >= 'a' && hex[i] <= 'f')
            r = hex[i] - 'a' + 10;
        else if (hex[i] >= 'A' && hex[i] <= 'F')
            r = hex[i] - 'A' + 10;

        if (r == -1) {
            return -1;
        }
        dec += r * pow(16, len);
    }
    return dec;
}
