/* Author: nsunga
 * 08-11-15
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct xrec{
    char firstName[100];
    char lastName[100];
    char ssn[12];
    char gender;
    double age, hours, rate;
    double salary, overtime, regHrs;
    struct xrec *next;
};
typedef struct xrec EMPLOYEE;

EMPLOYEE * addEmployee(EMPLOYEE * list, EMPLOYEE t);
EMPLOYEE * deleteEmployee(EMPLOYEE * list, char * tssn);
int displayMenu(void);
void clearBuffer(void);
void listEmployee(EMPLOYEE * list);
void listFemales(EMPLOYEE * list);
void listOvertime(EMPLOYEE * list);
void sortEmployee(EMPLOYEE * list);
void highestSalary(EMPLOYEE * list);
void overwrite(EMPLOYEE * temp);
void save(EMPLOYEE * list);
/* list is head node */

int main(void){
    EMPLOYEE temp;
    EMPLOYEE * head = NULL;
    char tssn[12];
    int num, size = 0;
    FILE * infile;

    infile = fopen("employeedata.txt", "r");
    if (infile == NULL){
        printf("File not found.\n");
        exit(0);
    }
    
    while (fscanf(infile, "%10s %10s %11s %c %lf %lf %lf", temp.firstName,
                  temp.lastName, temp.ssn, &temp.gender, &temp.age,
                  &temp.hours, &temp.rate) != EOF){
        temp.firstName[99] = '\0';
        temp.lastName[99] = '\0';
        temp.ssn[11] = '\0';
        head = addEmployee(head, temp);
    }
    fclose(infile);
    
    do{
        num = displayMenu();
        switch (num){
            case 1: listEmployee(head); break;
            case 2: overwrite(&temp); head = addEmployee(head, temp); break;
            case 3: head = deleteEmployee(head, tssn); break;
            case 4: listFemales(head); break;
            case 5: listOvertime(head); break;
            case 6: sortEmployee(head); break;
            case 7: highestSalary(head); break;
            case 8: save(head); break;
            default: printf("This shouldn't run.."); break;
        }
    } while (num != 8);
    return 0;
}

/***************************** FUNCTIONS ****************************/

EMPLOYEE * addEmployee(EMPLOYEE * list, EMPLOYEE t){
    EMPLOYEE * newNode;
    
    newNode = (EMPLOYEE *) malloc(sizeof(EMPLOYEE));
    (*newNode) = t;
    newNode->next = list;

    return newNode;
}

/******************************/

EMPLOYEE * deleteEmployee(EMPLOYEE * list, char * tssn){
    EMPLOYEE * curr, * prev;
    
    printf("Enter SSN:\n");
    gets(tssn);
    
    for (prev = NULL,curr=list;
         (curr != NULL && !(strcmp(curr->ssn,tssn) == 0))
         ;prev=curr,curr=curr->next)
        ;/* searches for match */
    
    if (prev == NULL){
        list = curr->next;
        free(curr);
        printf("The First recored was deleted\n");
        return list;
    }
    
    if (curr == NULL){ /* only NULL if it runs through the whole list */
        printf("There was no Match\n");
        return list;
    }
    else{
        prev->next = curr->next;
        free(curr);
        printf("One recored was deleted\n");
        return list;
    }
}

/******************************/

void listEmployee(EMPLOYEE * list){
    int i = 0;

    while (list != NULL){
        if (list->hours < 41)
            list->salary = list->hours * list->rate;
        else{
            list->overtime = list->hours - 40;
            list->regHrs = list->hours - list->overtime;
            list->salary = (list->overtime * list->rate * 1.5)
            + (list->regHrs * list->rate);
        }
        printf("%d. %-10s %-10s %11s %c %.2f %.2f %.2f $%.2f\n", i + 1,
               list->firstName, list->lastName, list->ssn, list->gender,
               list->age, list->hours, list->rate, list->salary);
        list = list->next;
        i++;
    }
}

/******************************/

void listFemales(EMPLOYEE * list){
    int i = 0;
    while (list != NULL){
        if (list->gender == 'F'){
            printf("%d. %-10s %-10s %11s %c %.2f %.2f %.2f $%.2f\n", i + 1,
                   list->firstName, list->lastName, list->ssn, list->gender,
                   list->age, list->hours, list->rate, list->salary);
            i++;
        }
        list = list->next;
    }
    if (i == 0)
        printf("There are no female employees.\n");
}

/******************************/

void listOvertime(EMPLOYEE * list){ /* outputs workers that did overtime */
    int i = 0;
    while (list != NULL){
        if (list->hours > 40){
            printf("%d. %-10s %-10s %11s %c %.2f %.2f %.2f $%.2f\n", i + 1,
                   list->firstName, list->lastName, list->ssn, list->gender,
                   list->age, list->hours, list->rate, list->salary);
            i++;
        }
        list = list->next;
    }
    if (i == 0)
        printf("No employees worked overtime.\n");
}

/******************************/

