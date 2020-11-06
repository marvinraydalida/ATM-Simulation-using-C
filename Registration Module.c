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
char regName[50];
char regPin[7];
char confirmPin[7];
char* checkDrives[2] = {"D:\\checkFd.dbf", "G:\\checkFd.dbf"};
char* drives[2] = {"D:\\bank.dbf","G:\\bank.dbf"};
int regPinInt, regAccountNo, regBalance, ctr = -1, exist = 0, checkName = 0, checkPin = 0, pinCharacter = 0;

void makenull();
int insertCard();
void retrieve();
int checkFd();
void registration();
int isEmpty();
int isFull();
void insert();
int locate(int x);
void save();
void saveToCard();

//UI AND ANIMATION
void delay(int ms);
void insertCardAnimation();
void registerUi();
void gotoxy(int x, int y);
void printToxy(int x, int y, char* label);
COORD coord = {0,0};
void createBlock(int x, int y, int len, char *label);
void clear(char *label);
void dontRemove(int input);

int main(){
    int i;
    system("color 06");
    makenull();
    if(insertCard()){
        _getch();
        system("cls");
        retrieve();
        registerUi();
        registration();
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
        dontRemove(0);
        printToxy(42,9,"                             ");
        printToxy(43,9,"P R E S S   A N Y   K E Y");
        return 1;
    }
    else{
        printToxy(46,9,"R E G I S T E R E D");
        return 0;
    }
    fclose(fp);
}

int checkFd(){
    FILE *fp;
    fp = fopen(checkDrives[ctr],"r");
    if(fp == NULL)
        return 1;
    return 0;
    fclose(fp);
}

void retrieve(){
    FILE *fp;
    int i = 0;
    fp = fopen("accdb.dbf","r+");

    if (fp==NULL){
        registerUi();
        printToxy(42,22,"D A T A B A S E   I S   E M P T Y");
        _getch();
        system("cls");
    }
    else{
        while(!feof(fp)){
            l.last++;
            fscanf(fp,"%d %[^\t]%d %[^\n]",&accountInfo[l.last].acountNo,accountInfo[l.last].name,&accountInfo[l.last].balance,accountInfo[l.last].pin);
        }
    }
    fclose(fp);
}

