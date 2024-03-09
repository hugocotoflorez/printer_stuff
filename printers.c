#include "include/printers.h"
#include "include/cola.h"
#include "include/lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * store malloc calls and free calls
 * -- just for debugging
 *  +1 <- malloc call
 *  -1 <- free call
 */
int malloc_counter = 0;
//
// can be redefine as just malloc
void* MALLOC(int size)
{
    if(DEBUG > 0)
        printf("\e[33m[#] Allocating \t%d Bytes\e[0m\n", size);
    malloc_counter++;
    return malloc(size);
}

// can be redefine as just free
void FREE(void* ptr)
{
    if(DEBUG > 0)
        printf("\e[33m[#] Freeing \t%p \e[0m\n", ptr);
    malloc_counter--;
    free(ptr);
}


void free_printer_structure(PRINTER printer_s)
{
    FREE(printer_s.name);
    FREE(printer_s.brand);
    FREE(printer_s.model);
    FREE(printer_s.ubication);
    FREE(printer_s.tasks_count);
    destruirCola(&(printer_s.tasks));
}


void* new_printer_structure()
{
    PRINTER* main;
    main              = MALLOC(sizeof(PRINTER));
    main->name        = MALLOC(NAME_LEN);
    main->brand       = MALLOC(BRAND_LEN);
    main->model       = MALLOC(MODEL_LEN);
    main->ubication   = MALLOC(UBICATION_LEN);
    main->tasks_count = MALLOC(sizeof(int));
    crearCola(&(main->tasks));
    if((main == NULL) || (main->name == NULL) || (main->brand == NULL) ||
    (main->model == NULL) || (main->ubication == NULL) || (main->tasks_count == NULL))
    {
        free_printer_structure(*main);
        printf("[!] Cannot allocate memory\n");
        return NULL;
    }
    *main->tasks_count = 0;
    return main;
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
            printf("[>] Removing connection with printer: %s %s %s %s\n",
            current_element.name, current_element.brand, current_element.model,
            current_element.ubication);
            free_printer_structure(current_element);
            malloc_counter--;
        }
    destruirLista(list);
    if(DEBUG > 0)
        printf("[#] Mallocs without free: %d\n", malloc_counter);
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
            printf("[>] Removing connection with printer: %s %s %s %s\n",
            current_element.name, current_element.brand, current_element.model,
            current_element.ubication);
            free_printer_structure(current_element);
            suprimirElementoLista(printers_list, current);
            malloc_counter--;
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
    if(sscanf(line, "%s %s %s %s", printer_s->name, printer_s->brand,
       printer_s->model, printer_s->ubication) < 4)
    {
        printf("[!] Incomplete entry in printers config file\n");
        return 1;
    }
    printf("| Done\n");
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
        printf("[>] Loading printer: %s", line);
        printer_s = new_printer_structure();
        if(!parse_printer_data(line, printer_s))
        {
            insertarElementoLista(printers_list, finLista(*printers_list), *printer_s);
        }
        else
        {
            free_printer_structure(*printer_s);
            FREE(printer_s);
            return 1;
        }
    }
    return 0;
}


int load_initial_data(char* filename, TLISTA* printers_list)
{
    FILE* f;
    if(*printers_list!=NULL)
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
        if(DEBUG > 0)
            printf("[#] Mallocs without free: %d\n", malloc_counter);
        return 1;
    }
    if(get_printer_from_file(f, printers_list))
    {
        delete_list(printers_list);
        if(DEBUG > 0)
            printf("[#] Mallocs without free: %d\n", malloc_counter);
        return 1;
    }
    return 0;
}


int get_printer_data(TLISTA* printers_list, char* printer_name, PRINTER* printer)
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
        recuperarElementoLista(*printers_list, current, printer);
        if(!strcmp(printer->name, printer_name))
        {
            return 0;
        }
    }
    printf("[!] Printer %s is not connected\n", printer_name);
    return 1;
}


void add_printer(TLISTA* printers_list, PRINTER printer)
{
    PRINTER temp_printer;
    if(get_printer_data(printers_list, printer.name, &temp_printer))
    {
        insertarElementoLista(printers_list, primeroLista(*printers_list), printer);
    }
    else
    {
        printf("[!] Printer named %s already exists\n", printer.name);
        free_printer_structure(printer);
    }
}


void new_task(TLISTA* printers_list, char* printer_name, int task)
{
    PRINTER* current_element;
    current_element = MALLOC(sizeof(PRINTER));
    if(!get_printer_data(printers_list, printer_name, current_element))
    {
        anadirElementoCola(&current_element->tasks, task);
        (*current_element->tasks_count)++;
        printf("[+] Task %d added to %s\n", task, current_element->name);
    }
    FREE(current_element);
}


void show_pending_tasks(TLISTA* printers_list, char* printer_name)
{
    PRINTER current_printer;
    TIPOELEMENTOCOLA task;
    int length;
    if(!get_printer_data(printers_list, printer_name, &current_printer))
    {
        length = *current_printer.tasks_count;
        printf("[Pending tasks for %s: %d]\n", printer_name, length);
        for(int i = 0; i < length && i < 10; i++)
        {
            consultarPrimerElementoCola(current_printer.tasks, &task);
            anadirElementoCola(&current_printer.tasks, task);
            suprimirElementoCola(&current_printer.tasks);
            printf("| Pending task %d: task %d\n", i + 1, task);
        }
        if(!length)
            printf("[!] Printer have not pending tasks\n");
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
    TPOSICION current;
    PRINTER current_printer;
    char* ll_printers = NULL;
    TPOSICION ultimo  = finLista(*printers_list);
    int lowest_load   = MAX_LOAD;
    int lowest_load_n = 0;
    int load;
    if(esListaVacia(*printers_list))
        printf("[!] No printers connected\n");
    else
        for(current = primeroLista(*printers_list); current != ultimo;
            current = siguienteLista(*printers_list, current))
        {
            recuperarElementoLista(*printers_list, current, &current_printer);
            load = *current_printer.tasks_count;
            if(load < lowest_load)
            {
                lowest_load   = load;
                lowest_load_n = 0;
                free(ll_printers);
            }
            if(load <= lowest_load)
            {
                ll_printers = realloc(ll_printers, NAME_LEN * ++lowest_load_n);
                strncpy(ll_printers + NAME_LEN * (lowest_load_n - 1),
                current_printer.name, NAME_LEN);
            }
        }
    for(int i = 0; i < lowest_load_n; i++)
    {
        printf("[>] Printer %s: Pending tasks: %d\n", ll_printers + i * NAME_LEN, lowest_load);
    }
    free(ll_printers);
}
