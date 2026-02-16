/*DRAGAN Marius 312CB*/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct arbore{
    int frec;
    char *nume;
    struct arbore * st;
    struct arbore * dr;

}Tarbore;
typedef struct lista_sateliti{
    int frec;
    char *nume;
    Tarbore * nod;
}TLista_sateliti;

typedef int (*TFCmp)(TLista_sateliti , TLista_sateliti);

typedef struct Heap {
    int nrMax, nrElem;
    TLista_sateliti *v;
    TFCmp comp;
} THeap;

typedef struct celula
{ 
  Tarbore *nod;
  struct celula *urm;
} TCelula, *TLista;

typedef struct coada
{ 
  TLista inc;
  TLista sf;
} TCoada;

Tarbore * cerinta1(FILE *in);
THeap* AlocaHeap(int nrMax, TFCmp comp);
int RelMinHeap(TLista_sateliti a, TLista_sateliti b);
Tarbore * create_nod(int frec,char *str,Tarbore *st,Tarbore *dr);
void DistrugeHeap(THeap **h);
void pushUP(THeap *h,int poz);
void InsertHeap(THeap *h,int frec,char *str,Tarbore *copilst,Tarbore *copildr);
void pushDown(int poz,THeap *h);
TLista_sateliti ExtrHeap(THeap *h);
Tarbore * cerinta1(FILE *in);
TCoada* InitQ ();
int IntrQ(TCoada *c, Tarbore * nod);
Tarbore *ExtrQ(TCoada *c);
void afisare_cerinta1(Tarbore * arb,FILE *out);
void cerinta2(FILE *in,Tarbore * arb,FILE *out);
void ParcurgereRSD(Tarbore *arb,char *nume_cod,char rezultat[1000],int nr,int *ok);
void cerinta3(FILE * in, Tarbore *arb,FILE *out );
void ParcurgereRSD4(Tarbore * arb,int *ok,char * cautat);
void verificare_existenta_in_subarbore(Tarbore *arb,char ** nume_cod,int nr_cod,char **rez);
void cerinta4(FILE *in,Tarbore *arb,FILE * out);
void Distrugerearb(Tarbore * arb);