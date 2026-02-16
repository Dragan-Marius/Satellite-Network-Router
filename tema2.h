/*DRAGAN Marius 312CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree
{
  int freq;
  char *name;
  struct tree *left;
  struct tree *right;

} Ttree;
typedef struct satellite_list
{
  int freq;
  char *name;
  Ttree *node;
} TSatellite;

typedef int (*TFCmp)(TSatellite, TSatellite);

typedef struct Heap
{
  int max_size, current_size;
  TSatellite *v;
  TFCmp comp;
} THeap;

typedef struct cell
{
  Ttree *node;
  struct cell *next;
} TCell, *TList;

typedef struct queue
{
  TList head;
  TList tail;
} TQueue;

Ttree *build_huffmann_tree(FILE *in);
THeap *alloc_heap(int max_size, TFCmp comp);
int RelMinHeap(TSatellite a, TSatellite b);
void displayHeap(THeap *h, int pos, FILE *out);
Ttree *create_node(int freq, char *str, Ttree *st, Ttree *dr);
void destroyHeap(THeap **h);
void pushUP(THeap *h, int poz);
void InsertHeap(THeap *h, int freq, char *str, Ttree *copilst, Ttree *copildr);
void pushDown(int poz, THeap *h);
TSatellite ExtrHeap(THeap *h);
TQueue *InitQ();
int enqueue(TQueue *c, Ttree *node);
Ttree *dequeue(TQueue *c);
void printTreeLevelOrder(Ttree *arb, FILE *out);
void decodeSattelitePaths(FILE *in, Ttree *arb, FILE *out);
void traverseAndEncode(Ttree *arb, char *name_cod, char rezultat[1000], int nr, int *ok);
void encodeSatellitePaths(FILE *in, Ttree *arb, FILE *out);
void findInSubtree(Ttree *arb, int *ok, char *cautat);
void checkExistenceInSubtree(Ttree *arb, char **name_cod, int nr_cod, char **rez);
void findCommonAncestor(FILE *in, Ttree *arb, FILE *out);
void destroTree(Ttree *arb);