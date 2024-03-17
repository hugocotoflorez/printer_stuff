#include "include/printers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RALIGN "\e[40C"
#define ADJUST "\e[1F"
#define CLRSCREEN() printf("\e[H\e[J");
#define BG "\e[44m"
#define RBG "\e[0m"

#define MENU(char)                                \
    printf("\e[s\e[100F" BG);                     \
    printf(RALIGN "┌───────────────────────┐\n"); \
    printf(RALIGN "│Printer stuff          │\n"); \
    printf(RALIGN "├───────────────────────┤\n"); \
    printf(RALIGN "│h) Avaliable printers  │\n"); \
    printf(RALIGN "│d) Delete printer      │\n"); \
    printf(RALIGN "│a) Add printer         │\n"); \
    printf(RALIGN "│t) Send new task       │\n"); \
    printf(RALIGN "│s) Show pending tasks  │\n"); \
    printf(RALIGN "│p) Print task          │\n"); \
    printf(RALIGN "│l) Lowest load printer │\n"); \
    printf(RALIGN "│e) Exit                │\n"); \
    printf(RALIGN "├───────────────────────┤\n"); \
    printf(RALIGN "│Option:                │\n"); \
    printf(RALIGN "└───────────────────────┘\n"); \
    printf("\e[2F\e[9C" RALIGN);                  \
    scanf(" %c", &option);                        \
    printf(RBG "\e[u" ADJUST);


#define GET_PNAME(name)       \
    printf("Printer name: "); \
    scanf(" %s", name);

#define nscanf(n, string) fgets(string, n, stdin);


void add_new_printer(TLISTA printers_list)
{
    char name[NAME_LEN], brand[BRAND_LEN], model[MODEL_LEN], ubication[UBICATION_LEN];
    char option;
    printf("Printer name: ");
    scanf(" %s", name);
    printf("Brand: ");
    scanf(" %s", brand);
    printf("Model: ");
    scanf(" %s", model);
    printf("Ubication: ");
    scanf(" %s", ubication);
    if(strlen(name) > NAME_LEN || strlen(brand) > BRAND_LEN ||
    strlen(model) > MODEL_LEN || strlen(ubication) > UBICATION_LEN)
    {
        printf("[!] Invalid printer format!\n  Cannot overpass %d-%d-%d-%d length\n",
        NAME_LEN, BRAND_LEN, MODEL_LEN, UBICATION_LEN);
        return;
    }
    printf("Add printer %s [y/n] ", name);
    scanf(" %c", &option);

    switch(option)
    {
        case 'y':
        case 'Y':
            add_printer(printers_list, *allocate_printer(name, brand, model, ubication));
            break;
    }
}


void send_task(TLISTA* printers_list)
{
    char printer_name[NAME_LEN];
    int task;
    printf("Printer name: ");
    scanf(" %s", printer_name);
    printf("Task id: ");
    scanf(" %d", &task);
    new_task(printers_list, printer_name, task);
}


int main(int argc, char** argv)
{
    char option;
    char name[NAME_LEN];
    TLISTA printers_list = NULL;
    CLRSCREEN();
    if(argc <= 1)
    {
        printf("[!] This program must be called as ./'program name' 'printers "
               "config file'. Exiting\n");
        return 1;
    }
    else if(load_initial_data(argv[1], &printers_list))
    {
        printf("[!] Cannot load initial data. Exiting\n");
        return 1;
    }
    avaliable_printers(printers_list);
    do
    {
        MENU(option);
        printf("\e[2B");
        switch(option)
        {
            case 'h':
            case 'H':
                avaliable_printers(printers_list);
                break;
            case 'd':
            case 'D':
                GET_PNAME(name);
                delete_printer(&printers_list, name);
                avaliable_printers(printers_list);
                break;
            case 'a':
            case 'A':
                add_new_printer(&printers_list);
                avaliable_printers(printers_list);
                break;
            case 't':
            case 'T':
                send_task(&printers_list);
                break;
            case 's':
            case 'S':
                GET_PNAME(name);
                show_pending_tasks(&printers_list, name);
                break;
            case 'p':
            case 'P':
                GET_PNAME(name);
                print_task(&printers_list, name);
                break;
            case 'l':
            case 'L':
                show_lowest_load_printer(&printers_list);
                break;
            case 'e':
            case 'E':
                rewrite_printers_file(argv[1], &printers_list);
                delete_list(&printers_list);
                printf("Exiting...\n");
                CLRSCREEN();
                break;
            default:
                printf("Option not found\n");
                break;
        }
    } while(option != 'e');
    return 0;
}
