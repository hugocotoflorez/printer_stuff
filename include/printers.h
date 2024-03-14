#ifndef _PRINTERS_H
#define _PRINTERS_H

#include "lista.h"
#include <stdio.h>

#define PRINTER_STR_MAX 100
#define UBICATION_LEN 20
#define MODEL_LEN 20
#define BRAND_LEN 20
#define NAME_LEN 20
#define MAX_LOAD 100

/**
 * This library depends on lista.h and its
 * designed to use with PRINTER structures
 * declared inside lista.h
 *
 * Author: Hugo Coto Florez
 */


/**
 * free_printer_structure
 */
void free_printer_structure(PRINTER printer_s);

/**
 * new_printer_structure
 */
void* new_printer_structure();

/**
 * delete_list
 */
void delete_list(TLISTA* list);

/**
 * delete_printer
 */
int delete_printer(TLISTA* printers_list, char* printer_name);

/**
 * avaliable_printers
 */
void avaliable_printers(TLISTA printers_list);

/**
 * parse_printer_data
 */
int parse_printer_data(char* line, PRINTER* printer_s);

/**
 * get_printer_from_file
 */
int get_printer_from_file(FILE* file, TLISTA* printers_list);

/**
 * load_initial_data
 */
int load_initial_data(char* filename, TLISTA* printers_list);

/**
 * rewrite_printers_file
 */
int rewrite_printers_file(char* filename, TLISTA* printers_list);

/**
 * get_printer_data
 */
int get_printer_data(TLISTA* printers_list, char* printer_name, PRINTER* printer_s);

/**
 * add_printer
 */
void add_printer(TLISTA* printers_list, PRINTER printer_s);

/**
 * new_task
 */
void new_task(TLISTA* printers_list, char* printer_name, int task);

/**
 * show_pending_tasks
 */
void show_pending_tasks(TLISTA* printers_list, char* printer_name);

/**
 * print_task
 */
void print_task(TLISTA* printers_list, char* printer_name);

/**
 * show_lowest_load_printer
 */
void show_lowest_load_printer(TLISTA* printers_list);


#endif // !_PRINTERS_H
