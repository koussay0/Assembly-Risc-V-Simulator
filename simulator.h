#pragma once
#include "Radix.h"
#include <fstream>
#include <string>
#include<sstream>
#include <vector>
#include<map>

//#ifndef
//#endif

using namespace std;

string commands[][5] = {
    {"add", "R", "0110011", "000", "0000000"},
    {"addi", "I", "0010011", "000", ""},
    {"and", "R", "0110011", "111", "0000000"},
    {"andi", "I", "0010011", "111", ""},
    {"auiprogramCounter", "J", "0010111", "", ""},
    {"beq", "SB", "1100011", "000", ""},
    {"bge", "SB", "1100011", "101", ""},
    {"bgeu", "SB", "1100011", "111", ""},
    {"blt", "SB", "1100011", "100", ""},
    {"bltu", "SB", "1100011", "110", ""},
    {"bne", "SB", "1100011", "001", ""},
    {"ebreak", "Z", "", "", ""},
    {"ecall", "Z", "", "", ""},
    {"fence", "Z", "", "", ""},
    {"jal", "UJ", "1101111", "", ""},
    {"jalr", "I", "1100111", "000", ""},
    {"lb", "I", "0000011", "000", ""},
    {"lbu", "I", "0000011", "100", ""},
    {"lh", "I", "0000011", "001", ""},
    {"lhu", "I", "0000011", "101", ""},
    {"li", "", "", "", ""},
    {"lui", "U", "0110111", "", ""},
    {"lw", "I", "0000011", "010", ""},
    {"mv", "", "", "", ""},
    {"neg", "", "", "", ""},
    {"or", "R", "0110011", "110", "0000000"},
    {"ori", "I", "0010011", "110", ""},
    {"sb", "S", "0100011", "000", ""},
    {"seqz", "", "", "", ""},
    {"sh", "S", "0100011", "001", ""},
    {"sll", "R", "0110011", "001", "0000000"},
    {"slli", "I", "0010011", "001", "0000000"},
    {"slt", "R", "0110011", "010", "0000000"},
    {"slti", "I", "0010011", "010", ""},
    {"sltiu", "I", "0010011", "011", ""},
    {"sltu", "R", "0110011", "011", "0000000"},
    {"sltz", "", "", "", ""},
    {"sra", "R", "0110011", "101", "0100000"},
    {"srai", "I", "0010011", "101", "0100000"},
    {"srl", "R", "0110011", "101", "0000000"},
    {"srli", "I", "0010011", "101", "0000000"},
    {"sub", "R", "0110011", "000", "0100000"},
    {"sw", "S", "0100011", "010", ""},
    {"xor", "R", "0110011", "100", "0000000"},
    {"xori", "I", "0010011", "100", ""}
}; // Array of Commands  44 in the format as follows {instruction,format,OprogramCounterODE,funct3,funct 7}


int programCounter = 0; // program counter
vector<string> memoryStore;
vector<vector<string>> memoryContents;
vector<vector<string>> stackFrames;
int regs[32] = { 0 }; // 32 regs

class simulator {
public:
    int FindString(const string& key, int total); // Searches for the given string `key` in an array of instructions.
    int RegisterIndex(const string& regName); // Maps a register name(e.g., "x0", "a1", "sp") to its numeric index.
    
    string stripCharacter(string& token, char ch);  // Removes all occurrences of a specific character `ch` from the given string `token`.
    int Run(int instruction, string line); // Decodes and executes a single assembly instruction represented by `line`
    void loadProgram(const string& filePath); // Reads a program file specified by `filePath` and loads its instructions into memory.
    string stripComma(string& token);  // Removes commas from the given string `token`.
    void stripColon(string& token); // Removes colons from the given string `token`.
  
    void print(); // Prints the current state of the simulator
};


int simulator::FindString(const string& key, int total)
{
    int start = 0, end = total - 1;
    while (start <= end)
    {
        int middle = (start + end) >> 1; // Use bit-shifting for division by 2
        if (commands[middle][0] == key)
        {
            return middle;
        }
        else if (commands[middle][0] < key)
        {
            start = middle + 1;
        }
        else
        {
            end = middle - 1;
        }
    }
    return -1;
}


