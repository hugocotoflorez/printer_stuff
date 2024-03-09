#ifndef _PRINTERS_H
#define _PRINTERS_H

#include "lista.h"
#include <stdio.h>

#define DEBUG 1

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


extern void FREE(void*);

void show_lowest_load_printer(TLISTA* printers_list);
extern void* new_printer_structure();
void new_task(TLISTA* printers_list, char* printer_name, int task);

void print_task(TLISTA* printers_list, char* printer_name);
void show_pending_tasks(TLISTA* printers_list, char* printer_name);

extern void add_printer(TLISTA* printers_list, PRINTER printer);

/**
 * free_printer_structure(PRINTER) -> void
 * Free the fields inside the structure provided.
 * Needed because lista.h/destruirLista dont free
 * structure corectly.
 */
extern void free_printer_structure(PRINTER printer_s);

/**
 * delete_list(TLISTA*) -> void
 * Delete a non empty list correctly. It merges
 * free_printer_structure with list.h/destruirLista
 * to correctly destroy a list of PRINTER structures.
 */
extern void delete_list(TLISTA* list);

/**
 * delete_printer(TLISTA*, char*) -> int (error value)
 * Remove a single printer by name
 * If its not coneected returns 1,
 * otherwise remove it and returns 0.
 * If there are more than one with the same
 * name it will only delete one.
 */
extern int delete_printer(TLISTA* printers_list, char* printer_name);

/**
 * __debug_print_list(TLISTA) -> void
 * Print the name of the printers stored in the
 * list provided. Must be used only for debugging
 * purposes.
 */
extern void avaliable_printers(TLISTA printers_data);

/**
 * load_initial_data(char*, TLISTA*) -> int (error value)
 * Move the name of printers stored in 'filename'
 * to an list. This list is created inside the function
 * so it must be passed as an empty TLISTA.
 * ERRORS:
 *  all errors generated inside the function
 *  are a break point with return value 1.
 *  Otherwise, if no errors, the return value is 0.
 */
extern int load_initial_data(char* filename, TLISTA* printers_data);

/**
 * get_printer_from_file(FILE*, TLISTA*) -> int (error value)
 * Get the printers stored in a file.
 * Its recommended to use load_initial_data because
 * its the hight level implementation of this function.
 */
extern int get_printer_from_file(FILE* file, TLISTA* printers_data);

/**
 * parse_printer_data(char*, PRINTER*) -> int (error value)
 * Given a string that contains printers information
 * it extracts the data and store it in a given struct.
 */
extern int parse_printer_data(char* line, PRINTER* printer_s);


#endif // !_PRINTERS_H
