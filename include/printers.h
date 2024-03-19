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
 * Libera la memoria asignada a una estructura de impresora y a sus tareas.
 * Parámetros:
 *  - printer_s: La estructura de impresora a liberar.
 * Retorno: void
 */
void free_printer_structure(PRINTER printer_s);

/**
 * new_printer_structure
 * Crea una nueva estructura de impresora, inicializando sus campos.
 * Retorno: Puntero a la nueva estructura de impresora.
 */
void* new_printer_structure();

/**
 * allocate_printer
 * Asigna memoria para una nueva estructura de impresora y la inicializa con los valores dados.
 * Parámetros:
 *  - name: Nombre de la impresora.
 *  - brand: Marca de la impresora.
 *  - model: Modelo de la impresora.
 *  - ubication: Ubicación de la impresora.
 * Retorno: Puntero a la nueva estructura de impresora.
 */
PRINTER* allocate_printer(const char* name, const char* brand, const char* model, const char* ubication);

/**
 * delete_list
 * Elimina una lista de impresoras, liberando la memoria asociada a cada una de ellas.
 * Parámetros:
 *  - list: Puntero a la lista de impresoras a eliminar.
 * Retorno: void
 */
void delete_list(TLISTA* list);

/**
 * delete_printer
 * Elimina una impresora de la lista de impresoras.
 * Parámetros:
 *  - printers_list: Puntero a la lista de impresoras.
 *  - printer_name: Nombre de la impresora a eliminar.
 * Retorno: 0 si la impresora se eliminó correctamente, 1 si no se encontró la impresora.
 */
int delete_printer(TLISTA* printers_list, char* printer_name);

/**
 * avaliable_printers
 * Muestra las impresoras disponibles en la lista.
 * Parámetros:
 *  - printers_list: Lista de impresoras.
 * Retorno: void
 */
void avaliable_printers(TLISTA printers_list);

/**
 * parse_printer_data
 * Analiza una línea de datos de impresora y crea una estructura de impresora.
 * Parámetros:
 *  - line: Línea de datos de impresora.
 * Retorno: Puntero a la estructura de impresora creada, NULL si hubo un error en el análisis.
 */
PRINTER* parse_printer_data(char* line);

/**
 * get_printer_from_file
 * Lee datos de impresoras desde un archivo y los añade a la lista de impresoras.
 * Parámetros:
 *  - file: Puntero al archivo que contiene los datos de impresoras.
 *  - printers_list: Puntero a la lista de impresoras.
 * Retorno: 0 si se leyeron los datos correctamente, 1 si hubo un error.
 */
int get_printer_from_file(FILE* file, TLISTA* printers_list);

/**
 * load_initial_data
 * Carga datos iniciales de impresoras desde un archivo y los añade a la lista de impresoras.
 * Parámetros:
 *  - filename: Nombre del archivo que contiene los datos de impresoras.
 *  - printers_list: Puntero a la lista de impresoras.
 * Retorno: 0 si se cargaron los datos correctamente, 1 si hubo un error.
 */
int load_initial_data(char* filename, TLISTA* printers_list);

/**
 * rewrite_printers_file
 * Escribe los datos de impresoras en un archivo.
 * Parámetros:
 *  - filename: Nombre del archivo en el que se escribirán los datos de impresoras.
 *  - printers_list: Puntero a la lista de impresoras.
 * Retorno: 0 si se escribieron los datos correctamente, 1 si hubo un error.
 */
int rewrite_printers_file(char* filename, TLISTA* printers_list);

/**
 * get_printer_data
 * Obtiene los datos de una impresora de la lista de impresoras.
 * Parámetros:
 *  - printers_list: Puntero a la lista de impresoras.
 *  - printer_name: Nombre de la impresora cuyos datos se desean obtener.
 *  - printer_s: Puntero a la estructura donde se almacenarán los datos de la impresora.
 * Retorno: 0 si se encontraron los datos de la impresora, 1 si la impresora no está en la lista.
 */
int get_printer_data(TLISTA* printers_list, char* printer_name, PRINTER* printer_s);

/**
 * add_printer
 * Añade una impresora a la lista de impresoras.
 * Parámetros:
 *  - printers_list: Puntero a la lista de impresoras.
 *  - printer_s: Estructura de impresora a añadir a la lista.
 * Retorno: void
 */
void add_printer(TLISTA* printers_list, PRINTER printer_s);

/**
 * new_task
 * Agrega una nueva tarea a una impresora.
 * Parámetros:
 *  - printers_list: Puntero a la lista de impresoras.
 *  - printer_name: Nombre de la impresora a la que se añadirá la tarea.
 *  - task: Número de la tarea a añadir.
 * Retorno: void
 */
void new_task(TLISTA* printers_list, char* printer_name, int task);

/**
 * show_pending_tasks
 * Muestra las tareas pendientes de una impresora.
 * Parámetros:
 *  - printers_list: Puntero a la lista de impresoras.
 *  - printer_name: Nombre de la impresora de la que se mostrarán las tareas pendientes.
 * Retorno: void
 */
void show_pending_tasks(TLISTA* printers_list, char* printer_name);

/**
 * print_task
 * Imprime la próxima tarea de una impresora.
 * Parámetros:
 *  - printers_list: Puntero a la lista de impresoras.
 *  - printer_name: Nombre de la impresora de la que se imprimirá la tarea.
 * Retorno: void
 */
void print_task(TLISTA* printers_list, char* printer_name);

/**
 * show_lowest_load_printer
 * Muestra en la consola la impresora o impresoras con la carga más baja.
 * Parámetros:
 *  - printers_list: Puntero a la lista de impresoras.
 * Retorno: void
 */
void show_lowest_load_printer(TLISTA* printers_list);

#endif // !_PRINTERS_H