int simulator::RegisterIndex(const string& regName)
{
    static map<string, int> registerMap = {
        {"zero", 0}, {"x0", 0}, {"ra", 1}, {"x1", 1}, {"sp", 2}, {"x2", 2},
        {"gp", 3}, {"x3", 3}, {"tp", 4}, {"x4", 4}, {"t0", 5}, {"x5", 5},
        {"t1", 6}, {"x6", 6}, {"t2", 7}, {"x7", 7}, {"s0", 8}, {"x8", 8},
        {"s1", 9}, {"x9", 9}, {"a0", 10}, {"x10", 10}, {"a1", 11}, {"x11", 11},
        {"a2", 12}, {"x12", 12}, {"a3", 13}, {"x13", 13}, {"a4", 14}, {"x14", 14},
        {"a5", 15}, {"x15", 15}, {"a6", 16}, {"x16", 16}, {"a7", 17}, {"x17", 17},
        {"s2", 18}, {"x18", 18}, {"s3", 19}, {"x19", 19}, {"s4", 20}, {"x20", 20},
        {"s5", 21}, {"x21", 21}, {"s6", 22}, {"x22", 22}, {"s7", 23}, {"x23", 23},
        {"s8", 24}, {"x24", 24}, {"s9", 25}, {"x25", 25}, {"s10", 26}, {"x26", 26},
        {"s11", 27}, {"x27", 27}, {"t3", 28}, {"x28", 28}, {"t4", 29}, {"x29", 29},
        {"t5", 30}, {"x30", 30}, {"t6", 31}, {"x31", 31}
    };

    auto it = registerMap.find(regName);
    return (it != registerMap.end()) ? it->second : -1;
}


void simulator::print()
{
    Radix converter;
    cout << "Register Contents:\n";
    for (int i = 0; i < 32; ++i)
    {
        cout << "x" << i << " = " << regs[i]
            << ", Binary: " << converter.decimalToBinary(regs[i], 32)
            << ", Hex: " << converter.decimalTohexa(regs[i]) << "\n";
    }

    cout << "\nData Contents:\n";
    for (const auto& data : memoryContents)
    {
        cout << data[0] << " = " << data[1];
        if (data.size() == 3)
        {
            cout << " = " << data[2];
        }
        cout << "\n";
    }

    cout << "\nstackFrames Contents:\n";
    for (const auto& frame : stackFrames )
    {
        cout << frame[0] << " = " << frame[1] << "\n";
    }

    cout << "\nProgram Counter = " << programCounter
        << ", Binary: " << converter.decimalToBinary(programCounter, 32)
        << ", Hex: " << converter.decimalTohexa(programCounter) << "\n";
}



string simulator::stripComma(string& word)
{
    word.erase(remove(word.begin(), word.end(), ','), word.end());
    return word;
}


void simulator::stripColon(string& word)
{
    word.erase(remove(word.begin(), word.end(), ':'), word.end());
}


string simulator::stripCharacter(string& word, char character)
{
    for (size_t i = 0; i < word.size(); /* no increment here */)
    {
        if (word[i] == character)
        {
            word.erase(i, 1); // Remove the character
        }
        else
        {
            ++i; // Increment only if no removal happens
        }
    }
    return word;
}

