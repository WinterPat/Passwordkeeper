/*
===============================================================================
 Name        : Passwordkeeper.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <string.h>
// TODO: insert other include files here
#define MY_FILE "text.txt"
#define LOGIN "userInfo.txt"
#define MAX_SIZE 64
// TODO: insert other definitions and declarations here
void printAll();
void printSelected();
void newlogin();
void encryptDecrypt(char *str, char *key);
void addNew();
void removeSelected();
int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here
    int i = 0, size = 0;
    char c[MAX_SIZE];
    FILE* read;

    //checks if there is login info, if not then you can add new.
    read = fopen(LOGIN,"r+");

    if (NULL != read) {
        fseek (read, 0, SEEK_END);
        size = ftell(read);

        if (0 == size) {
        	newlogin();
            printf("file is empty\n");
        }
    }
    fclose(read);

    i = login();

        while(i == 0){

            printf("Select: add,find,stop,remove or new\n");
            printf("'add' to add new password to manager\n");
            printf("'find' to find password for selected site\n");
            printf("'new' to change user and password\n");
            printf("'remove' to remove selected site from manager\n");
            printf("'stop' to end the program\n");

            scanf("%s", c);

            if(strcmp(c,"add") == 0){
                addNew();
            }
            if(strcmp(c,"find") == 0){
                printSelected();
            }
            if(strcmp(c,"new") == 0){
                newlogin();
            }
            if(strcmp(c,"remove") == 0){
                removeSelected();
                    }
            if(strcmp(c,"stop") == 0){
                i++;
            }
        }
    // Force the counter to be placed into memory
    volatile static int x = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        x++ ;
    }
    return 0 ;
}
void printAll(){
    FILE* read;
    char str[MAX_SIZE];
    char key[MAX_SIZE];
    int i = 0;

    read = fopen(MY_FILE,"r+");

    while(fgets(str, sizeof(str), read) != NULL){
        for(i = 0; i < strlen(str); i++){
            str[i] = str[i];
        }
        str[i-1] = '\0';
        printf("%s\n", str);
    }
    fclose(read);
}
//function to remove selected site and password from manager
void removeSelected(){
    FILE* read;
    FILE* read1;
    char key[MAX_SIZE];
    char find[MAX_SIZE];
    char str[MAX_SIZE];
    char str1[MAX_SIZE];
    char str2[MAX_SIZE];
    char temp[] = "temp.txt";
    char *array[2];
    int i,j = 0;

    printf("Enter the name of site you want to remove: ");
    scanf("%s", find);

    printf("Enter the key for the password: ");
    scanf("%s", key);

    read = fopen(MY_FILE,"r+");
    if(read == NULL){
        printf("Unable to open file!");
    }
    read1 = fopen(temp, "w");
    if(read1 == NULL){
        printf("Unable to open file!");
    }

    while(fgets(str, sizeof(str), read) != NULL){
        encryptDecrypt(str, key);


        for(i = 0; i < strlen(str); i++){
            str[i] = str[i];
        }
        str[i-1] = '\0';

        strcpy(str2,str);

        char *p = strtok (str, ":");

        i = 0;
        while (p != NULL){
            array[i] = p;
            i++;
            p = strtok (NULL, ":");
        }

        if(strcmp(array[0], find) != 0){
        	printf("%s\n", str2);
        	encryptDecrypt(str2, key);
        	fprintf(read1, "%s\n", str2);
            j++;
        }

        memset(str, '\0', sizeof(str));
        memset(array[0], '\0', sizeof(array[0]));
        memset(array[1], '\0', sizeof(array[1]));

    }
    if(j == 0){
        printf("Password not found!\n");
    }
    fclose(read1);
    fclose(read);
    remove(MY_FILE);
    rename(temp, MY_FILE);
}
/**
 * function to print selected site's password
 */
