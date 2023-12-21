#pragma clang diagnostic push
#pragma clang diagnostic pop
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#pragma ide diagnostic ignored "misc-no-recursion"
#pragma ide diagnostic ignored "cert-err34-c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

//function declaration
void menu(void);
FILE* bookfileinit(void);
FILE* userfileinit(void);
FILE* tempfileinit(void);
void choice(FILE* bookfile, FILE* userfile, FILE* tempfile);
void invalidinput(FILE* bookfile, FILE* userfile, FILE* tempfile);
void addbook(FILE* bookfile);
void bookinfo(FILE* bookfile);
void outputbookinfo(FILE* bookfile, int choice);
void displaybooks(FILE* bookfile);
void searchbooks(FILE* bookfile);
void adduser(FILE* userfile);
void userinfo(FILE* userfile);
void outputuserinfo(FILE* userfile, int choice);
void displayusers(FILE* userfile);
void searchusers(FILE* userfile);
void borrowbook(FILE* bookfile, FILE* tempfile);
void returnbook(FILE* bookfile, FILE* tempfile);
void renewbook(FILE* bookfile);
int bookselect(FILE* bookfile);
int userselect(FILE* userfile);
void updateloanstatus(FILE* bookfile, FILE* tempfile, int choice, bool borrow);
void librarystats(FILE* bookfile, FILE* userfile);
void payment(FILE* userfile);
void block(void);

//book and user structure definitions
struct book{
    char loanstatus[50];
    char title[100];
    char author[100];
    char subject[50];
    char location[50];
    char loantype[50];
    int copies;
    unsigned long long ISBN;
};

struct user{
    char name[100];
    char usertype[100];
};

int main(void){
    FILE *bookfile, *userfile, *tempfile;

    bookfile=bookfileinit(); //opens the three txt files and initialises their pointers
    userfile=userfileinit();
    tempfile=tempfileinit();
    menu();     //display menu
    choice(bookfile, userfile, tempfile);   //take user input for int

    fclose(bookfile);   //closes all txt files
    fclose(userfile);
    fclose(tempfile);

    remove("temp.txt");     //deletes temp file
}

//prints menu for users
void menu(void){
    printf("***University Library Information Service***");
    printf("\n\nMenu:\n");
    printf("\n1) Add Book");
    printf("\n2) Book Information");
    printf("\n3) Add User");
    printf("\n4) User Information");
    printf("\n5) Borrow Book");
    printf("\n6) Return Book");
    printf("\n7) Renew Book");
    printf("\n8) Library Statistics");
    printf("\n9) Make Payment");
    printf("\n10) Block Account");
    printf("\n0) Exit");
}

//opens books.txt and returns the pointer
FILE* bookfileinit(void){
    FILE *bookfile;
    bookfile = fopen("books.txt", "a+");

    return bookfile;
}

//opens users.txt and returns the pointer
FILE* userfileinit(void){
    FILE *userfile;
    userfile = fopen("users.txt", "a+");

    return userfile;
}

//opens temp.txt and returns the pointer
FILE* tempfileinit(void){
    FILE *tempfile;
    tempfile = fopen("temp.txt", "w");

    return tempfile;
}

//takes users input from the menu and then selects the matching function
void choice(FILE* bookfile, FILE* userfile, FILE* tempfile){
    int choice;

    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);

    switch(choice){
        case 1:
            addbook(bookfile);
            break;
        case 2:
            bookinfo(bookfile);
            break;
        case 3:
            adduser(userfile);
            break;
        case 4:
            userinfo(userfile);
            break;
        case 5:
            borrowbook(bookfile, tempfile);
            break;
        case 6:
            returnbook(bookfile, tempfile);
            break;
        case 7:
            renewbook(bookfile);
            break;
        case 8:
            librarystats(bookfile, userfile);
            break;
        case 9:
            payment(userfile);
            break;
        case 10:
            block();
            break;
        case 0:
            printf("\nGoodbye");
            exit(0);
        default:
            printf("\nInvalid Input");
            invalidinput(bookfile, userfile, tempfile);
    }
}

//return back to menu choice after incorrect input
void invalidinput(FILE* bookfile, FILE* userfile, FILE* tempfile){
    choice(bookfile, userfile, tempfile);
}