int simulator::Run(int instruction, string line)
{
    Radix convert;
    cout << line << endl;
    if (commands[instruction][1] == "Z")
    {
        return -2;
    }
    istringstream iss(line);
    string word;
    iss >> word;
    int rs1, rs2, rd;
    int reg[3];


    switch (instruction)
    {
    case 0: // ADD
    {

        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }

        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];
        regs[rd] = regs[rs1] + regs[rs2];

        break;
    }
    case 1: // ADDI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }

        rd = reg[0];

        rs1 = reg[1];
        regs[rd] = regs[rs1] + stoi(word);
        break;
    }
    case 2: // AND
    {

        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];

        int v1 = regs[rs1];
        int v2 = regs[rs2];
        string bin1 = convert.decimalToBinary(v1, 32);
        string bin2 = convert.decimalToBinary(v2, 32);
        string bin3 = "";

        for (int i = 0; i < 32; i++)
        {
            if ((bin1[i] == '1') && (bin2[i] == '1'))
            {
                bin3 += '1';
            }
            else
            {
                bin3 += '0';
            }
        }
        regs[rd] = convert.binaryToDecimal(bin3);
        break;
    }
    case 3: // ANDI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        int v1 = regs[rs1];
        int v2 = stoi(word);
        string bin1 = convert.decimalToBinary(v1, 32);
        string bin2 = convert.decimalToBinary(v2, 32);
        string bin3 = "";
        for (int i = 0; i < 32; i++)
        {
            if ((bin1[i] == '1') && (bin2[i] == '1'))
            {
                bin3 += '1';
            }
            else
            {
                bin3 += '0';
            }
        }
        regs[rd] = convert.binaryToDecimal(bin3);
        break;
    }

    case 4: // AUIprogramCounter
    {
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        if ((reg[0] == -1) || (reg[0] == 0))
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rd = reg[0];

        regs[rd] = convert.hexaToDecimal(word);
        break;
    }
    case 5: // BEQ
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rs1 = reg[0];
        rs2 = reg[1];
        if (regs[rs1] == regs[rs2])
        {
            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][1] == word)
                {
                    programCounter = convert.hexaToDecimal(memoryContents[i][0]) / 4;
                    break;
                }
                else if (i == memoryContents.size() - 1)
                {
                    for (int j = 0; j < word.size(); j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    programCounter += stoi(word) / 4;
                }
            }
        }
        break;
    }
    case 6: // BGE
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rs1 = reg[0];
        rs2 = reg[1];
        if (regs[rs1] >= regs[rs2])
        {
            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][1] == word)
                {
                    programCounter = convert.hexaToDecimal(memoryContents[i][0]) / 4;
                    break;
                }
                else if (i == memoryContents.size() - 1)
                {
                    for (int j = 0; j < word.size(); j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    programCounter += stoi(word) / 4;
                }
            }
        }
        break;
    }
    case 7: // BGEU
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rs1 = reg[0];
        rs2 = reg[1];
        if (abs(regs[rs1]) >= abs(regs[rs2]))
        {
            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][1] == word)
                {
                    programCounter = convert.hexaToDecimal(memoryContents[i][0]) / 4;
                    break;
                }
                else if (i == memoryContents.size() - 1)
                {
                    for (int j = 0; j < word.size(); j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    programCounter += stoi(word) / 4;
                }
            }
        }
        break;
    }
    case 8: // BLT
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rs1 = reg[0];
        rs2 = reg[1];
        if (regs[rs1] < regs[rs2])
        {
            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][1] == word)
                {
                    programCounter = convert.hexaToDecimal(memoryContents[i][0]) / 4;
                    break;
                }
                else if (i == memoryContents.size() - 1)
                {
                    for (int j = 0; j < word.size(); j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    programCounter += stoi(word) / 4;
                }
            }
        }
        break;
    }
    case 9: // BLTU
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rs1 = reg[0];
        rs2 = reg[1];
        if (abs(regs[rs1]) < abs(regs[rs2]))
        {
            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][1] == word)
                {
                    programCounter = convert.hexaToDecimal(memoryContents[i][0]) / 4;
                    break;
                }
                else if (i == memoryContents.size() - 1)
                {
                    for (int j = 0; j < word.size(); j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    programCounter += stoi(word) / 4;
                }
            }
        }
        break;
    }
    case 10: // BNE
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rs1 = reg[0];
        rs2 = reg[1];
        if (regs[rs1] != regs[rs2])
        {
            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][1] == word)
                {
                    programCounter = convert.hexaToDecimal(memoryContents[i][0]) / 4;
                    break;
                }
                else if (i == memoryContents.size() - 1)
                {
                    for (int j = 0; j < word.size(); j++)
                    {
                        if (!isdigit(word[i]))
                        {
                            return -1;
                        }
                    }
                    programCounter += stoi(word) / 4;
                }
            }
        }
        break;
    }

    case 14: // JAL
    {
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rd = reg[0];
        for (int i = 0; i < memoryContents.size(); i++)
        {
            if (memoryContents[i][1] == word)
            {
                programCounter = convert.hexaToDecimal(memoryContents[i][0]) / 4;
                break;
            }
            else if (i == memoryContents.size() - 1)
            {
                for (int j = 0; j < word.size(); j++)
                {
                    if (!isdigit(word[i]))
                    {
                        return -1;
                    }
                }
                regs[rd] = (programCounter * 4);
                programCounter += stoi(word) / 4;
            }
        }
        break;
    }
    case 15: // JALR
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rd = reg[0];
        rs1 = reg[1];
        for (int i = 0; i < memoryContents.size(); i++)
        {
            if (memoryContents[i][1] == word)
            {
                regs[rd] = programCounter;
                programCounter = ((convert.hexaToDecimal(memoryContents[i][0]) - programCounter * 4) / 4) + regs[rs1] / 4;
                break;
            }
            else if (i == memoryContents.size() - 1)
            {
                for (int j = 0; j < word.size(); j++)
                {
                    if (!isdigit(word[i]))
                    {
                        return -1;
                    }
                }
                regs[rd] = (programCounter * 4);
                programCounter = (regs[rs1] + stoi(word)) / 4;
            }
        }
        break;
    }
    case 16: // LB
    {
        string half_word;
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rd = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rs1 = RegisterIndex(regis);

        if (rs1 == -1)
        {
            return -1;
        }

        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                    {
                        string temp = stackFrames[i][1];

                        char temp_2 = temp[0];
                        if (temp_2 == '1')
                        {
                            half_word = "111111111111111111111111";
                        }

                        for (int i = 24; i < 32; i++)
                        {
                            half_word += temp[i];
                        }

                        regs[rd] = convert.binaryToDecimal(half_word);
                        return 0;
                    }
                }
                regs[rd] = 0;
            }
        }

        else
        {

            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                {
                    string temp = memoryContents[i][1];

                    char temp_2 = temp[0];
                    if (temp_2 == '1')
                    {
                        half_word = "111111111111111111111111";
                    }

                    for (int i = 24; i < 32; i++)
                    {
                        half_word += temp[i];
                    }

                    regs[rd] = convert.binaryToDecimal(half_word);
                    return 0;
                }
            }
            regs[rd] = 0;
        }
        break;
    }

    case 17: // LBU
    {
        string half_word;
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rd = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rs1 = RegisterIndex(regis);

        if (rs1 == -1)
        {
            return -1;
        }

        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                    {
                        string temp = stackFrames[i][1];

                        for (int i = 24; i < 32; i++)
                        {
                            half_word += temp[i];
                        }

                        regs[rd] = convert.binaryToDecimal(half_word);
                        return 0;
                    }
                }
                regs[rd] = 0;
            }
        }

        else
        {

            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                {
                    string temp = memoryContents[i][1];

                    for (int i = 24; i < 32; i++)
                    {
                        half_word += temp[i];
                    }

                    regs[rd] = convert.binaryToDecimal(half_word);
                    return 0;
                }
            }
            regs[rd] = 0;
        }
        break;
    }
    case 18: // LH
    {
        string half_word;
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rd = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rs1 = RegisterIndex(regis);

        if (rs1 == -1)
        {
            return -1;
        }

        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                    {
                        string temp = stackFrames[i][1];

                        char temp_2 = temp[0];
                        if (temp_2 == '1')
                        {
                            half_word = "1111111111111111";
                        }

                        for (int i = 16; i < 32; i++)
                        {
                            half_word += temp[i];
                        }

                        regs[rd] = convert.binaryToDecimal(half_word);
                        return 0;
                    }
                }
                regs[rd] = 0;
            }
        }

        else
        {

            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                {
                    string temp = memoryContents[i][1];

                    char temp_2 = temp[0];
                    if (temp_2 == '1')
                    {
                        half_word = "1111111111111111";
                    }

                    for (int i = 16; i < 32; i++)
                    {
                        half_word += temp[i];
                    }

                    regs[rd] = convert.binaryToDecimal(half_word);
                    return 0;
                }
            }
            regs[rd] = 0;
        }
        break;
    }

    case 19: // LHU
    {
        string half_word;
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rd = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rs1 = RegisterIndex(regis);

        if (rs1 == -1)
        {
            return -1;
        }

        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                    {
                        string temp = stackFrames[i][1];

                        for (int i = 16; i < 32; i++)
                        {
                            half_word += temp[i];
                        }

                        regs[rd] = convert.binaryToDecimal(half_word);
                        return 0;
                    }
                }
                regs[rd] = 0;
            }
        }

        else
        {

            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                {
                    string temp = memoryContents[i][1];

                    for (int i = 16; i < 32; i++)
                    {
                        half_word += temp[i];
                    }

                    regs[rd] = convert.binaryToDecimal(half_word);
                    return 0;
                }
            }
            regs[rd] = 0;
        }
        break;
    }
    case 20: //LI PSEUDO-INSTRUCTION
    {
        for (int i = 0; i < 1; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }

        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];

        regs[rd] = stoi(word);

        break;
    }
    case 21: // LUI
    {
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rd = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        string temp = convert.decimalToBinary(stoi(word), 20);
        temp += "000000000000";
        regs[rd] = convert.binaryToDecimal(temp);
        break;
    }

    case 22: // LW
    {
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rd = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rs1 = RegisterIndex(regis);
        if (rs1 == -1)
        {
            return -1;
        }
        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                    {
                        regs[rd] = convert.binaryToDecimal(stackFrames[i][1]);
                        return 0;
                    }
                }
                regs[rd] = 0;
            }
        }
        else
        {
            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rs1]))
                {
                    regs[rd] = convert.binaryToDecimal(memoryContents[i][1]);
                    return 0;
                }
            }
            regs[rd] = 0;
        }
        break;
    }

    case 23: //MV PSEUDO-INSTRUCTION
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];

        regs[rd] = regs[rs1];

        break;
    }


    case 24: //NEG PSEUDO-INSTRUCTION
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];

        regs[rd] = -1 * regs[rs1];

        break;
    }

    case 25: // OR
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];
        int v1 = regs[rs1];
        int v2 = regs[rs2];
        string bin1 = convert.decimalToBinary(v1, 32);
        string bin2 = convert.decimalToBinary(v2, 32);
        string bin3 = "";

        for (int i = 0; i < 32; i++)
        {
            if ((bin1[i] == '0') && (bin2[i] == '0'))
            {
                bin3 += '0';
            }
            else
            {
                bin3 += '1';
            }
        }
        regs[rd] = convert.binaryToDecimal(bin3);
        break;
    }

    case 26: // ORI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        int v1 = regs[rs1];
        int v2 = stoi(word);
        string bin1 = convert.decimalToBinary(v1, 32);
        string bin2 = convert.decimalToBinary(v2, 32);
        string bin3 = "";
        for (int i = 0; i < 32; i++)
        {
            if ((bin1[i] == '0') && (bin2[i] == '0'))
            {
                bin3 += '0';
            }
            else
            {
                bin3 += '1';
            }
        }
        regs[rd] = convert.binaryToDecimal(bin3);
        break;
    }

    case 27: //SB
    {
        string byte;
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rs1 = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rd = RegisterIndex(regis);

        if (rd == -1)
        {
            return -1;
        }

        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rd]))
                    {
                        string temp = convert.decimalToBinary(regs[rs1], 32);

                        for (int i = 24; i < 32; i++)
                        {
                            byte += temp[i];
                        }

                        stackFrames[i][1] = convert.decimalToBinary(convert.binaryToDecimal(byte), 32);
                        return 0;
                    }
                }
                string temp = convert.decimalToBinary(regs[rs1], 32);

                for (int i = 24; i < 32; i++)
                {
                    byte += temp[i];
                }
                stackFrames.push_back({ convert.decimalTohexa(stoi(offset) + regs[rd]),convert.decimalToBinary(convert.binaryToDecimal(byte),32) });
            }
        }

        else
        {

            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rd]))
                {
                    string temp = convert.decimalToBinary(regs[rs1], 32);

                    for (int i = 24; i < 32; i++)
                    {
                        byte += temp[i];
                    }

                    memoryContents[i][1] = convert.decimalToBinary(convert.binaryToDecimal(byte), 32);
                    return 0;
                }
            }
            string temp = convert.decimalToBinary(regs[rs1], 32);

            for (int i = 24; i < 32; i++)
            {
                byte += temp[i];
            }
            memoryContents.push_back({ convert.decimalTohexa(stoi(offset) + regs[rd]),convert.decimalToBinary(convert.binaryToDecimal(byte),32) });
        }
        break;
    }

    case 28: //SEQZ PSEUDO-INSTRUCTION
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];

        int temp_1 = regs[rs1];

        if (temp_1 < 0)
        {
            temp_1 *= -1;
        }

        if (temp_1 < 1)
        {
            regs[rd] = 1;
        }
        else
        {
            regs[rd] = 0;
        }

        break;
    }
    case 29: // SH
    {
        string half_word;
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rs1 = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rd = RegisterIndex(regis);

        if (rd == -1)
        {
            return -1;
        }

        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rd]))
                    {
                        string temp = convert.decimalToBinary(regs[rs1], 32);

                        for (int i = 16; i < 32; i++)
                        {
                            half_word += temp[i];
                        }

                        stackFrames[i][1] = convert.decimalToBinary(convert.binaryToDecimal(half_word), 32);
                        return 0;
                    }
                }
                string temp = convert.decimalToBinary(regs[rs1], 32);

                for (int i = 16; i < 32; i++)
                {
                    half_word += temp[i];
                }
                stackFrames.push_back({ convert.decimalTohexa(stoi(offset) + regs[rd]),convert.decimalToBinary(convert.binaryToDecimal(half_word),32) });
            }
        }

        else
        {

            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rd]))
                {
                    string temp = convert.decimalToBinary(regs[rs1], 32);

                    for (int i = 16; i < 32; i++)
                    {
                        half_word += temp[i];
                    }

                    memoryContents[i][1] = convert.decimalToBinary(convert.binaryToDecimal(half_word), 32);
                    return 0;
                }
            }
            string temp = convert.decimalToBinary(regs[rs1], 32);

            for (int i = 16; i < 32; i++)
            {
                half_word += temp[i];
            }
            memoryContents.push_back({ convert.decimalTohexa(stoi(offset) + regs[rd]),convert.decimalToBinary(convert.binaryToDecimal(half_word),32) });
        }
        break;
    }
    case 30: // SLL
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];
        regs[rd] = regs[rs1] * pow(2, regs[rs2]);
        break;
    }
    case 31: // SLLI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }

        rd = reg[0];

        rs1 = reg[1];
        regs[rd] = regs[rs1] * pow(2, stoi(word));
        break;
    }

    case 32: // SLT
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }

        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];

        if (regs[rs1] < regs[rs2])
        {
            regs[rd] = 1;
        }
        else
        {
            regs[rd] = 0;
        }
        break;
    }

    case 33: // SLTI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];

        if (regs[rs1] < stoi(word))
        {
            regs[rd] = 1;
        }
        else
        {
            regs[rd] = 0;
        }
        break;
    }

    case 34: // SLTIU
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);

        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];

        int temp_1 = regs[rs1];
        int temp_2;

        if (temp_1 < 0)
        {
            temp_1 *= -1;
        }

        if (stoi(word) < 0)
        {
            temp_2 = stoi(word) * -1;
        }

        if (temp_1 < temp_2)
        {
            regs[rd] = 1;
        }
        else
        {
            regs[rd] = 0;
        }

        break;
    }

    case 35: // SLTU
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }

        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];

        int temp_1 = regs[rs1];
        int temp_2 = regs[rs2];

        if (temp_1 < 0)
        {
            temp_1 *= -1;
        }
        if (temp_2 < 0)
        {
            temp_2 *= -1;
        }


        if (temp_1 < temp_2)
        {
            regs[rd] = 1;
        }
        else
        {
            regs[rd] = 0;
        }
        break;
    }

    case 36: //SLTZ PSEUDO-INSTRUCTION
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            if (word == ",")
            {
                iss >> word;
            }
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }

        rd = reg[0];
        rs1 = reg[1];

        if (regs[rs1] < 0)
        {
            regs[rd] = 1;
        }
        else
        {
            regs[rd] = 0;
        }
        break;
    }

    case 37: // SRA
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }

        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];

        string x = convert.decimalToBinary(regs[rs1], 32);

        char temp = x[0];

        for (int i = 0; i < regs[rs2]; i++)
        {
            x.pop_back();
        }

        for (int i = 0; i < regs[rs2]; i++)
        {
            if (temp == '1')
            {
                x.insert(0, "1");
            }
            else
            {
                x.insert(0, "0");
            }
        }
        regs[rd] = convert.binaryToDecimal(x);
        break;
    }

    case 38: // SRAI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }

        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];

        string x = convert.decimalToBinary(regs[rs1], 32);

        char temp = x[0];

        for (int i = 0; i < stoi(word); i++)
        {
            x.pop_back();
        }

        for (int i = 0; i < stoi(word); i++)
        {
            if (temp == '1')
            {
                x.insert(0, "1");
            }
            else
            {
                x.insert(0, "0");
            }
        }
        regs[rd] = convert.binaryToDecimal(x);

        break;
    }
    case 39: // SRL
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];
        regs[rd] = regs[rs1] / pow(2, regs[rs2]);

        break;
    }
    case 40: // SRLI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }

        rd = reg[0];

        rs1 = reg[1];
        regs[rd] = regs[rs1] / pow(2, stoi(word));

        break;
    }

    case 41: // SUB
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];
        regs[rd] = regs[rs1] - regs[rs2];

        break;
    }

    case 42: // SW
    {
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        word = stripComma(word);
        reg[0] = RegisterIndex(word);
        rs1 = reg[0];
        if (reg[0] == -1)
        {
            return -1;
        }
        iss >> word;
        if (word == ",")
        {
            iss >> word;
        }
        int n = 0;
        string offset = "";
        while (word[n] != '(')
        {
            offset += word[n];
            n++;
        }
        string regis = "";
        n++;
        while (word[n] != ')')
        {
            regis += word[n];
            n++;
        }
        rd = RegisterIndex(regis);

        if (rd == -1)
        {
            return -1;
        }

        if (regis == "sp")
        {
            if (INT_MAX - regs[2] - stoi(offset) <= 0)
            {
                cout << "invalid stackFrames location\n";
                return -1;;
            }
            else
            {
                for (int i = 0; i < stackFrames.size(); i++)
                {
                    if (stackFrames[i][0] == convert.decimalTohexa(stoi(offset) + regs[rd]))
                    {

                        stackFrames[i][1] = convert.decimalToBinary(regs[rs1], 32);
                        return 0;
                    }
                }
                stackFrames.push_back({ convert.decimalTohexa(stoi(offset) + regs[rd]),convert.decimalToBinary(regs[rs1],32) });
            }
        }

        else
        {

            for (int i = 0; i < memoryContents.size(); i++)
            {
                if (memoryContents[i][0] == convert.decimalTohexa(stoi(offset) + regs[rd]))
                {
                    memoryContents[i][0] = convert.decimalToBinary(regs[rs1], 32);
                    return 0;
                }
            }
            memoryContents.push_back({ convert.decimalTohexa(stoi(offset) + regs[rd]),convert.decimalToBinary(regs[rs1],32) });
        }
        break;
    }

    case 43: // XOR
    {
        for (int i = 0; i < 3; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        rs2 = reg[2];
        int v1 = regs[rs1];
        int v2 = regs[rs2];
        string bin1 = convert.decimalToBinary(v1, 32);
        string bin2 = convert.decimalToBinary(v2, 32);
        string bin3 = "";

        for (int i = 0; i < 32; i++)
        {
            if (((bin1[i] == '1') && (bin2[i] == '0')) || ((bin1[i] == '0') && (bin2[i] == '1')))
            {
                bin3 += '1';
            }
            else
            {
                bin3 += '0';
            }
        }
        regs[rd] = convert.binaryToDecimal(bin3);

        break;
    }

    case 44: // XORI
    {
        for (int i = 0; i < 2; i++)
        {
            iss >> word;
            word = stripComma(word);
            reg[i] = RegisterIndex(word);
            if (reg[i] == -1)
            {
                return -1;
            }
        }
        iss >> word;
        word = stripComma(word);
        if (reg[0] == 0)
        {
            return -1;
        }
        rd = reg[0];
        rs1 = reg[1];
        int v1 = regs[rs1];
        int v2 = regs[stoi(word)];
        string bin1 = convert.decimalToBinary(v1, 32);
        string bin2 = convert.decimalToBinary(v2, 32);
        string bin3 = "";
        for (int i = 0; i < 32; i++)
        {
            if (((bin1[i] == '1') && (bin2[i] == '0')) || ((bin1[i] == '0') && (bin2[i] == '1')))
            {
                bin3 += '1';
            }
            else
            {
                bin3 += '0';
            }
        }
        regs[rd] = convert.binaryToDecimal(bin3);
        break;
    }

    default:
    {
        break;
    }
    }

    return 0;
}

