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
    PRINTER* printer_s     = malloc(sizeof(PRINTER));
    printer_s->name        = NULL;
    printer_s->brand       = NULL;
    printer_s->model       = NULL;
    printer_s->ubication   = NULL;
    printer_s->tasks_count = malloc(sizeof(int));
    crearCola(&printer_s->tasks);
    if((printer_s == NULL) || (printer_s->tasks_count == NULL))
    {
        free_printer_structure(*printer_s);
        printf("[!] Cannot allocate memory\n");
        return NULL;
    }
    *printer_s->tasks_count = 0;
    return printer_s;
}


PRINTER* allocate_printer(const char* name, const char* brand, const char* model, const char* ubication)
{
    PRINTER* new_printer = new_printer_structure();
    if(new_printer == NULL)
        return NULL;
    new_printer->name      = malloc(sizeof(char) * strlen(name));
    new_printer->brand     = malloc(sizeof(char) * strlen(brand));
    new_printer->model     = malloc(sizeof(char) * strlen(model));
    new_printer->ubication = malloc(sizeof(char) * strlen(ubication));
    if((new_printer->name == NULL) || (new_printer->brand == NULL) ||
    (new_printer->model == NULL) || (new_printer->ubication == NULL))
    {
        free_printer_structure(*new_printer);
        printf("[!] Cannot allocate memory\n");
        return NULL;
    }
    strncpy(new_printer->name, name, NAME_LEN);
    strncpy(new_printer->brand, brand, BRAND_LEN);
    strncpy(new_printer->model, model, MODEL_LEN);
    strncpy(new_printer->ubication, ubication, UBICATION_LEN);
    return new_printer;
}


void delete_list(TLISTA* list)
{
    TPOSICION current_position;
    PRINTER current_element;
    TPOSICION last_position = finLista(*list);
    if(!esListaVacia(*list))
        for(current_position = primeroLista(*list); current_position != last_position;
            current_position = siguienteLista(*list, current_position))
        {
            recuperarElementoLista(*list, current_position, &current_element);
            printf("[>] Removing connection with printer: \n\t%s %s %s %s\n",
            current_element.name, current_element.brand, current_element.model,
            current_element.ubication);
            free_printer_structure(current_element);
        }
    destruirLista(list);
}


int delete_printer(TLISTA* printers_list, char* printer_name)
{
    TPOSICION current_position;
    PRINTER current_element;
    TPOSICION last_position = finLista(*printers_list);
    printf("[+] To Delete: %s\n", printer_name);
    if(esListaVacia(*printers_list))
    {
        printf("[!] Printer %s is not connected\n", printer_name);
        return 1;
    }
    for(current_position = primeroLista(*printers_list); current_position != last_position;
        current_position = siguienteLista(*printers_list, current_position))
    {
        recuperarElementoLista(*printers_list, current_position, &current_element);
        if(!strcmp(current_element.name, printer_name))
        {
            printf("[>] Removing connection with printer: \n\t%s %s %s %s\n",
            current_element.name, current_element.brand, current_element.model,
            current_element.ubication);
            free_printer_structure(current_element);
            suprimirElementoLista(printers_list, current_position);
            return 0;
        }
    }
    printf("[!] Printer %s is not connected\n", printer_name);
    return 1;
}


void avaliable_printers(TLISTA printers_list)
{
    TPOSICION current_position;
    PRINTER current_element;
    TPOSICION last_position = finLista(printers_list);
    if(!esListaVacia(printers_list))
        printf("[Avaliable printers]\n");
    for(current_position = primeroLista(printers_list); current_position != last_position;
        current_position = siguienteLista(printers_list, current_position))
    {
        recuperarElementoLista(printers_list, current_position, &current_element);
        printf("|  %s %s %s %s\n", current_element.name, current_element.brand,
        current_element.model, current_element.ubication);
    }
}


PRINTER* parse_printer_data(char* line)
{
    char name[NAME_LEN], brand[BRAND_LEN], model[MODEL_LEN], ubication[UBICATION_LEN];
    if(sscanf(line, "%s %s %s %s", name, brand, model, ubication) < 4)
    {
        printf("[!] Incomplete entry in printers config file\n");
        return NULL;
    }
    return allocate_printer(name, brand, model, ubication);
}


int get_printer_from_file(FILE* file, TLISTA* printers_list)
{
    char line[PRINTER_STR_MAX];
    PRINTER* printer_s;
    PRINTER temp;
    if(file == NULL)
    {
        printf("[!] Invalid file pointer\n");
        return 1;
    }
    while(fgets(line, PRINTER_STR_MAX, file))
    {
        if((printer_s = parse_printer_data(line)) != NULL)
        {
            add_printer(printers_list, *printer_s);
            continue;
        }
        return 1;
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


int rewrite_printers_file(char* filename, TLISTA* printers_list)
{
    FILE* f;
    TPOSICION current_position, last_position = finLista(*printers_list);
    PRINTER current_printer;
    f = fopen(filename, "w");
    if(f == NULL)
    {
        printf("[!] Filename '%s' not found\n", filename);
        return 1;
    }
    if(!esListaVacia(*printers_list))
        for(current_position = primeroLista(*printers_list); current_position != last_position;
            current_position = siguienteLista(*printers_list, current_position))
        {
            recuperarElementoLista(*printers_list, current_position, &current_printer);
            printf("[+] Saved: %s %s %s %s\n", current_printer.name,
            current_printer.brand, current_printer.model, current_printer.ubication);
            fprintf(f, "%s %s %s %s\n", current_printer.name, current_printer.brand,
            current_printer.model, current_printer.ubication);
        }
    fclose(f);
    return 0;
}


int get_printer_data(TLISTA* printers_list, char* printer_name, PRINTER* printer_s)
{
    TPOSICION current_position;
    TPOSICION last_position = finLista(*printers_list);
    if(esListaVacia(*printers_list))
    {
        return 1;
    }
    for(current_position = primeroLista(*printers_list); current_position != last_position;
        current_position = siguienteLista(*printers_list, current_position))
    {
        recuperarElementoLista(*printers_list, current_position, printer_s);
        if(!strcmp(printer_s->name, printer_name))
        {
            return 0;
        }
    }
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
        printf("[!] Printer %s already exists\n", printer_s.name);
        free_printer_structure(printer_s);
    }
}


void new_task(TLISTA* printers_list, char* printer_name, int task)
{
    PRINTER current_element;
    if(!get_printer_data(printers_list, printer_name, &current_element))
    {
        anadirElementoCola(&current_element.tasks, task);
        (*current_element.tasks_count)++;
        printf("[+] Task %d added to %s\n", task, current_element.name);
    }
    else
        printf("[!] Printer %s is not connected\n", printer_name);
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
            printf("|  Pending task %d: task %d\n", i + 1, task);
        }
    }
    else
        printf("[!] Printer %s is not connected\n", printer_name);
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
    TPOSICION last_position = finLista(*printers_list), current_position;
    char* ll_printers       = NULL;
    int lowest_load = MAX_LOAD, lowest_load_n = 0;
    if(!esListaVacia(*printers_list))
        for(current_position = primeroLista(*printers_list); current_position != last_position;
            current_position = siguienteLista(*printers_list, current_position))
        {
            recuperarElementoLista(*printers_list, current_position, &current_printer);

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