void printSelected(){
    FILE* read;
    char *array[2];
    char key[MAX_SIZE];
    char find[MAX_SIZE];
    char str[MAX_SIZE];
    int i,j = 0;

    printf("Enter the name of site to encrypt the password: ");
    scanf("%s", find);

    printf("Enter the key for the password: ");
    scanf("%s", key);

    read = fopen(MY_FILE,"r+");

    if(read == NULL){
        printf("Unable to open file!");
    }
    while(fgets(str, sizeof(str), read) != NULL){
        encryptDecrypt(str, key);

        for(i = 0; i < strlen(str); i++){
            str[i] = str[i];
        }
        str[i-1] = '\0';

        char *p = strtok (str, ":");
        i = 0;
        while (p != NULL){
        	array[i] = p;
            i++;
            p = strtok (NULL, ":");
         }

        if(strcmp(array[0], find) == 0){
            printf("Password: %s\n", array[1]);

            j++;
        }
        memset(str, '\0', sizeof(str));
        memset(array[0], '\0', sizeof(array[0]));
        memset(array[1], '\0', sizeof(array[1]));
    }
    if(j == 0){
        printf("Password not found!\n");
    }
    fclose(read);
}
//function to add new site and password to manager
void addNew(){
    FILE* read;
    char site[MAX_SIZE];
    char pass[MAX_SIZE];
    char key[MAX_SIZE];
    char str[MAX_SIZE];

    read = fopen(MY_FILE,"a");

    printf("Enter name of the site: ");
    scanf("%s", site);
    printf("Enter the password: ");
    scanf("%s", pass);
    printf("Enter the encryption key: ");
    scanf("%s", key);

    sprintf(str, "%s:%s", site,pass);
    encryptDecrypt(str, key);
    fprintf(read, "%s\n", str);

    memset(str, '\0', sizeof(str));
    memset(pass, '\0', sizeof(pass));
    memset(site, '\0', sizeof(site));
    memset(key, '\0', sizeof(key));

    fclose(read);
}
/*
 * function to set new login info
 */
void newlogin(){
    FILE* read;
    char str[MAX_SIZE];
    char name[MAX_SIZE];
    char pass[MAX_SIZE];

    read = fopen(LOGIN,"w+");

    printf("Enter new user name: ");
    scanf("%s", name);
    printf("Enter new password: ");
    scanf("%s", pass);

    sprintf(str,"%s:%s", name, pass);
    encryptDecrypt(str,"abc");

    fprintf(read, "%s\n", str);
    memset(str, '\0', sizeof(str));
    memset(name, '\0', sizeof(name));
    memset(pass, '\0', sizeof(pass));
    fclose(read);
}
// login function
int login(){
    FILE* read;
    char userLogin[MAX_SIZE];
    char passLogin[MAX_SIZE];
    char str[MAX_SIZE];
    char *array[2];
    int i = 0;

    printf("User: ");
    scanf("%s", userLogin);
    printf("Password: ");
    scanf("%s", passLogin);

    read = fopen(LOGIN,"r+");

    if(read == NULL){
        printf("Unable to open file!");
    }
    while(fgets(str, sizeof(str), read) != NULL){
        encryptDecrypt(str,"abc");
        for(i = 0; i < strlen(str); i++){
            str[i] = str[i];
        }
        str[i-1] = '\0';

        char *p = strtok (str, ":");
        i = 0;
        while (p != NULL){
        	array[i] = p;
            i++;
            p = strtok (NULL, ":");
        }

        if(strcmp(array[0], userLogin) == 0 && strcmp(array[1], passLogin) == 0){
            printf("Logging in...\n");
            return 0;
        }else{
            printf("Invalid user or password!");
            return 1;
        }
        memset(str, '\0', sizeof(str));
        memset(array[0], '\0', sizeof(array[0]));
        memset(array[1], '\0', sizeof(array[1]));
    }
    fclose(read);
}
//Encryption
void encryptDecrypt(char *str, char *key){
    int len = strlen(str);
    int klen = strlen(key);
    int i = 0, j = 0;

    for(i = 0,j = 0; i < len; i++,j++){
        if(j >= klen){
            j = 0;
        }
        if(isupper(key[j])){
        	key[j] = tolower(key[j]);
        }
        str[i] = str[i] ^ key[j];
        if(str[i] >= 127){
            str[i] = str[i] - 127;
        }
        if(str[i] <= 97){
            str[i] = str[i] + 32;
        }
    }
}