void simulator::loadProgram(const string& filename)
{
    Radix convert;
    fill(regs, regs + 32, 0);
    memoryContents.clear();
    stackFrames.clear();
    memoryStore.clear();
    regs[2] = INT_MAX;
    string read;
    ifstream my_file(filename);
    int x, i = 0;
    while (getline(my_file, read))
    {
        memoryStore.push_back(read);
        istringstream iss(read);
        string word;
        iss >> word;
        if (read.empty())
        {
            i++;
            continue;
        }
        else if (word[0] == '#')
        {
            i++;
            continue;
        }
        else if (!word.empty() && word.back() == ':') {
            stripColon(word);
            memoryContents.push_back({ convert.decimalTohexa(i * 4), word });
            i++;
            continue;
        }
        else if (word == ".data")
        {
            while (word != ".text")
            {
                getline(my_file, read);
                memoryStore.push_back(read);
                istringstream iss(read);
                string word;
                iss >> word;
                stripColon(word);
                string name, type;
                name = word;
                iss >> type;
                if (type == ".string")
                {
                    string message = "";
                    while (iss >> word)
                    {
                        message += word;
                    }
                    stripCharacter(message, '"');
                    memoryContents.push_back({ convert.decimalTohexa(i * 4),name, message });
                }
                else
                {
                    cout << "format not supported\n";
                }
                i++;

                break;
            }
        }
        i++;
    }
    my_file.close();
    for (programCounter; programCounter < memoryStore.size(); programCounter++)
    {
        istringstream iss(memoryStore[programCounter]);
        string word;
        iss >> word;
        if (memoryStore[programCounter].empty())
        {
            continue;
        }
        if (word[0] == '#')
        {
            continue;
        }
        else if (!word.empty() && word.back() == ':') {
            continue;
        }
        else if (word == ".data")
        {
            while (memoryStore[programCounter] != ".text")
            {
                programCounter++;
            }
            continue;
        }
        int z = FindString(word, 45);
        if (z == -1)
        {
            cout << "ERROR! Invalid instruction, program terminated.\n";
            break;
        }
        x = Run(z, memoryStore[programCounter]);
        print();
        if (x == -1)
        {
            cout << "ERROR! Invalid instruction, program terminated.\n";
            break;
        }
        if (x == -2)
        {
            break;
        }
    }
}