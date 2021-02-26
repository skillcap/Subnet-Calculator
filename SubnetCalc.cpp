/*
    Subnet Calculator (std:: version)
    Created by Nathaniel Morrow
    Last modified: 8/14/2020
*/

/*  TODO:
    case 'A' optimizations
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>

//using namespace std;

//function prototypes
unsigned short* parseIP(std::string s);
void errorAndQuit();

int main(int argc, char *argv[])
{
    //validation part one, proper syntax
    unsigned short *address;
    unsigned short *mask;
    switch (argc){
        case 1:{
            std::cout << "Enter a (private) IPv4 Network Address: ";
            std::string nw = "";
            std::cin >> nw;
            address = parseIP(nw);
            std::cout << "Enter a Subnet Mask: ";
            std::string msk = "";
            std::cin >> msk;
            mask = parseIP(msk);
            break;
        }
        case 3:
            //parse network address and mask into arrays of 4 short values, use periods to separate each value.
            //validation part two, proper IPv4 format
            address = parseIP(argv[1]);
            mask = parseIP(argv[2]);
            break;
        default:
            errorAndQuit();
    }

    //DETERMINE class
    //validation part 3, private address
    int xModifier = 0; //class C size
    int yModifier = 0;
    char addressClass = 'C';
    int octet = 3;
    switch (address[0])
    {
    case 192: //class C
        if (address[1] != 168)
            std::cout << "Public address detected, please enter a valid private IPv4 adress!\n";
        //proceed with single value subnet and default values.
        break;
    case 172: //class B
        if (address[1] < 16 || address[1] > 31){
            std::cout << "Public address detected, please enter a valid private IPv4 adress!\n";
            errorAndQuit();
        } else {
            addressClass = 'B';
            if (mask[3] == 0){
                yModifier = 8;
                octet = 2;
            } else {
                xModifier = 8;
            }
        }
        break;
    case 10: //class A
        //proceed with 3-value subnet
        addressClass = 'A';
        if (mask[2] == 0){
            yModifier = 16;
            octet = 1;
        } else if (mask[3] == 0){
            yModifier = 8;
            xModifier = 8;
            octet = 2;
        } else { //class 
            xModifier = 16;
        }
        break;
    default:
        std::cout << "Public address detected, please enter a valid private IPv4 adress!\n";
        errorAndQuit();
        break;
    }

    int blockSize = 256 - mask[octet];

    int x = 0; //masked bits   1's networks
    int y = 8; //unmasked bits 0's hosts
    int tableSize = 2;

    //determine total number of masked/unmasked bits
    switch (mask[octet]) {
        case 0:    // 00000000
            break;
        case 128: // 10000000
            x = 1 + xModifier,
            y = 7 + yModifier;
            break;
        case 192: // 11000000
            x = 2 + xModifier,
            y = 6 + yModifier;
            break;
        case 224: // 11100000
            x = 3 + xModifier,
            y = 5 + yModifier;
            break;
        case 240: // 11110000
            x = 4 + xModifier,
            y = 4 + yModifier;
            break;
        case 248: // 11111000
            x = 5 + xModifier,
            y = 3 + yModifier;
            break;
        case 252: // 11111100
            x = 6 + xModifier,
            y = 2 + yModifier;
            break;
        case 254: // 11111110
            x = 7 + xModifier,
            y = 1 + yModifier;
            break;
        case 255: // 11111111
            x = 8 + xModifier;
            y = 0 + yModifier;
            if (octet != 3) //255.255.255.255 is an invalid mask
                break;
        default:
            std::cout << "invalid mask entered\n";
            errorAndQuit();
        break;
    }

    //calculate # of subnets
    //2^x (x = masked bits)
    int numSubnets = pow(2, x);
    if(numSubnets > 8)
        tableSize = 8;
    else
        tableSize = numSubnets;
    //calculate # of hosts per subnet
    //2^y-2 (y = unmasked bits)
    int hosts = pow(2, y) - 2;

    std::vector<std::string> subnets;
    std::vector<std::string> broadcasts;
    std::vector<std::string> firsts;
    std::vector<std::string> lasts;
    std::string temp;

    unsigned short *tempSubnet;
    unsigned short *tempBroadcast;
    switch(addressClass){ //can make temp subent array just size 3, use const 10 or address value or addr[0]
        case 'A':
            tempSubnet = new unsigned short[4];
            tempSubnet[0] = 10;
            tempSubnet[1] = 0;
            tempSubnet[2] = 0;
            tempSubnet[3] = 0;
            tempBroadcast = new unsigned short[4];
            tempBroadcast[0] = 10;
            tempBroadcast[1] = 0;
            tempBroadcast[2] = 0;
            tempBroadcast[octet] = blockSize-1;
            if (octet<3){
                tempBroadcast[octet+1] = 255;
                if (octet==1){
                    tempBroadcast[octet+2] = 255;
                }
            }
            subnets.push_back(std::to_string(tempSubnet[0])       + '.' + std::to_string(tempSubnet[1])    + '.' + std::to_string(tempSubnet[2])    + '.' + std::to_string(tempSubnet[3]));
            broadcasts.push_back(std::to_string(tempBroadcast[0]) + '.' + std::to_string(tempBroadcast[1]) + '.' + std::to_string(tempBroadcast[2]) + '.' + std::to_string(tempBroadcast[3]));
            firsts.push_back(std::to_string(tempSubnet[0])        + '.' + std::to_string(tempSubnet[1])    + '.' + std::to_string(tempSubnet[2])    + '.' + std::to_string(tempSubnet[3] + 1));
            lasts.push_back(std::to_string(tempBroadcast[0])      + '.' + std::to_string(tempBroadcast[1]) + '.' + std::to_string(tempBroadcast[2]) + '.' + std::to_string(tempBroadcast[3] - 1));
            for (int i = 1; i < tableSize; i++){   
                tempSubnet[octet] += blockSize;
                
                if (tempSubnet[octet] == 256){
                    tempSubnet[octet] = 0;
                    tempSubnet[octet-1]++;
                    tempBroadcast[octet] = blockSize-1;
                    tempBroadcast[octet-1]++;
                } else {
                    tempBroadcast[octet] += blockSize;
                }
                subnets.push_back(std::to_string(tempSubnet[0])       + '.' + std::to_string(tempSubnet[1])    + '.' + std::to_string(tempSubnet[2])    + '.' + std::to_string(tempSubnet[3]));
                broadcasts.push_back(std::to_string(tempBroadcast[0]) + '.' + std::to_string(tempBroadcast[1]) + '.' + std::to_string(tempBroadcast[2]) + '.' + std::to_string(tempBroadcast[3]));
                firsts.push_back(std::to_string(tempSubnet[0])        + '.' + std::to_string(tempSubnet[1])    + '.' + std::to_string(tempSubnet[2])    + '.' + std::to_string(tempSubnet[3]+1));
                lasts.push_back(std::to_string(tempBroadcast[0])      + '.' + std::to_string(tempBroadcast[1]) + '.' + std::to_string(tempBroadcast[2]) + '.' + std::to_string(tempBroadcast[3]-1));
            }
            break;
        case 'B':
            tempSubnet = new unsigned short[2];
            tempSubnet[0] = 0;
            tempSubnet[1] = 0;
            tempBroadcast = new unsigned short[2];
            if(octet == 2){
                tempBroadcast[0] = blockSize-1;
                tempBroadcast[1] = 255;
            } else {
                tempBroadcast[0] = 0;
                tempBroadcast[1] = blockSize-1;
            }

            subnets.push_back(std::to_string(address[0])    + '.' + std::to_string(address[1]) + '.' + std::to_string(tempSubnet[0]) + '.' + std::to_string(tempSubnet[1]));
            broadcasts.push_back(std::to_string(address[0]) + '.' + std::to_string(address[1]) + '.' + std::to_string(tempBroadcast[0]) + '.' + std::to_string(tempBroadcast[1]));
            firsts.push_back(std::to_string(address[0])     + '.' + std::to_string(address[1]) + '.' + std::to_string(tempSubnet[0]) + '.' + std::to_string(tempSubnet[1] + 1));
            lasts.push_back(std::to_string(address[0])      + '.' + std::to_string(address[1]) + '.' + std::to_string(tempBroadcast[0]) + '.' + std::to_string(tempBroadcast[1] - 1));

            for (int i = 1; i < tableSize; i++){
                
                if(octet == 2){
                    tempSubnet[0] += blockSize;
                    tempBroadcast[0] += blockSize;
                } else {
                    tempSubnet[1] += blockSize;
                    if (tempSubnet[1] == 256){
                        tempSubnet[1] = 0;
                        tempSubnet[0]++;
                        tempBroadcast[1] = blockSize - 1;
                        tempBroadcast[0]++;
                    } else {
                        tempBroadcast[1] += blockSize;
                    }
                }
                subnets.push_back(std::to_string(address[0])    + '.' + std::to_string(address[1]) + '.' + std::to_string(tempSubnet[0]) + '.' + std::to_string(tempSubnet[1]));
                broadcasts.push_back(std::to_string(address[0]) + '.' + std::to_string(address[1]) + '.' + std::to_string(tempBroadcast[0]) + '.' + std::to_string(tempBroadcast[1]));
                firsts.push_back(std::to_string(address[0])     + '.' + std::to_string(address[1]) + '.' + std::to_string(tempSubnet[0]) + '.' + std::to_string(tempSubnet[1] + 1));
                lasts.push_back(std::to_string(address[0])      + '.' + std::to_string(address[1]) + '.' + std::to_string(tempBroadcast[0]) + '.' + std::to_string(tempBroadcast[1] - 1));
            }
            break;
        case 'C':
            tempSubnet = new unsigned short[1];
            tempSubnet[0] = 0;
            tempBroadcast = new unsigned short[1];
            tempBroadcast[0] = blockSize-1;
            subnets.push_back(std::to_string(address[0])    + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempSubnet[0]));
            broadcasts.push_back(std::to_string(address[0]) + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempBroadcast[0]));
            firsts.push_back(std::to_string(address[0])     + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempSubnet[0] + 1));
            lasts.push_back(std::to_string(address[0])      + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempBroadcast[0] - 1));
            for(int i = 1 ; i < tableSize; i++){
                tempSubnet[0] += blockSize;
                tempBroadcast[0] += blockSize;
                subnets.push_back(std::to_string(address[0])    + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempSubnet[0]));
                broadcasts.push_back(std::to_string(address[0]) + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempBroadcast[0]));
                firsts.push_back(std::to_string(address[0])     + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempSubnet[0] + 1));
                lasts.push_back(std::to_string(address[0])      + '.' + std::to_string(address[1]) + '.' + std::to_string(address[2]) + '.' + std::to_string(tempBroadcast[0] - 1));
            }
            break;
    }
    delete tempSubnet;
    delete tempBroadcast;

    std::string line = "---------------";
    //output table and information:
    std::cout << "General Network Information" << std::endl
              << "---------------------------------\n"
              << "Network address : "           << address[0] << "." << address[1] << "." << address[2] << "." << address[3] << std::endl
              << "Subnet mask: "                << mask[0] << "." << mask[1] << "." << mask[2] << "." << mask[3] << std::endl
              << "Number of Subnets: "          << numSubnets << std::endl
              << "Number of Hosts per Subnet: " << hosts << std::endl
              << "-------------";
    switch(tableSize){
        case 1:
            std::cout << line << std::endl
                      << "Subnet:      " << std::left << std::setw(17) << subnets[0] << std::endl
                      << "First Host:  " << std::left << std::setw(17) << firsts[0] << std::endl
                      << "Last Host:   " << std::left << std::setw(17) << lasts[0] << std::endl
                      << "Broadcast:   " << std::left << std::setw(17) << broadcasts[0] << std::endl
                      << line;
            break;
        case 2:
            std::cout << line << line << line << std::endl
                      << "Subnet:          " << std::left << std::setw(17) << subnets[0] << std::left << std::setw(17) << subnets[1] << std::left << std::setw(17) << std::endl
                      << "First Host:  " << std::left << std::setw(17) << firsts[0] << std::left << std::setw(17) << firsts[1] << std::left << std::setw(17) << std::endl
                      << "Last Host:   " << std::left << std::setw(17) << lasts[0] << std::left << std::setw(17) << lasts[1] << std::left << std::setw(17) << std::endl
                      << "Broadcast:   " << std::left << std::setw(17) << broadcasts[0] << std::left << std::setw(17) << broadcasts[1] << std::endl
                      << line << line << line;
            break;
        case 4:
            std::cout << line << line << line << line << line << std::endl
                      << "Subnet:      " << std::left << std::setw(17) << subnets[0] << std::left << std::setw(17) << subnets[1] << std::left << std::setw(17) << subnets[2] << std::left << std::setw(17) << subnets[3] << std::endl
                      << "First Host:  " << std::left << std::setw(17) << firsts[0] << std::left << std::setw(17) << firsts[1] << std::left << std::setw(17) << firsts[2] << std::left << std::setw(17) << firsts[3] << std::endl
                      << "Last Host:   " << std::left << std::setw(17) << lasts[0] << std::left << std::setw(17) << lasts[1] << std::left << std::setw(17) << lasts[2] << std::left << std::setw(17) << lasts[3] << std::endl
                      << "Broadcast:   " << std::left << std::setw(17) << broadcasts[0] << std::left << std::setw(17) << broadcasts[1] << std::left << std::setw(17) << broadcasts[2] << std::left << std::setw(17) << broadcasts[3] << std::endl
                      << line << line << line << line << line;
            break;
        case 8:
            std::cout << line << line << line << line << line << line << line << line << line << std::endl
                      << "Subnet:      " << std::left << std::setw(17) << subnets[0] << std::left << std::setw(17) << subnets[1] << std::left << std::setw(17) << subnets[2] << std::left << std::setw(17) << subnets[3] << std::left << std::setw(17) << subnets[4] << std::left << std::setw(17) << subnets[5] << std::left << std::setw(17) << subnets[6] << std::left << std::setw(17) << subnets[7] << std::endl
                      << "First Host:  " << std::left << std::setw(17) << firsts[0] << std::left << std::setw(17) << firsts[1] << std::left << std::setw(17) << firsts[2] << std::left << std::setw(17) << firsts[3] << std::left << std::setw(17) << firsts[4] << std::left << std::setw(17) << firsts[5] << std::left << std::setw(17) << firsts[6] << std::left << std::setw(17) << firsts[7] << std::endl
                      << "Last Host:   " << std::left << std::setw(17) << lasts[0] << std::left << std::setw(17) << lasts[1] << std::left << std::setw(17) << lasts[2] << std::left << std::setw(17) << lasts[3] << std::left << std::setw(17) << lasts[4] << std::left << std::setw(17) << lasts[5] << std::left << std::setw(17) << lasts[6] << std::left << std::setw(17) << lasts[7] << std::endl
                      << "Broadcast:   " << std::left << std::setw(17) << broadcasts[0] << std::left << std::setw(17) << broadcasts[1] << std::left << std::setw(17) << broadcasts[2] << std::left << std::setw(17) << broadcasts[3] << std::left << std::setw(17) << broadcasts[4] << std::left << std::setw(17) << broadcasts[5] << std::left << std::setw(17) << broadcasts[6] << std::left << std::setw(17) << broadcasts[7] << std::endl
                      << line << line << line << line << line << line << line << line << line;
            break;
    };
    std::cout << "-------------";
    delete address;
    delete mask;
    return 0;
}

/*
    Name: parseIP
    Function: Provided an IPv4 network address or mask in string form
    will return an array of length 4 with corresponding values.
    validates an ipv4 address, exits if invalid.

    RETURNS ALLOCATED MEMORY, DELETE!
*/
unsigned short* parseIP(std::string s){
    unsigned short* address = new unsigned short[4];
    std::string temp = "";
    for (int i = 0; i < 4; i++){
        temp = s.substr(0, s.find('.'));
        address[i] = stoi(temp); //gotta love c++11
        if ((i < 3 && s.find('.') == - 1) || address[i] > 255){ //not fool entirely, 21abc4 is read as 21
            std::cout << "Invalid IPv4 address!\n";
            errorAndQuit();
        }
        s = s.substr(s.find('.') + 1,s.size());
    }
    return address;
}

void errorAndQuit(){
    std::cout << "Improper usage, \n"
         << "please provide a network adress and subnet mask as arguments in the form\n"
         << "\"network address\", \"subnet mask\"\n"
         << "example: ~.exe 192.168.10.0 255.255.255.0";
    exit(1);
}
