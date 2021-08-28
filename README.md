Subnet Calculator Documentation -- 7/13/2020

Compilation: 
You must have have gcc or clang installed with support for c++ 11 or newer
you may compile with gcc using "g++ -o scalc.exe SubnetCalc.cpp"
This will create scalc.exe

Usage:
After compilation, the program may be ran in the same directory with the following formats depending on your shell:
scalc "network address" "subnet mask"
or
./scalc.exe "network address" "subnet mask"

alternatively, you may simply launch using 
scalc
or
./scalc.exe
and you will be prompted for the required information.



Output will display first 8 networks, if there are fewer than 8 networks then all networks will be displayed.



example: 

input: .\a.exe 10.16.10.0 255.255.255.192

output: 
General Network Information
---------------------------------------------
Network address : 10.16.10.0
Subnet mask: 255.255.255.192
Number of Subnets: 262144
Number of Hosts per Subnet: 62
---------------------------------------------
Subnet:      10.0.0.0         10.0.0.64        10.0.0.128       10.0.0.192       10.0.1.0         10.0.1.64        10.0.1.128       10.0.1.192
First Host:  10.0.0.1         10.0.0.65        10.0.0.129       10.0.0.193       10.0.1.1         10.0.1.65        10.0.1.129       10.0.1.193
Last Host:   10.0.0.62        10.0.0.126       10.0.0.190       10.0.0.254       10.0.1.62        10.0.1.126       10.0.1.190       10.0.1.254
Broadcast:   10.0.0.63        10.0.0.127       10.0.0.191       10.0.0.255       10.0.1.63        10.0.1.127       10.0.1.191       10.0.1.255
---------------------------------------------
