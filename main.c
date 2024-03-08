#include "include/printers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void menu()
{
    printf("┌───────────────────────┐\n");
    printf("│Printer stuff          │\n");
    printf("├───────────────────────┤\n");
    printf("│d) Delete printer      │\n");
    printf("│a) Add printer         │\n");
    printf("│n) Send new task       │\n");
    printf("│s) Show pending tasks  │\n");
    printf("│p) Print task          │\n");
    printf("│l) Lowest load printer │\n");
    printf("│e) Exit                │\n");
    printf("├───────────────────────┤\n");
    printf("│Option:                │\n");
    printf("└───────────────────────┘\n");
    printf("\e[2F\e[9C");
}

void add_new_printer(TLISTA printers_data)
{
    PRINTER* printer;
    char option;
    printer = new_printer_structure();
    printf("Name: ");
    scanf(" %s", printer->name);
    printf("Brand: ");
    scanf(" %s", printer->brand);
    printf("Model: ");
    scanf(" %s", printer->model);
    printf("Ubication: ");
    scanf(" %s", printer->ubication);
    printf("Add printer %s %s %s %s ?[y/n]", printer->name, printer->brand,
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
            FREE(printer);
            break;
    }
}


int main(int argc, char** argv)
{
    // TODO
    TLISTA printers_data = NULL;
    if(argc <= 1)
    {
        printf("This program must be called as ./'program name' 'printers "
               "config file'. Exiting\n");
        return 1;
    }
    else if(load_initial_data(argv[1], &printers_data))
    {
        printf("Cannot load initial data. Exiting\n");
        return 1;
    }
    char option;
    char name[NAME_LEN];
    do
    {
        menu();
        scanf(" %c", &option);
        printf("\e[2B");
        switch(option)
        {
            case 'd':
            case 'D':
            {
                printf("Printer name: ");
                scanf(" %s", name);
                delete_printer(&printers_data, name);
                printf("Avaliable printers:\n");
                avaliable_printers(printers_data);
                break;
            }
            case 'a':
            case 'A':
            {
                add_new_printer(&printers_data);
                printf("Avaliable printers:\n");
                avaliable_printers(printers_data);
                break;
            }
            case 'n':
            case 'N':
            {
                break;
            }
            case 's':
            case 'S':
            {
                break;
            }
            case 'p':
            case 'P':
            {
                break;
            }
            case 'l':
            case 'L':
            {
                break;
            }
            case 'e':
            case 'E':
            {
                delete_list(printers_data);
                printf("Exiting...\n");
                break;
            }
            default:
            {
                printf("Option not found\n");
                break;
            }
        }
    } while(option != 'e');
    return 0;
}