//add book to the file
void addbook(FILE* bookfile){
    struct book newbook;

    printf("\nAdd Book");

    //takes users input for each piece of data for the book and stores it
    printf("\n\nEnter title: ");
    scanf(" %[^\n]", newbook.title);

    printf("Enter author: ");
    scanf(" %[^\n]", newbook.author);

    printf("Enter subject: ");
    scanf(" %[^\n]", newbook.subject);

    printf("Enter location: ");
    scanf(" %[^\n]", newbook.location);

    printf("Enter loan type: ");
    scanf(" %[^\n]", newbook.loantype);

    printf("Enter number of copies: ");
    scanf("%d", &newbook.copies);

    printf("Enter ISBN number: ");
    scanf("%llu", &newbook.ISBN);

    //writes data stored in variables to the text file formatted with a semicolon between each bit
    //also adds 'onshelf', so the book is in the library by default
    fprintf(bookfile, "%s;%s;onshelf;%s;%s;%s;%d;%llu;\n", newbook.title, newbook.author, newbook.subject, newbook.location, newbook.loantype, newbook.copies, newbook.ISBN);

    printf("\n\n");
    system("pause");
    exit(0);
}

//asks user if they want to display all books or search for book and then select the book to get the info about
void bookinfo(FILE* bookfile){
    int menuchoice, choice;

    printf("\nUser information\n");

    printf("\nSelect from list (1) or Search for book (2): ");
    scanf("%d", &menuchoice);

    switch(menuchoice){
        case 1:
            printf("\n");
            displaybooks(bookfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            outputbookinfo(bookfile, choice);   //calls function to display the book data
            break;
        case 2:
            searchbooks(bookfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            outputbookinfo(bookfile, choice);
            break;
        default:
            printf("Invalid Input");
            break;
    }
}

//takes the selected book from 'bookinfo' and displays all the information on the book
void outputbookinfo(FILE* bookfile, int choice){
    int count = 0;
    char loanstatus[50];

    struct book selectedbook;

    rewind(bookfile);

    while (fscanf(bookfile, " %[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d;%llu;", selectedbook.title, selectedbook.author, selectedbook.loanstatus, selectedbook.subject, selectedbook.location, selectedbook.loantype, &selectedbook.copies, &selectedbook.ISBN) != EOF) {
        count++;

        if (strcmp(loanstatus, "onloan") == 0){
            strcpy(loanstatus, "Out on loan");
        } else{
            strcpy(loanstatus, "In the library");
        }

        if (count == choice){
            printf("\nTitle: %s\nAuthor: %s\nLoan Status: %s\nLocation: %s\nLoan Type: %s\nNumber of copies: %d\nISBN number: %llu", selectedbook.title, selectedbook.author, loanstatus, selectedbook.location, selectedbook.loantype, selectedbook.copies, selectedbook.ISBN);
        }
    }

    printf("\n\n");
    system("pause");
    exit(0);
}

//displays list of all the book titles and authors
void displaybooks(FILE* bookfile){
    struct book displaybook;
    int bookcount = 1;

    while (fscanf(bookfile, "%[^;];%[^;];", displaybook.title, displaybook.author) != EOF) {
        printf("%d) %s by %s\n", bookcount, displaybook.title, displaybook.author);

        while (getc(bookfile) != '\n') {
        }

        bookcount++;
    }
}

//User enters letter and then displays all books that begin with that letter
void searchbooks(FILE* bookfile){
    int bookcount = 1;
    char c;

    struct book searchedbook;

    printf("\n\nEnter the first character of the book: ");
    scanf(" %c", &c);
    printf("\n");

    rewind(bookfile);

    while (fscanf(bookfile, " %[^;];%[^;];", searchedbook.title, searchedbook.author) != EOF) {
        if (searchedbook.title[0] == c) {
            printf("%d) %s by %s\n", bookcount, searchedbook.title, searchedbook.author);
        }

        while (getc(bookfile) != '\n') {
        }

        bookcount++;
    }
}

//add a new user and store it to the file
void adduser(FILE* userfile){
    struct user newuser;

    printf("\nAdd User");

    printf("\n\nEnter name: ");
    scanf(" %[^\n]", newuser.name);

    printf("\nEnter user type (Staff or Student): ");
    scanf(" %[^\n]", newuser.usertype);

    fprintf(userfile, "%s;%s;\n", newuser.name, newuser.usertype);

    printf("\n\n");
    system("pause");
    exit(0);
}

//asks user if they want to display all users or search for the user and then select the user to get the info about
void userinfo(FILE* userfile){
    int choice;

    printf("\nUser information\n\n");

    choice=userselect(userfile);

    outputuserinfo(userfile, choice);
}

//takes the selected user from 'userinfo' and displays all the information on the user
void outputuserinfo(FILE* userfile, int choice){
    int count = 0;

    struct user selecteduser;

    rewind(userfile);

    while (fscanf(userfile, " %[^;];%[^;];", selecteduser.name, selecteduser.usertype) != EOF) {
        count++;

        if (count == choice){
            printf("\nName: %s\nUser Type: %s", selecteduser.name, selecteduser.usertype);
        }
    }

    printf("\n\n");
    system("pause");
    exit(0);
}

//displays list of all the users names and types
void displayusers(FILE* userfile) {
    struct user displayuser;
    int usercount = 1;

    while (fscanf(userfile, "%[^;];%[^;];", displayuser.name, displayuser.usertype) != EOF) {
        printf("%d. %s, %s\n", usercount, displayuser.name, displayuser.usertype);

        while (getc(userfile) != '\n') {
        }

        usercount++;
    }
}

//User enters letter and then displays all users names that begin with that letter
void searchusers(FILE* userfile) {
    struct user searcheduser;
    int usercount = 1;
    char c;

    printf("\nSearch Users");

    printf("\n\nEnter the first character of the users forename: ");
    scanf(" %c", &c);
    printf("\n");

    while (fscanf(userfile, " %[^;];%[^;];", searcheduser.name, searcheduser.usertype) != EOF) {
        if (searcheduser.name[0] == c) {
            printf("%d) %s, %s\n", usercount, searcheduser.name, searcheduser.usertype);
        }

        while (getc(userfile) != '\n') {
        }

        usercount++;
    }
}

//select a book and then change its loan status to 'onloan'
void borrowbook(FILE* bookfile, FILE* tempfile){
    int choice;
    bool borrow = true;

    printf("\nBorrow Book\n\n");

    choice = bookselect(bookfile);

    updateloanstatus(bookfile, tempfile, choice, borrow);
}

//
void returnbook(FILE* bookfile, FILE* tempfile){
    int choice;
    bool borrow = false;

    printf("\nReturn Book\n\n");

    choice = bookselect(bookfile);

    updateloanstatus(bookfile, tempfile, choice, borrow);
}

void renewbook(FILE* bookfile) {
    int menuchoice, choice;

    printf("\nRenew Book");

    printf("\n\nSelect from list (1) or Search for book (2): ");
    scanf("%d", &menuchoice);

    switch (menuchoice) {
        case 1:
            printf("\n");
            displaybooks(bookfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            printf("Test1");
            printf("\n\n");
            system("pause");
            exit(0);
        case 2:
            searchbooks(bookfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            printf("\nTest2");
            printf("\n\n");
            system("pause");
            exit(0);
        default:
            printf("Invalid Input");
            printf("\n\n");
            system("pause");
            exit(0);
    }
}

int bookselect(FILE* bookfile){
    int choice = 0, menuchoice;

    printf("Select from list (1) or Search for book (2): ");
    scanf("%d", &menuchoice);

    switch (menuchoice) {
        case 1:
            printf("\n");
            displaybooks(bookfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            break;
        case 2:
            searchbooks(bookfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            break;
        default:
            printf("Invalid Input");
            printf("\n\n");
            system("pause");
            exit(0);
    }

    return choice;
}

int userselect(FILE* userfile){
    int choice = 0, menuchoice;

    printf("Select user from list (1) or Search for user (2): ");
    scanf("%d", &menuchoice);

    switch (menuchoice) {
        case 1:
            printf("\n");
            displayusers(userfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            break;
        case 2:
            searchusers(userfile);
            printf("\nEnter choice: ");
            scanf("%d", &choice);
            break;
        default:
            printf("Invalid Input");
            printf("\n\n");
            system("pause");
            exit(0);
    }
    return choice;
}

void updateloanstatus(FILE* bookfile, FILE* tempfile, int choice, bool borrow) {
    struct book selectedbook;
    int count = 0;
    char loanstatus[100];

    rewind(bookfile);

    if (borrow == 1){
        while (fscanf(bookfile, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d;%llu;", selectedbook.title, selectedbook.author, loanstatus, selectedbook.subject, selectedbook.location, selectedbook.loantype, &selectedbook.copies, &selectedbook.ISBN) != EOF) {
            count++;

            if (count == choice) {
                if (strcmp(loanstatus, "onshelf") == 0) {
                    fprintf(tempfile, "%s;%s;onloan;%s;%s;%s;%d;%llu;\n", selectedbook.title, selectedbook.author, selectedbook.subject, selectedbook.location, selectedbook.loantype, selectedbook.copies, selectedbook.ISBN);
                    printf("\nBook successfully borrowed\n");
                } else {
                    fprintf(tempfile, "%s;%s;onloan;%s;%s;%s;%d;%llu;\n", selectedbook.title, selectedbook.author, selectedbook.subject, selectedbook.location, selectedbook.loantype, selectedbook.copies, selectedbook.ISBN);
                    printf("\nBook is already out on loan\n");
                }
            } else {
                fprintf(tempfile, "%s;%s;%s;%s;%s;%s;%d;%llu;\n", selectedbook.title, selectedbook.author, loanstatus, selectedbook.subject, selectedbook.location, selectedbook.loantype, selectedbook.copies, selectedbook.ISBN);
            }

            while (getc(bookfile) != '\n') {
            }
        }
    } else{
        while (fscanf(bookfile, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%d;%llu;", selectedbook.title, selectedbook.author, loanstatus, selectedbook.subject, selectedbook.location, selectedbook.loantype, &selectedbook.copies, &selectedbook.ISBN) != EOF) {
            count++;

            if (count == choice) {
                if (strcmp(loanstatus, "onloan") == 0) {
                    fprintf(tempfile, "%s;%s;onshelf;%s;%s;%s;%d;%llu;\n", selectedbook.title, selectedbook.author, selectedbook.subject, selectedbook.location, selectedbook.loantype, selectedbook.copies, selectedbook.ISBN);
                    printf("\nBook successfully returned\n");
                } else {
                    fprintf(tempfile, "%s;%s;onshelf;%s;%s;%s;%d;%llu;\n", selectedbook.title, selectedbook.author, selectedbook.subject, selectedbook.location, selectedbook.loantype, selectedbook.copies, selectedbook.ISBN);
                    printf("\nBook is already on the shelf\n");
                }
            } else {
                fprintf(tempfile, "%s;%s;%s;%s;%s;%s;%d;%llu;\n", selectedbook.title, selectedbook.author, loanstatus, selectedbook.subject, selectedbook.location, selectedbook.loantype, selectedbook.copies, selectedbook.ISBN);
            }

            while (getc(bookfile) != '\n') {
            }
        }
    }

    fclose(bookfile);
    fclose(tempfile);

    remove("books.txt");
    rename("temp.txt", "books.txt");

    printf("\n\n");
    system("pause");
    exit(0);
}

void librarystats(FILE* bookfile, FILE* userfile){
    int bookcount=0, usercount=0;

    printf("\nLibrary Statistics\n");

    while (!feof(bookfile)) {
        char c1 = fgetc(bookfile);

        if (c1 == '\n') {
            bookcount++;
        }
    }

    while (!feof(userfile)) {
        char c2 = fgetc(userfile);

        if (c2 == '\n') {
            usercount++;
        }
    }

    printf("\nNumber of books: %d", bookcount);
    printf("\n\nNumber of users: %d", usercount);
    printf("\n\n");
    system("pause");
    exit(0);

}

void payment(FILE* userfile){
    int choice, days, cvc, count = 0;
    unsigned long long cardno;
    float fine;
    char expirydate[10];
    struct user selecteduser;

    printf("\nMake Payment\n\n");

    userselect(userfile);

    printf("\nSelect type of fine: ");
    printf("\n\n1) Late\n2) Damaged\n3) Lost");
    printf("\n\nEnter choice: ");
    scanf("%d", &choice);

    switch(choice){
        case 1:
            printf("\n20p per day charge\n");
            printf("\nEnter number of days late: ");
            scanf("%d", &days);
            fine=days*0.2;
            printf("\nCharge = %.2f GBP", fine);
            break;
        case 2:
            fine=5;
            printf("\nCharge = %.2f GBP", fine);
            break;
        case 3:
            fine=10;
            printf("\nCharge = %.2f GBP", fine);
            break;
        default:
            printf("Invalid input");
            break;
    }

    printf("\n\nEnter card number: ");
    scanf("%llu", &cardno);
    printf("\nEnter expiry date: ");
    scanf("%s", expirydate);
    printf("\nEnter cvc: ");
    scanf("%d", &cvc);

    rewind(userfile);

    while (fscanf(userfile, " %[^;];", selecteduser.name) != EOF) {
        count++;

        if (count == choice){
            printf("\nName: %s", selecteduser.name);
        }
    }

    printf("\nCard number: %llu", cardno);
    printf("\nExpiry date: %s", expirydate);
    printf("\nCVC: %d", cvc);

    Sleep(1500);

    printf("\n\nTransaction of %.2f GBP complete", fine);

    printf("\n\n");
    system("pause");
    exit(0);
}

void block(){
    printf("\nBlock Account");

}