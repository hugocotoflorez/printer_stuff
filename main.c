#include "include/printers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CLRSCREEN() printf("\e[H\e[J");

#define MENU()                             \
    printf("┌───────────────────────┐\n"); \
    printf("│Printer stuff          │\n"); \
    printf("├───────────────────────┤\n"); \
    printf("│d) Delete printer      │\n"); \
    printf("│a) Add printer         │\n"); \
    printf("│t) Send new task       │\n"); \
    printf("│s) Show pending tasks  │\n"); \
    printf("│p) Print task          │\n"); \
    printf("│l) Lowest load printer │\n"); \
    printf("│e) Exit                │\n"); \
    printf("├───────────────────────┤\n"); \
    printf("│Option:                │\n"); \
    printf("└───────────────────────┘\n"); \
    printf("\e[2F\e[9C");


#define GET_PNAME(name)       \
    printf("Printer name: "); \
    scanf(" %s", name);


void add_new_printer(TLISTA printers_data)
{
    PRINTER* printer;
    char option;
    printer = new_printer_structure();
    printf("Printer name: ");
    scanf(" %s", printer->name);
    printf("Brand: ");
    scanf(" %s", printer->brand);
    printf("Model: ");
    scanf(" %s", printer->model);
    printf("Ubication: ");
    scanf(" %s", printer->ubication);
    printf("Add printer %s %s %s %s [y/n] ", printer->name, printer->brand,
    printer->model, printer->ubication);
    scanf(" %c", &option);
    switch(option)
    {
        case 'y':
        case 'Y':
            add_printer(printers_data, *printer);
            break;
        default:
            free_printer_structure(*printer);
            break;
    }
    free(printer);
}


void send_task(TLISTA* printers_data)
{
    char printer_name[NAME_LEN];
    int task;
    printf("Printer name: ");
    scanf(" %s", printer_name);
    printf("Task id: ");
    scanf(" %d", &task);
    new_task(printers_data, printer_name, task);
}


int main(int argc, char** argv)
{
    CLRSCREEN();
    TLISTA printers_data = NULL;
    if(argc <= 1)
    {
        printf("[!] This program must be called as ./'program name' 'printers "
               "config file'. Exiting\n");
        return 1;
    }
    else if(load_initial_data(argv[1], &printers_data))
    {
        printf("[!] Cannot load initial data. Exiting\n");
        return 1;
    }
    char option;
    char name[NAME_LEN];
    do
    {
        MENU();
        scanf(" %c", &option);
        printf("\e[2B");
        switch(option)
        {
            case 'd':
            case 'D':
                GET_PNAME(name);
                delete_printer(&printers_data, name);
                avaliable_printers(printers_data);
                break;
            case 'a':
            case 'A':
                add_new_printer(&printers_data);
                avaliable_printers(printers_data);
                break;
            case 't':
            case 'T':
                send_task(&printers_data);
                break;
            case 's':
            case 'S':
                GET_PNAME(name);
                show_pending_tasks(&printers_data, name);
                break;
            case 'p':
            case 'P':
                GET_PNAME(name);
                print_task(&printers_data, name);
                break;
            case 'l':
            case 'L':
                show_lowest_load_printer(&printers_data);
                break;
            case 'e':
            case 'E':
                delete_list(&printers_data);
                printf("Exiting...\n");
                break;
            default:
                printf("Option not found\n");
                break;
        }
    } while(option != 'e');
    return 0;
}
