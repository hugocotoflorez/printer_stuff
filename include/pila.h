

typedef int TELEMENTO ;	
		/* tipo de datos correspondiente a los elementos de la pila */

typedef struct  nodo 	{	TELEMENTO dato;
                       struct nodo * sig;	} TNodo;
                       
typedef TNodo *  TPILA;
	
void PilaVacia (TPILA *p);
void Push (TPILA * p , TELEMENTO e);
int EsVacia (TPILA  p );
void Cima (TPILA  p, TELEMENTO * pe);
void Pop(TPILA * p);

