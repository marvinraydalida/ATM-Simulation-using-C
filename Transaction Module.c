#include <stdio.h>
#include <stdlib.h>
#include <string.h> //string functions
#include <windows.h>//for gotoxy
#include <time.h>   //for loading animation
#include <conio.h>  //for getch()
#include <ctype.h>  //for isdigit()
#define MAX 500

typedef struct data{
    char name[50];
    char pin[7];
    int acountNo;
    int balance;
    int last;
}list;

list l;
list accountInfo[MAX];

//GLOBAL VARIABLES
char inputPin[7];
char regPin[7];
char confirmPin[7];
char pinFromCard[7];
char* checkDrives[2] = {"D:\\checkFd.dbf", "G:\\checkFd.dbf"};
char* drives[2] = {"D:\\bank.dbf","G:\\bank.dbf"};
int accountNoFromCard, regAccountNo, regBalance, accountIndex, otherAccIndex, ctr = -1, exist = 0, checkName = 0, checkPin = 0, pinCharacter = 0;

void makenull();
void retrieve();
int insertCard();
int checkFd();
void pinEnter();
int transaction();
int isEmpty();
int isFull();
void insert();
int locate();
void save();
void saveToCard();
void checkPinFromCard();
int confirmTransaction(int x, int y, int len);

//UI AND ANIMATION
void delay(int ms);
void insertCardAnimation();
void transactionUi();
void pinUi();
void gotoxy(int x, int y);
void printToxy(int x, int y, char* label);
COORD coord = {0,0};
void createBlock(int x, int y, int len, char *label);
void clear(char *label);
void fdRemoved();

int main()
{
    int i;
    system("color 06");
    makenull();
    retrieve();
    if(insertCard()){
        pinUi();
        pinEnter();
        accountIndex = locate(1);
        do{
            _getch();
            system("cls");
            system("color 06");
            transactionUi();
        }while(transaction());
    }
    i = remove(checkDrives[ctr]);
    gotoxy(0,25);
    printf("Submitted By: Dalida, Marvin Ray I.\tBSCS - 2A(NS)\n\nData Structure And Algorithm Lecture & Laboratory\n\nSubmitted to: Prof. Fernando Renegado & Prof. Francis Alfaro\n");
    _getch();
    return 0;
}

void makenull(){
    l.last = -1;
}

void retrieve(){
    FILE *fp;
    int i = 0;
    fp = fopen("accdb.dbf","r+");

    if (fp==NULL){
        createBlock(30, 10, 90, " ");
        printf("D A T A B A S E   I S   E M P T Y");
        delay(600);
        printToxy(32,12,"                                                       ");
        printToxy(42,12," R E G I S T E R  F I R S T . . .");
        _getch();
        gotoxy(1,30);
        exit(0);
    }
    else{
        while(!feof(fp)){
            l.last++;
            fscanf(fp,"%d %[^\t]%d %[^\n]",&accountInfo[l.last].acountNo,accountInfo[l.last].name,&accountInfo[l.last].balance,accountInfo[l.last].pin);
        }
    }
    fclose(fp);
}

int insertCard(){
    FILE *fp;
    printToxy(44,2,"I N S E R T   C A R D . . .");
    createBlock(40, 7, 72, "");

    do{
        ctr = (ctr + 1) % 2;
        fp = fopen(checkDrives[ctr],"w");
        insertCardAnimation();
    }while(fp == NULL);
    fclose(fp);

    fp = fopen(drives[ctr],"r");
    if(fp == NULL){
        printToxy(42,9,"N O T   R E G I S T E R E D .");
        return 0;
    }
    else{
        while(!feof(fp))
            fscanf(fp,"%d %[^\n]",&accountNoFromCard,pinFromCard);

        for(int i = 0; i < l.last + 1; i++){
            if(strcmp(accountInfo[i].pin,pinFromCard) == 0 && accountInfo[i].acountNo == accountNoFromCard){
                printToxy(46,9,"R E G I S T E R E D");
                return 1;
            }
        }
        printToxy(42,9,"N O T   R E G I S T E R E D .");
        return 0;
    }
    fclose(fp);
}

