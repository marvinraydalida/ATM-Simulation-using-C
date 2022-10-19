# ATM-Simulation-using-C

Basic C program to Simulate an Automated teller machine, which utilizes a Flash Drive as an alternative for a real ATM Card.

Run the ```registration.exe``` to register an account and register the Flash Drive.

Run the transaction.exe to make transactions which include:
  - Withdraw
  - Deposit
  - Balance Inquiry
  - Fund Transfer
  - Change Pin

The Accounts Database File, and bank database file inside the flash drive, will be automatically created.

**Important Note:** If the program suddenly notifies you that the Flash Drive is registered or not, without inserting the Flash Drive yet,
or if you inserted your flash drive already but is not recognized. try changing the drive letters in both `Registration Module.c` 
and `Transaction Module.c` and compile it in Code Blocks. Maybe `D:` drive is your back up storage.


Below are the Global variables, which are present in both transaction and registration .c files, that contains the drive letters:
```C 
char* checkDrives[2] = {"D:\\checkFd.dbf", "G:\\checkFd.dbf"};
char* drives[2] = {"D:\\bank.dbf","G:\\bank.dbf"};
```
You may change it to your flash drive letter so that your flash drive will be recognized.
  

