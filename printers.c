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
        printf("\e[33mAllocating \t%d Bytes\e[0m\n", size);
    malloc_counter++;
    return malloc(size);
}

// can be redefine as just free
void FREE(void* ptr)
{
    if(DEBUG > 0)
        printf("\e[33mFreeing \t%p \e[0m\n", ptr);
    malloc_counter--;
    free(ptr);
}


void* new_printer_structure()
{
    PRINTER* main;
    main = MALLOC(sizeof(PRINTER));
    if(main == NULL)
    {
        printf("Cannot allocate memory\n");
        return NULL;
    }
    main->name = MALLOC(NAME_LEN);
    if(main->name == NULL)
    {
        printf("Cannot allocate memory\n");
        return NULL;
    }
    main->brand = MALLOC(BRAND_LEN);
    if(main->brand == NULL)
    {
        printf("Cannot allocate memory\n");
        return NULL;
    }
    main->model = MALLOC(MODEL_LEN);
    if(main->model == NULL)
    {
        printf("Cannot allocate memory\n");
        return NULL;
    }
    main->ubication = MALLOC(UBICATION_LEN);
    if(main->ubication == NULL)
    {
        printf("Cannot allocate memory\n");
        return NULL;
    }
    crearCola(&(main->tasks));
    return main;
}


void free_printer_structure(PRINTER printer_s)
{
    FREE(printer_s.name);
    FREE(printer_s.brand);
    FREE(printer_s.model);
    FREE(printer_s.ubication);
    destruirCola(&(printer_s.tasks));
}


void delete_list(TLISTA* list)
{
    TPOSICION current;
    PRINTER current_element;
    TPOSICION ultimo = finLista(*list);
    if(esListaVacia(*list))
    {
        printf("Lista vacia\n");
    }
    for(current = primeroLista(*list); current != ultimo;
        current = siguienteLista(*list, current))
    {
        recuperarElementoLista(*list, current, &current_element);
        printf("Removing connection with printer: %s %s %s %s\n",
        current_element.name, current_element.brand, current_element.model,
        current_element.ubication);
        free_printer_structure(current_element);
        malloc_counter--;
    }
    destruirLista(list);
    printf("Mallocs without free: %d\n", malloc_counter);
}

// impleentar get printer data
int delete_printer(TLISTA* printers_list, char* printer_name)
{
    printf("To Delete: %s\n", printer_name);
    TPOSICION current;
    PRINTER current_element;
    TPOSICION ultimo = finLista(*printers_list);

    if(esListaVacia(*printers_list))
    {
        printf("Printer %s is not connected\n", printer_name);
        return 1;
    }

    for(current = primeroLista(*printers_list); current != ultimo;
        current = siguienteLista(*printers_list, current))
    {
        recuperarElementoLista(*printers_list, current, &current_element);
        if(!strcmp(current_element.name, printer_name))
        {
            printf("Removing connection with printer: %s %s %s %s\n",
            current_element.name, current_element.brand, current_element.model,
            current_element.ubication);
            free_printer_structure(current_element);
            suprimirElementoLista(*printers_list, current);
            malloc_counter--;
            return 0;
        }
    }
    printf("Printer %s is not connected\n", printer_name);
    return 1;
}


void avaliable_printers(TLISTA printers_list)
{
    TPOSICION current;
    PRINTER current_element;
    TPOSICION ultimo = finLista(printers_list);
    for(current = primeroLista(printers_list); current != ultimo;
        current = siguienteLista(printers_list, current))
    {
        recuperarElementoLista(printers_list, current, &current_element);
        printf("Printer: %s %s %s %s\n", current_element.name,
        current_element.brand, current_element.model, current_element.ubication);
    }
}


void add_printer(TLISTA* printers_list, PRINTER printer)
{
    insertarElementoLista(printers_list, primeroLista(*printers_list), printer);
}


int parse_printer_data(char* line, PRINTER* printer_s)
{
    if(sscanf(line, "%s %s %s %s", printer_s->name, printer_s->brand,
       printer_s->model, printer_s->ubication) < 4)
    {
        printf("One or more incomplete entries in printers config file\n");
        printf("%s %s %s %s\n", printer_s->name, printer_s->brand,
        printer_s->model, printer_s->ubication);
        return 1;
    }
    printf("Loaded printer: "
           "%s %s %s %s\n",
    printer_s->name, printer_s->brand, printer_s->model, printer_s->ubication);
    return 0;
}

int get_printer_from_file(FILE* file, TLISTA* printers_list)
{
    char* line;
    PRINTER* printer_s;
    if(file == NULL)
    {
        printf("Invalid file pointer\n");
        return 1;
    }
    line = MALLOC(sizeof(char) * PRINTER_STR_MAX);
    if(line == NULL)
    {
        printf("Cannot allocate memory. Exiting\n");
        return 1;
    }
    while(fgets(line, PRINTER_STR_MAX, file))
    {
        printf("Loading Printer: %s", line);
        printer_s = new_printer_structure();
        if(!parse_printer_data(line, printer_s))
        {
            insertarElementoLista(printers_list, finLista(*printers_list), *printer_s);
        }
        else
        {
            free_printer_structure(*printer_s);
            FREE(printer_s);
            FREE(line);
            return 1;
        }
    }
    FREE(line);
    return 0;
}

int load_initial_data(char* filename, TLISTA* printers_list)
{
    FILE* f;
    if(*printers_list != NULL)
    {
        printf("Load initial data requires an empty list. Exiting\n");
        return 1;
    }
    crearLista(printers_list);
    f = fopen(filename, "r");
    if(f == NULL)
    {
        printf("Filename '%s' not found\n", filename);
        delete_list(printers_list);
        printf("Mallocs without free: %d\n", malloc_counter);
        return 1;
    }
    if(get_printer_from_file(f, printers_list))
    {
        delete_list(printers_list);
        printf("Mallocs without free: %d\n", malloc_counter);
        return 1;
    }
    return 0;
}

// revisar. 3am
int get_printer_data(TLISTA* printers_list, char* printer_name, PRINTER* printer)
{
    TPOSICION current;
    TPOSICION ultimo = finLista(*printers_list);
    if(esListaVacia(*printers_list))
    {
        printf("Printer %s is not connected\n", printer_name);
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
    printf("Printer %s is not connected\n", printer_name);
    return 1;
}


// revisar. 3am
void new_task(TLISTA* printers_list, char* printer_name, int task)
{
    PRINTER current_element;
    if(!get_printer_data(printers_list, printer_name, &current_element))
    {
        anadirElementoCola(&current_element.tasks, task);
        printf("Task %d added to %s\n", task, current_element.name);
        return;
    }
    printf("Printer %s is not connected\n", printer_name);
    return;
}

// revisar. 3am
void get_pending_tasks(TLISTA* printers_list, char* printer_name)
{
    PRINTER current_element;
    TIPOELEMENTOCOLA task;
    TCOLA temp_queue;
    if(!get_printer_data(printers_list, printer_name, &current_element))
    {
        temp_queue = current_element.tasks;
        while(!esColaVacia(temp_queue))
        {
            consultarPrimerElementoCola(temp_queue, &task);
            suprimirElementoCola(&temp_queue);
        }
        return;
    }
    printf("Printer %s is not connected\n", printer_name);
    return;
}


// hacer funcion get_printer_data