int checkFd(){
    FILE *fp;
    fp = fopen(checkDrives[ctr],"r");
    if(fp == NULL){
        clear("result2");
        printToxy(43,22,"F L A S H   D R I V E   E R R O R");
        fdRemoved();
        gotoxy(0,25);
        return 1;
    }
    return 0;
    fclose(fp);
}

void pinEnter(){
    int i = 0;
    do{
        if(i == 3){
            printToxy(46,17," E X I T I N G . . .");
            delay(2000);
            gotoxy(0,20);
            exit(0);
        }
        gotoxy(52, 13);
        for(int i = 0; i < 6; i++){
            inputPin[i] = _getch() + 9;
            putchar('*');
            putchar(' ');
        }
        inputPin[6] = '\0';

        if(strcmp(inputPin,pinFromCard) != 0){
            printToxy(44,17,"I N V A L I D   P I N");
            _getch();
            clear("result");
            clear("pinBoxes");
        }
        i++;
    }while(strcmp(inputPin,pinFromCard) != 0);
    printToxy(46,17,"W E L C O M E ! ! !");
}

int locate(int x){

    if(x == 0){
        for(int i = 0; i < l.last + 1; i++){
            if(strcmp(accountInfo[i].pin,regPin) == 0){
                checkPin = 1;
                return 1;
            }
        }
        return 0;
    }
    else if(x == 1){
        for(int i = 0; i < l.last + 1; i++){
            if(accountInfo[i].acountNo == accountNoFromCard)
                return i;
        }
        return 0;
   }
   else if(x == 2){
        for(int i = 0; i < l.last + 1; i++){
            if(accountInfo[i].acountNo == regAccountNo){
                otherAccIndex = i;
                return 0;
            }
        }
        return 1;
   }
}

