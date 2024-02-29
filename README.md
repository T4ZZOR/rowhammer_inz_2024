# rowhammer_inz_2024

Repository created for the purpose of writing an engineering paper on the Rowhammer attack

### test info
The attack was carried out mainly on the Kali-Linux 2023.3 x64 operating system and on systems: Xubuntu, Lubuntu, Linux Mint.

### c_program_dynamic
The program dynamically allocates blocks of memory (at the user's choice), and then enters random data **int** from the range 0-999 into the allocated area. After the correct allocation, the program creates a copy of the data, so that you can compare blocks of data to each other to check the occurrence of changes - rowhammer attack. The program is divided into two phases: reading and writing. The first phase reads the data extensively to cause interference. The second phase writes data into rows which can also cause interference. After each attack on a row in a block of data, the progam checks whether the data is consistent with itself. If changes are detected, the program terminates, if not - the next rows of the allocated area are attacked.

### c_program_simulation -. 
A program that is exactly the same as **c_program_dynamic**, whose task is to simulate a rowhammer attack. While the program is running, a random number of iterations is generated for the error to occur. When this number is reached, one value in the data block is changed, making the program detect the attack.

### assembler_program
The program allocates a small array of data and then performs a write operation and a read operation on the data. The program runs until a change in the data is detected.