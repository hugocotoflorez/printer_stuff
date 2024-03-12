#include "include/printers.h"
#include "include/cola.h"
#include "include/lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void free_printer_structure(PRINTER printer_s)
{
    free(printer_s.name);
    free(printer_s.brand);
    free(printer_s.model);
    free(printer_s.ubication);
    free(printer_s.tasks_count);
    destruirCola(&printer_s.tasks);
}


void* new_printer_structure()
{
    PRINTER* main;
    main              = malloc(sizeof(PRINTER));
    main->name        = NULL;
    main->brand       = NULL;
    main->model       = NULL;
    main->ubication   = NULL;
    main->tasks_count = malloc(sizeof(int));
    crearCola(&(main->tasks));
    if((main == NULL) || (main->tasks_count == NULL))
    {
        free_printer_structure(*main);
        printf("[!] Cannot allocate memory\n");
        return NULL;
    }
    *main->tasks_count = 0;
    return main;
}


PRINTER* alloc_printer(PRINTER printer)
{
    PRINTER* new_printer = new_printer_structure();
    if(!load_new_printer(new_printer, printer))
        return new_printer;
    printf("[!] Cannot allocate memory\n");
    return NULL;
}


// 0: no error, 1: error. move data from struct to pointer to struct
int load_printer_data(PRINTER* dest_printer, PRINTER src_printer)
{
    dest_printer->name        = malloc(sizeof(char)*strlen(src_printer.name);
    dest_printer->brand       = malloc(sizeof(char)*strlen(src_printer.brand);
    dest_printer->model       = malloc(sizeof(char)*strlen(src_printer.model);
    dest_printer->ubication   = malloc(sizeof(char)*strlen(src_printer.ubication);
    strcpy(dest_name, src_name);
    strcpy(dest_brand, src_brand);
    strcpy(dest_model, src_model);
    strcpy(dest_ubication, src_ubication);
    if((main->name == NULL) || (main->brand == NULL) ||
    (main->model == NULL) || (main->ubication == NULL))
    {
        free_printer_structure(*main);
        printf("[!] Cannot allocate memory\n");
        return NULL;
    }
}


void delete_list(TLISTA* list)
{
    TPOSICION current;
    PRINTER current_element;
    TPOSICION ultimo = finLista(*list);
    if(!esListaVacia(*list))
        for(current = primeroLista(*list); current != ultimo;
            current = siguienteLista(*list, current))
        {
            recuperarElementoLista(*list, current, &current_element);
            printf("[>] Removing connection with printer_s: %s %s %s %s\n",
            current_element.name, current_element.brand, current_element.model,
            current_element.ubication);
            free_printer_structure(current_element);
        }
    destruirLista(list);
}


int delete_printer(TLISTA* printers_list, char* printer_name)
{
    TPOSICION current;
    PRINTER current_element;
    TPOSICION ultimo = finLista(*printers_list);
    printf("[+] To Delete: %s\n", printer_name);
    if(esListaVacia(*printers_list))
    {
        printf("[!] Printer %s is not connected\n", printer_name);
        return 1;
    }
    for(current = primeroLista(*printers_list); current != ultimo;
        current = siguienteLista(*printers_list, current))
    {
        recuperarElementoLista(*printers_list, current, &current_element);
        if(!strcmp(current_element.name, printer_name))
        {
            printf("[>] Removing connection with printer_s: %s %s %s %s\n",
            current_element.name, current_element.brand, current_element.model,
            current_element.ubication);
            free_printer_structure(current_element);
            suprimirElementoLista(printers_list, current);
            return 0;
        }
    }
    printf("[!] Printer %s is not connected\n", printer_name);
    return 1;
}


void avaliable_printers(TLISTA printers_list)
{
    TPOSICION current;
    PRINTER current_element;
    TPOSICION ultimo = finLista(printers_list);
    printf("[Avaliable printers]\n");
    for(current = primeroLista(printers_list); current != ultimo;
        current = siguienteLista(printers_list, current))
    {
        recuperarElementoLista(printers_list, current, &current_element);
        printf("| Printer: %s %s %s %s\n", current_element.name,
        current_element.brand, current_element.model, current_element.ubication);
    }
}


int parse_printer_data(char* line, PRINTER* printer_s)
{
    PRINTER printer;
    if(sscanf(line, "%s %s %s %s", printer.name, printer.brand,
       printer.model, printer.ubication) < 4)
    {
        printf("[!] Incomplete entry in printers config file\n");
        return 1;
    }
    load_printer_data(printer_s, printer);
    return 0;
}


int get_printer_from_file(FILE* file, TLISTA* printers_list)
{
    char line[PRINTER_STR_MAX];
    PRINTER* printer_s;
    if(file == NULL)
    {
        printf("[!] Invalid file pointer\n");
        return 1;
    }
    while(fgets(line, PRINTER_STR_MAX, file))
    {
        printf("[>] Loading printer_s: %s", line);
        printer_s = new_printer_structure();
        if(!parse_printer_data(line, printer_s))
        {
            insertarElementoLista(printers_list, finLista(*printers_list), *printer_s);
        }
        else
        {
            free_printer_structure(*printer_s);
            free(printer_s);
            return 1;
        }
    }
    return 0;
}


int load_initial_data(char* filename, TLISTA* printers_list)
{
    FILE* f;
    if(*printers_list != NULL)
    {
        printf("[!] Load initial data requires an empty list. Exiting\n");
        return 1;
    }
    crearLista(printers_list);
    f = fopen(filename, "r");
    if(f == NULL)
    {
        printf("[!] Filename '%s' not found\n", filename);
        delete_list(printers_list);
        return 1;
    }
    if(get_printer_from_file(f, printers_list))
    {
        delete_list(printers_list);
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}


int get_printer_data(TLISTA* printers_list, char* printer_name, PRINTER* printer_s)
{
    TPOSICION current;
    TPOSICION ultimo = finLista(*printers_list);
    if(esListaVacia(*printers_list))
    {
        printf("[!] No printers connected\n");
        return 1;
    }
    for(current = primeroLista(*printers_list); current != ultimo;
        current = siguienteLista(*printers_list, current))
    {
        recuperarElementoLista(*printers_list, current, printer_s);
        if(!strcmp(printer_s->name, printer_name))
        {
            return 0;
        }
    }
    printf("[!] Printer %s is not connected\n", printer_name);
    return 1;
}


void add_printer(TLISTA* printers_list, PRINTER printer_s)
{
    PRINTER temp_printer;
    if(get_printer_data(printers_list, printer_s.name, &temp_printer))
    {
        insertarElementoLista(printers_list, primeroLista(*printers_list), printer_s);
    }
    else
    {
        printf("[!] Printer named %s already exists\n", printer_s.name);
        free_printer_structure(printer_s);
    }
}


void new_task(TLISTA* printers_list, char* printer_name, int task)
{
    PRINTER* current_element;
    current_element = malloc(sizeof(PRINTER));
    if(!get_printer_data(printers_list, printer_name, current_element))
    {
        anadirElementoCola(&current_element->tasks, task);
        (*current_element->tasks_count)++;
        printf("[+] Task %d added to %s\n", task, current_element->name);
    }
    free(current_element);
}


void show_pending_tasks(TLISTA* printers_list, char* printer_name)
{
    PRINTER current_printer;
    TIPOELEMENTOCOLA task;
    int length;
    if(!get_printer_data(printers_list, printer_name, &current_printer))
    {
        length = *current_printer.tasks_count;
        printf("[%d pending tasks for %s]\n", length, printer_name);
        for(int i = 0; i < length && i < 10; i++)
        {
            consultarPrimerElementoCola(current_printer.tasks, &task);
            anadirElementoCola(&current_printer.tasks, task);
            suprimirElementoCola(&current_printer.tasks);
            printf("| Pending task %d: task %d\n", i + 1, task);
        }
    }
}


void print_task(TLISTA* printers_list, char* printer_name)
{
    PRINTER printer_data;
    TIPOELEMENTOCOLA task;
    if(!get_printer_data(printers_list, printer_name, &printer_data))
    {
        if(!esColaVacia(printer_data.tasks))
        {
            consultarPrimerElementoCola(printer_data.tasks, &task);
            suprimirElementoCola(&printer_data.tasks);
            (*printer_data.tasks_count)--;
            printf("[>] Printing task %d in %s\n", task, printer_name);
        }
        else
            printf("[!] Printer have not pending tasks\n");
    }
}


void show_lowest_load_printer(TLISTA* printers_list)
{
    PRINTER current_printer;
    TPOSICION ultimo  = finLista(*printers_list), current;
    char* ll_printers = NULL;
    int lowest_load = MAX_LOAD, lowest_load_n = 0;
    if(!esListaVacia(*printers_list))
        for(current = primeroLista(*printers_list); current != ultimo;
            current = siguienteLista(*printers_list, current))
        {
            recuperarElementoLista(*printers_list, current, &current_printer);

            if(*current_printer.tasks_count < lowest_load)
            {
                lowest_load   = *current_printer.tasks_count;
                lowest_load_n = 0;
                free(ll_printers);
            }
            if(*current_printer.tasks_count == lowest_load)
            {
                ll_printers = realloc(ll_printers, NAME_LEN * ++lowest_load_n);
                strncpy(ll_printers + NAME_LEN * (lowest_load_n - 1),
                current_printer.name, NAME_LEN);
            }
        }
    else
        printf("[!] No printers connected\n");
    for(int i = 0; i < lowest_load_n; i++)
        printf("[>] Printer %s: Pending tasks: %d\n", ll_printers + i * NAME_LEN, lowest_load);
    free(ll_printers);
}