void sortEmployee(EMPLOYEE * list){ /* Function switches elements if needed, not the entire nodes */
    EMPLOYEE * head = list;
    EMPLOYEE * previous = head;
    char tfirstName[100];
    char tlastName[100];
    char tssn[12];
    char tgender;
    double tage, thours, trate;
    
    list = list->next; /* changing value of list does not affect above ptrs */

    while(list != NULL){
        
        previous = head; /* keeps the link list intact */
        
        while(previous != list){
            if (strcmp(previous->firstName, list->firstName) > 0){

                strcpy(tfirstName, list->firstName);
                strcpy(list->firstName, previous->firstName);
                strcpy(previous->firstName, tfirstName);
                
                strcpy(tlastName, list->lastName);
                strcpy(list->lastName, previous->lastName);
                strcpy(previous->lastName, tlastName);
                
                strcpy(tssn, list->ssn);
                strcpy(list->ssn, previous->ssn);
                strcpy(previous->ssn, tssn);
                
                tgender = list->gender;
                list->gender = previous->gender;
                previous->gender = tgender;
                
                tage = list->age;
                list->age = previous->age;
                previous->age = tage;
                
                thours = list->hours;
                list->hours = previous->hours;
                previous->hours = thours;
                
                trate = list->rate;
                list->rate = previous->rate;
                previous->rate = trate;
            }
            else
                previous = previous->next;
        }
        list = list->next;
    }
    printf("Sorting is....done!\n");
}

/******************************/

void highestSalary(EMPLOYEE * list){ /* output employee w/ highest salary */
    EMPLOYEE temp, temp2;
    temp = *list;
    double max;
    int i = 0;
    
    while (list != NULL){
        if (list->hours < 41)
            list->salary = list->hours * list->rate;
        else{
            list->overtime = list->hours - 40;
            list->regHrs = list->hours - list->overtime;
            list->salary = (list->overtime * list->rate * 1.5)
            + (list->regHrs * list->rate);
        }
        list = list->next;
    }
    
    temp2 = temp;
    list = &temp; /* return to starting node */
    max = list->salary;
    
    while (list != NULL){
        if (max <= list->salary){
            max = list->salary;
            temp = *list;
            i++;
        }
        list = list->next;
    }
    
    list = &temp2; /* return to starting node */
    
    if (i == 0){
        printf("Employee with the highest salary is...\n");
        printf("%-10s %-10s %11s %c %.2f %.2f %.2f $%.2f\n", temp.firstName,
               temp.lastName, temp.ssn, temp.gender, temp.age, temp.hours,
               temp.rate, temp.salary);
    }
    else{
        printf("Employees with the highest salary are...\n");
        while (list != NULL){
            if (list->salary == temp.salary){
                printf("%-10s %-10s %11s %c %.2f %.2f %.2f $%.2f\n",
                       list->firstName, list->lastName, list->ssn,
                       list->gender, list->age, list->hours, list->rate,
                       list->salary);
            }
            list = list->next;
        }
    }
}

/******************************/

void overwrite(EMPLOYEE * temp){ /* overwrites a temporary node to add to link list */
    printf("Enter first name: ");
    gets(temp->firstName);
    printf("Enter last name: ");
    gets(temp->lastName);
    printf("Enter SSN (XXX-XX-XXXX): ");
    gets(temp->ssn);
    printf("Enter gender M or F: ");
    scanf("%c", &temp->gender);
    printf("Enter age: ");
    scanf("%lf", &temp->age);
    printf("Enter the number of hours worked: ");
    scanf("%lf", &temp->hours);
    printf("Enter rate or pay by the hour: ");
    scanf("%lf", &temp->rate);
}

/******************************/

void save(EMPLOYEE * list){ /* saves current employees to the existing .txt file */
    FILE * infile;
    infile = fopen("employeedata.txt", "w");
    
    while (list != NULL){
        fprintf(infile, "%10s %10s %11s %c %lf %lf %lf", list->firstName,
                list->lastName, list->ssn, list->gender, list->age, list->hours,
                list->rate);
        list = list->next;
    }
    fclose(infile);
    printf("File is...updated!\n");
}

/******************************/

int displayMenu(void){
    int input;
    
    do{
        printf("\n*************************************************\n");
        printf("**** 1. List all employees w/ their salary.  ****\n");
        printf("**** 2. Add an employee to the payroll.      ****\n");
        printf("**** 3. Delete an employee from the payroll. ****\n");
        printf("**** 4. List all the female employees.       ****\n");
        printf("**** 5. List all employees with overtime.    ****\n");
        printf("**** 6. Alphabetize the list of employees.   ****\n");
        printf("**** 7. Who gets the highest salary?         ****\n");
        printf("**** 8. Quit and save.                       ****\n");
        printf("*************************************************\n\n");
        
        printf("Enter your choice: ");
        scanf("%d", &input);
        clearBuffer();
    } while (input < 1 || input > 8); /* ensures user follows directions */
    
    return input;
}

/******************************/

void clearBuffer(void){
    while (getchar() != '\n')
        ;
}