void registration(){
    int end;
    do{
        end = 1;
        l.last++;
        checkName = 0;
        gotoxy(42, 6);
        scanf(" %[^\n]s",regName);
        strlwr(regName);

        do{
            srand(time(0));
            regAccountNo = 1000+(rand()%9000);
        }while(locate(1));

        gotoxy(36, 10);
        printf("%d",regAccountNo);

       do{
            printToxy(69,10,"                 ");
            gotoxy(69, 10);
            scanf("%d",&regBalance);

            if(regBalance < 500)
                printToxy(32,22,"I N I T I A L   D E P O S I T   M U S T   B E   4 9 9 +");
            else{
                clear("result");
                break;
            }
        }while(1);

        do{
            checkPin = 0;
            pinCharacter = 0;
            gotoxy(55, 14);
            for(int i = 0; i < 6; i++){
                regPin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            regPin[6] = '\0';

            gotoxy(55, 18);
            for(int i = 0; i < 6; i++){
                confirmPin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            confirmPin[6] = '\0';

            end = strcmp(confirmPin,regPin);

            for(int i = 0; i < 6; i++){
                if(isdigit(confirmPin[i] - 9)  == 0){
                    pinCharacter = 1;
                    break;
                }
            }

            if(end != 0){
                printToxy(42,22,"P I N   D I D   N O T   M A T C H !");
                _getch();
                clear("result");
                clear("pinBoxes");
            }

            else if(locate(0) && checkPin == 1){
                printToxy(46,22,"P I N   N O T   U N I Q U E");
                _getch();
                clear("result");
                clear("pinBoxes");
            }
            else if(pinCharacter == 1){
                printToxy(36,22,"P I N   C O N T A I N S   A   C H A R A C T E R");
                _getch();
                clear("result");
                clear("pinBoxes");
            }

        }while(end != 0 || checkPin || pinCharacter);

        if(locate(0) && checkName == 1){
            l.last--;
            printToxy(39,22,"A C C O U N T   E X I S T   A L R E A D Y");
            _getch();
            system("cls");
            registerUi();
        }

    }while(checkName);

    if(isFull())
        printToxy(43,22,"D A T A B A S E   I S   F U L L");
    else
        insert();
}

int isEmpty(){
    return(l.last == 0);
}

int isFull(){
    return(l.last == MAX);
}

void insert(){
    if(checkFd()){
        printToxy(43,22,"F L A S H   D R I V E   E R R O R");
        dontRemove(1);
        gotoxy(0,25);
        exit(0);
    }
    strcpy(accountInfo[l.last].name,regName);
    accountInfo[l.last].acountNo = regAccountNo;
    accountInfo[l.last].balance = regBalance;
    strcpy(accountInfo[l.last].pin,regPin);
    printToxy(47,22,"R E G I S T E R E D !!!");
    save();
    saveToCard();
}

int locate(int x){
    if(x == 0){
        for(int i = 0; i < l.last + 1; i++){
            if(strcmp(accountInfo[i].name,regName) == 0 || strcmp(accountInfo[i].pin,regPin) == 0){
                if(strcmp(accountInfo[i].name,regName) == 0)
                    checkName = 1;
                if(strcmp(accountInfo[i].pin,regPin) == 0)
                    checkPin = 1;
                return 1;
            }
        }
        return 0;
    }
    else if(x == 1){
        for(int i = 0; i < l.last + 1; i++){
            if(accountInfo[i].acountNo == regAccountNo)
                return 1;
        }
        return 0;
   }
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
    fprintf(fp,"%d\t%s",regAccountNo,regPin);
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

void registerUi(){
    printToxy(48,2,"R E G I S T R A T I O N");
    createBlock(30, 4, 90, "Full Name:");
    createBlock(30, 8, 45, "No.:");
    createBlock(47, 8, 90, "Initial Deposit: Php");
    createBlock(30, 12, 90, "Enter PIN:");
    createBlock(30, 16, 90, "Confirm PIN:");
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
        printToxy(32,22,"                                                       ");
        printToxy(57,22,"- - -");
    }
    else if(strcmp(label,"pinBoxes") == 0){
        printToxy(55,18,"                 ");
        printToxy(55, 14,"                 ");
    }
}

void dontRemove(int error){
    int i = 0, x, y, counter;
    if(error == 1){
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
        x = 49;
        y = 11;
        counter = 4;
    }
    else if(error == 0){
        x = 46;
        y = 12;
        counter = 2;
        printToxy(29,21,"[  D O   N O T   R E M O V E   W H E N   I N   U S E  ]");
        printToxy(27,23,"[  A C C O U N T   W I L L   B E   I N V A L I D A T E D  ]");
    }
    do{
        for(int j = y; j < y + 7; j++)
            printToxy(x - 1,j,"                          ");
        printToxy(x,y,"         __     __");
        printToxy(x,y + 1,"_    ____\\ \\___/ /");
        printToxy(x,y + 2," |_ /     \\ \\_/ /");
        printToxy(x,y + 3," | |       \\   /|");
        printToxy(x,y + 4," |_|       / _ \\|");
        printToxy(x,y + 5,"_| |_____ / /_\\ \\");
        printToxy(x,y + 6,"         /_/   \\_\\ ");
        delay(580);
        for(int j = y; j < y + 1; j++)
            printToxy(x - 1,j,"                           ");
        printToxy(x,y,"         __     __");
        printToxy(x,y + 1,"_        \\ \\___/ /___");
        printToxy(x,y + 2," |   ___ /\\ \\_/ /    |");
        printToxy(x,y + 3," |  |[] |  \\   /     |");
        printToxy(x,y + 4," |  |[]_|  / _ \\     |");
        printToxy(x,y + 5,"_|      |_/ /_\\ \\___ |");
        printToxy(x,y + 6,"         /_/   \\_\\ ");
        delay(580);
        i++;
    }while(i < counter);
}