int transaction(){
    char choice;
    int end;
    gotoxy(41,12);
    scanf(" %c",&choice);
    if(choice == 'a' || choice == 'A'){
        if(accountInfo[accountIndex].balance != 0){
            do{
                clear("result2");
                clear("enter");
                printToxy(36,22,"E N T E R   A M O U N T   T O   W I T H D R A W");
                printToxy(31,18,"Enter Amount: Php  ");
                scanf("%d",&regBalance);
                if(regBalance > accountInfo[accountIndex].balance){
                    printToxy(33,22,"A M O U N T   G R E A T E R   T H A N   B A L A N C E");
                    _getch();
                }
                else if(regBalance < 0){
                    clear("result2");
                    printToxy(45,22,"N E G A T I V E   E N T R Y");
                    _getch();
                }
                else if(regBalance == 0){
                    clear("result2");
                    printToxy(45,22,"  I N V A L I D   E N T R Y");
                    _getch();
                }
            }while(regBalance > accountInfo[accountIndex].balance || regBalance < 0 || regBalance == 0);
            if(confirmTransaction(44,11,76)){
                if(checkFd()){
                    exit(0);
                }
                accountInfo[accountIndex].balance -= regBalance;
                system("color 02");
                printToxy(50,22,"S U C C E S S ! ! !");
            }
            else
                printToxy(50,22,"C A N C E L L E D");
        }
        else
            printToxy(50,22,"N O   B A L A N C E");
    }
    else if(choice == 'b' || choice == 'B'){
        do{
            clear("result2");
            clear("enter");
            printToxy(37,22,"E N T E R   A M O U N T   T O   D E P O S I T");
            printToxy(31,18,"Enter Amount: Php  ");
            scanf("%d",&regBalance);
            if(regBalance < 0){
                clear("result2");
                printToxy(45,22,"N E G A T I V E   E N T R Y");
                _getch();
            }
            else if(regBalance == 0){
                clear("result2");
                printToxy(45,22,"  I N V A L I D   E N T R Y");
                _getch();
            }
        }while(regBalance < 0 || regBalance == 0);
        if(confirmTransaction(44,11,76)){
                if(checkFd()){
                    exit(0);
                }
                system("color 02");
                accountInfo[accountIndex].balance += regBalance;
                printToxy(50,22,"S U C C E S S ! ! !");
            }
        else
            printToxy(50,22,"C A N C E L L E D");
    }
    else if(choice == 'c' || choice == 'C'){
        if(checkFd()){
            exit(0);
        }
        printToxy(48,22,"Y O U R   B A L A N C E");
        gotoxy(31,18);
        printf("Balance: Php  %d", accountInfo[accountIndex].balance);
    }

    else if(choice == 'd' || choice == 'D'){
        if(l.last > 0){
            if(accountInfo[accountIndex].balance != 0){
                do{
                    otherAccIndex = -1;
                    clear("result2");
                    clear("enter");
                    printToxy(47,22,"F U N D   T R A N S F E R");
                    printToxy(31,18,"Enter Account No. you want to Transfer:  ");
                    scanf("%d",&regAccountNo);
                    if(locate(2)){
                        clear("result2");
                        printToxy(34,22,"A C C O U N T   N U M B E R   D O N \' T   E X I S T");
                        _getch();
                    }
                    else if(accountInfo[accountIndex].acountNo == accountInfo[otherAccIndex].acountNo){
                        otherAccIndex = -1;
                        clear("result2");
                        printToxy(37,22,"I N V A L I D   A C C O U N T   N U M B E R");
                        _getch();
                    }
                    else{
                        do{
                            clear("result2");
                            clear("enter");
                            printToxy(36,22,"E N T E R   A M O U N T   T O   T R A N S F E R");
                            printToxy(31,18,"Enter Amount: Php  ");
                            scanf("%d",&regBalance);
                            if(regBalance > accountInfo[accountIndex].balance){
                                clear("result2");
                                printToxy(33,22,"A M O U N T   G R E A T E R   T H A N   B A L A N C E");
                                _getch();
                            }
                            else if(regBalance < 0){
                                clear("result2");
                                printToxy(45,22,"N E G A T I V E   E N T R Y");
                                _getch();
                            }
                            else if(regBalance == 0){
                                clear("result2");
                                printToxy(45,22,"  I N V A L I D   E N T R Y");
                                _getch();
                            }
                        }while(regBalance < 0 || regBalance > accountInfo[accountIndex].balance || regBalance == 0);
                    }
                }while(otherAccIndex == -1);
                if(confirmTransaction(44,11,76)){
                    if(checkFd()){
                        exit(0);
                    }
                    system("color 02");
                    accountInfo[accountIndex].balance -= regBalance;
                    accountInfo[otherAccIndex].balance += regBalance;
                    printToxy(50,22,"S U C C E S S ! ! !");
                }
                else
                    printToxy(50,22,"C A N C E L L E D");
            }
            else
                printToxy(50,22,"N O   B A L A N C E");
        }
        else if(l.last == 0)
            printToxy(32,22,"N O   O T H E R   A C C O U N T   T O   T R A N S F E R");
    }

    else if(choice == 'e' || choice == 'E'){
        do{
            if(checkFd()){
                exit(0);
            }
            checkPinFromCard();
            do{
                clear("result2");
                clear("enter");
                printToxy(50,22,"C H A N G E   P I N");
                printToxy(31,18,"Enter Current PIN:  ");

                for(int i = 0; i < 6; i++){
                    inputPin[i] = _getch() + 9;
                    putchar('*');
                    putchar(' ');
                }
                if(strcmp(inputPin,pinFromCard) != 0){
                    clear("result2");
                    printToxy(48,22,"I N V A L I D   P I N");
                    _getch();
                }
            }while(strcmp(inputPin,pinFromCard) != 0);

            clear("result2");
            clear("enter");
            printToxy(31,18,"Enter new PIN:  ");
            for(int i = 0; i < 6; i++){
                regPin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            regPin[6] = '\0';

            clear("result2");
            clear("enter");
            printToxy(31,18,"Confirm new PIN:  ");
            for(int i = 0; i < 6; i++){
                confirmPin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            confirmPin[6] = '\0';

            end = strcmp(confirmPin,regPin);

            checkPin = 0;
            pinCharacter = 0;

            for(int i = 0; i < 6; i++){
                if(isdigit(confirmPin[i] - 9)  == 0){
                    pinCharacter = 1;
                    break;
                }
            }

            if(end != 0){
                clear("result2");
                printToxy(42,22,"P I N   D I D   N O T   M A T C H !");
                _getch();
            }

            else if(locate(0) && checkPin == 1){
                clear("result2");
                printToxy(46,22,"P I N   N O T   U N I Q U E");
                _getch();
            }

            else if(pinCharacter == 1){
                clear("result2");
                printToxy(36,22,"P I N   C O N T A I N S   A   C H A R A C T E R");
                _getch();
            }

        }while(end != 0 || checkPin || pinCharacter);
        if(checkFd()){
            exit(0);
        }
        system("color 02");
        strcpy(accountInfo[accountIndex].pin,regPin);
        clear("result2");
        printToxy(50,22,"S U C C E S S ! ! !");
        saveToCard();
    }

    else if(choice == 'x' || choice == 'X'){
        printToxy(39,22,"T H A N K   Y O U   F O R   U S I N G ! ! !");
        return 0;
    }
    else
        printToxy(45,22,"I N V A L I D   C H O I C E");
    save();
    return 1;
}

void checkPinFromCard(){
    FILE* fp;
    fp = fopen(drives[ctr],"r");
    while(!feof(fp))
            fscanf(fp,"%d %[^\n]",&accountNoFromCard,pinFromCard);
}

int confirmTransaction(int x, int y, int len){
    char input;
    for(int i = y; i < y + 8; i++)
        printToxy(x,i,"                                ");
    for(int i = x; i < len; i++){
        printToxy(i,y - 1,"_");
        printToxy(i,y + 2,"_");
        printToxy(i, y + 7,"_");
    }
    for(int i = y; i < y + 8; i++){
        printToxy(x - 1,i,"|");
        printToxy(len,i,"|");
    }
    printToxy(x + 6,y + 1,"CONFIRM TRANSACTION?");
    printToxy(x + 5,y + 5,"[Y] YES       [N] NO");
    do{
        input = _getch();
        if(input == 'Y' || input == 'y' || input == 'N' || input == 'n'){
            system("cls");
            transactionUi();
            if(input == 'Y' || input == 'y')
                return 1;
            else if(input == 'N' || input == 'n')
                return 0;
        }
    }while(input != 'Y' || input != 'y' || input != 'N' || input != 'n');
}

void save(){
    FILE *fp;
    fp = fopen("accDb.dbf","w");
    for(int i = 0; i <= l.last; i++){
        fprintf(fp,"%d\t%s\t%d\t%s",accountInfo[i].acountNo,accountInfo[i].name,accountInfo[i].balance,accountInfo[i].pin);
        if(i != l.last)
            fprintf(fp,"\n");
    }
    fclose(fp);
}

void saveToCard(){
    FILE *fp;
    fp = fopen(drives[ctr],"w");
    fprintf(fp,"%d\t%s",accountInfo[accountIndex].acountNo,regPin);
    fclose(fp);
}

void delay(int ms){
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

void insertCardAnimation(){
    char loadingCharacters[] = {'.','.','.','o','.','.','0','o','.','0','0','o','0','0','0','o','0','0','.','o','0','.','.','o','.','.','.','\0'};
    int x = 0, y = 1, z = 2, loop = 0;
    while(loop != 9){
        if(loop > 0){
            x += 3;
            y += 3;
            z += 3;
        }
        delay(130);
        printToxy(43,5,"         ");
        printf("%c   %c   %c",loadingCharacters[x],loadingCharacters[y],loadingCharacters[z]);
        gotoxy(72,10);
        loop++;
    }
}

void pinUi(){
    createBlock(25, 11, 39, "PIN:");
    createBlock(41, 11, 86, "");
    createBlock(25, 15, 86, "");
    printToxy(53,17,"- - -");
}

void transactionUi(){
    printToxy(48,2," T R A N S A C T I O N");
    createBlock(30, 4, 75, "Full Name:");
    createBlock(77, 4, 90, "No.:");
    gotoxy(42, 6);
    printf("%s",accountInfo[accountIndex].name);
    gotoxy(83, 6);
    printf("%d",accountInfo[accountIndex].acountNo);
    printToxy(31, 12,"Option: [   ]");
    for(int i = 30; i < 90; i++){
        printToxy(i,8,"_");
        printToxy(i,15,"_");
    }
    for(int i = 9; i < 16; i++){
        printToxy(29,i,"|");
        printToxy(46,i,"|");
        printToxy(90,i,"|");
    }
    printToxy(48,10,"[A] Withdraw          [D] Fund Transfer");
    printToxy(48,12,"[B] Deposit           [E] Change PIN");
    printToxy(48,14,"[C] Balance Inquiry   [X] Exit");
    createBlock(30, 16, 90, "");
    createBlock(30, 20, 90, " ");
    printToxy(57,22,"- - -");
}

void gotoxy(int x, int y){
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void printToxy(int x, int y, char* label){
    gotoxy(x,y);
    printf("%s",label);
}

void createBlock(int x, int y, int len, char *label){
    int i;
    for(i = x; i < len; i++){
        printToxy(i,y,"_");
        printToxy(i,y + 3,"_");
    }
    for(i = y + 1; i < y + 4; i++){
        printToxy(x - 1,i,"|");
        printToxy(len,i,"|");
    }
    printToxy(x + 1, y + 2,label);
}

void clear(char *label){
    if(strcmp(label,"result") == 0){
        printToxy(25,17,"                                                       ");
        printToxy(53,17,"- - -");
    }
    else if(strcmp(label,"result2") == 0){
        printToxy(32,22,"                                                       ");
        printToxy(57,22,"- - -");
    }
    else if(strcmp(label,"enter") == 0)
        printToxy(31,18,"                                                       ");
    else if(strcmp(label,"pinBoxes") == 0)
        printToxy(52, 13,"                 ");
}

void fdRemoved(){
    int i = 0;
    system("color 04");
    for(int i = 11; i < 19; i++)
        printToxy(44,i,"                                ");
    for(int i = 44; i < 76; i++){
        printToxy(i,10,"_");
        printToxy(i, 18,"_");
    }
    for(int i = 11; i < 19; i++){
        printToxy(43,i,"|");
        printToxy(76,i,"|");
    }
    do{
        for(int j = 11; j < 18; j++)
            printToxy(48,j,"                          ");
        printToxy(49,11,"         __     __");
        printToxy(49,12,"_    ____\\ \\___/ /");
        printToxy(49,13," |_ /     \\ \\_/ /");
        printToxy(49,14," | |       \\   /|");
        printToxy(49,15," |_|       / _ \\|");
        printToxy(49,16,"_| |_____ / /_\\ \\");
        printToxy(49,17,"         /_/   \\_\\ ");
        delay(580);
        for(int j = 11; j < 18; j++)
            printToxy(48,j,"                           ");
        printToxy(49,11,"         __     __");
        printToxy(49,12,"_        \\ \\___/ /___");
        printToxy(49,13," |   ___ /\\ \\_/ /    |");
        printToxy(49,14," |  |[] |  \\   /     |");
        printToxy(49,15," |  |[]_|  / _ \\     |");
        printToxy(49,16,"_|      |_/ /_\\ \\___ |");
        printToxy(49,17,"         /_/   \\_\\ ");
        delay(580);
        i++;
    }while(i < 4);
}
