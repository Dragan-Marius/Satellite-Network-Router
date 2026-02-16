/*DRAGAN Marius 312CB*/
#include "tema2.h"


THeap* AlocaHeap(int nrMax, TFCmp comp) {
    THeap* h = (THeap*) malloc(sizeof(struct Heap));
    if (!h) {
        return NULL;
    }

    h->v = (TLista_sateliti *) malloc(nrMax * sizeof(TLista_sateliti));
    if (!h->v) {
		free(h);
		return NULL;
	}

    h->nrMax = nrMax;
    h->nrElem = 0;
    h->comp = comp;

    return h;
}
Tarbore * create_nod(int frec,char *str,Tarbore *st,Tarbore *dr){
    Tarbore *aux=(Tarbore *)malloc(sizeof(Tarbore));
    if (aux==NULL)
    return NULL;
    aux->frec=frec;
    aux->nume=strdup(str);
    aux->dr=dr;
    aux->st=st;
    return aux;
    //creeam un nod din arborele nostru cu frecventa frec si numele str si copiii st,dr
}

int RelMinHeap(TLista_sateliti  a, TLista_sateliti b) {
    if (a.frec != b.frec)
	return a.frec < b.frec;
    return strcmp(a.nume, b.nume) < 0;
}

void AfisareHeap(THeap* h, int pos,FILE * out) {
    if (pos >= h->nrElem) {      
        fprintf(out,"-"); 
        return;
    }
    
    fprintf(out," %d<->%s ", h->v[pos].frec,h->v[pos].nume);

    if (2 * pos + 1 >= h->nrMax && 2 * pos + 2 >=h->nrMax)
        return;

	fprintf(out,"(");
	
    AfisareHeap(h, 2 * pos + 1,out);
	fprintf(out,",");
	
    AfisareHeap(h, 2 * pos + 2,out);
	fprintf(out,")");
}

void DistrugeHeap(THeap **h) {
    //eliberare heap
    for (int i=0; i < (*(h))->nrElem; i++)
        free((*(h))->v[i].nume);
	free((*h)->v);
	free(*h);
	*h = NULL; 
}

void pushUP(THeap *h, int poz) {
    int indice_tata = (poz-1)/2;
    while (poz > 0 && !h->comp(h->v[indice_tata], h->v[poz])) {
        TLista_sateliti aux = h->v[poz];
        h->v[poz] = h->v[indice_tata];
        h->v[indice_tata] = aux;
        poz = indice_tata;
        indice_tata = (poz-1)/2;
    }
    //reordonare heap dupa introducere
}

void InsertHeap(THeap *h, int frec, char *str, Tarbore *copilst, Tarbore *copildr) {
    h->v[h->nrElem].frec = frec;
    h->v[h->nrElem].nume = strdup(str);
    h->v[h->nrElem].nod = create_nod(frec,str,copilst,copildr);
    h->nrElem++;
    h->nrMax++;
    //inseram elementul la final si reordonam heap
    pushUP (h,h->nrElem-1);
}

void pushDown(int poz, THeap *h) {
    //reordonarea heap-ului dupa ce am extras elementul
    //luat din exercitiul din laborator
    int indice_fiu_st = 2 * poz + 1;
    while (indice_fiu_st<h->nrElem) {
        int s = indice_fiu_st;
        int indice_fiu_dr = 2 * poz + 2;
        if (indice_fiu_dr < h->nrElem) {
            if (h->v[s].frec != h->v[indice_fiu_dr].frec) {
                if (h->v[s].frec > h->v[indice_fiu_dr].frec) {
                    s = indice_fiu_dr;
                }
            }
            else {
                    if (strcmp(h->v[s].nume, h->v[indice_fiu_dr].nume) > 0) {
                        s = indice_fiu_dr;
                }
            }
        }
        if (h->v[poz].frec < h->v[s].frec)
        break;
        else if (h->v[poz].frec == h->v[s].frec && strcmp(h->v[poz].nume, h->v[s].nume) < 0) {
            break;
        }
        else {
            TLista_sateliti aux = h->v[poz];
            h->v[poz] = h->v[s];
            h->v[s] = aux;
            poz = s;
            indice_fiu_st = 2 * poz + 1;
        }
    }
}
TLista_sateliti ExtrHeap(THeap *h) {
    h->nrElem--;
    TLista_sateliti aux = h->v[0];
    h->v[0] = h->v[h->nrElem];
    h->nrMax--;
    //extragem primul element din heap si reordonam arborele de heap-ul dupa aceea
    pushDown(0,h);
    return aux;
}

Tarbore * cerinta1(FILE *in) {
    int n;
    fscanf(in,"%d",&n);
    TLista_sateliti * v = malloc(sizeof(TLista_sateliti)*n);
    if (v == NULL)
    return NULL;
    THeap* h = NULL;
    for (int i = 0; i < n; i++) {
        fscanf(in,"%d", &v[i].frec);
        char str[15];
        fscanf(in,"%s", str);
        v[i].nume = strdup(str);
        if (v[i].nume == NULL) {
            for (int j = 0; j < i; j++)
               free(v[j].nume);
            free(v);
            return NULL;
        }
        v[i].nod = create_nod(v[i].frec, v[i].nume, NULL, NULL);
        if (v[i].nod == NULL) {
            for (int j = 0;j < i; j++) {
                free(v[j].nod->nume);
                free(v[j].nod);
            }
            for (int j = 0; j < n; j++) {
                free(v[j].nume);
            free(v);
            return NULL;
            }
        }
    //citire noduri initiale
    }
    h = AlocaHeap(n, RelMinHeap);
    if (h == NULL){
        for (int i = 0; i < n; i++) {
            free(v[i].nume);
            free(v[i].nod->nume);
            free(v[i].nod);
        }
    }
    for (int i = 0; i < n; i++) {
        InsertHeap(h, v[i].frec, v[i].nume, v[i].nod->st, v[i].nod->dr);
        //inserare in heap
    }
    while (h->nrElem>1) {
        //construirea arborelui cat timp in heap avem cel putin 2 elemente
        TLista_sateliti aux_1 = ExtrHeap(h);
        TLista_sateliti aux_2 = ExtrHeap(h);
        if (aux_1.frec > aux_2.frec) {
            TLista_sateliti aux = aux_1;
            aux_1 = aux_2;
            aux_2 = aux;
        }
        else if (aux_1.frec == aux_2.frec && strcmp(aux_1.nume, aux_2.nume) > 0) {
            TLista_sateliti aux = aux_1;
            aux_1 = aux_2;
            aux_2 = aux;
        }
        char *str = malloc(strlen(aux_1.nume) + strlen(aux_2.nume) + 1);
        strcpy(str,aux_1.nume);
        strcat(str,aux_2.nume);
        int s = aux_1.frec;
        s = s+aux_2.frec;
       // Tarbore *nod_nou=create_nod(s,str,aux_1.nod,aux_2.nod);/*stanga dreapta*/
       // if (nod_nou==NULL)
           // return 0;
        InsertHeap(h, s, str, aux_1.nod, aux_2.nod);
        free(str);
        free(aux_1.nume);
        free(aux_2.nume);;
        //free(nod_nou);
    }
    //eliberari de memorie
    for (int i = 0; i < n; i++) {
        free(v[i].nume);
        free(v[i].nod->nume);
        free(v[i].nod);
    }
    free(v);
    Tarbore * aux;
    aux=h->v[h->nrElem-1].nod;
    DistrugeHeap(&h);
    return aux;
}

TCoada* InitQ () {
  TCoada* c;
  c = (TCoada*)malloc(sizeof(TCoada));
  if (!c) return NULL;                  
  c->inc = NULL;
  c->sf=NULL;
  return c;          
}

int IntrQ(TCoada *c, Tarbore * nod)   { 
  TLista aux;
  aux=(TLista)malloc(sizeof(TCelula));
  if (!aux) return 0;
  aux->nod = nod;
  aux->urm = NULL;
    if (c->sf == NULL) {
        c->inc = aux;
        c->sf = aux;
    }
    else {
        c->sf->urm = aux;
        c->sf = aux;
    }
  return 1;             
}

Tarbore *ExtrQ(TCoada *c)  {   
    TLista aux;
    Tarbore* aux_ret;
    aux=c->inc;
    aux_ret=aux->nod;
    aux=c->inc;
    c->inc=aux->urm;
    if (c->inc == NULL)
    c->sf = NULL;
    free(aux);  
    return aux_ret;                  
}

void afisare_cerinta1(Tarbore * arb, FILE *out) {
    //pentru afisarea pe niveluri a nodurilor am folosit o coada
    TCoada *c = InitQ();
    if (c == NULL) return;
    IntrQ(c, arb);
    int z = 1;
    //numarul de noduri de pe nivelul curent
    while ((c->inc) != NULL) {
        int i,j;
        j = 0;
        for (i = 0; i < z; i++) {
            Tarbore *aux = ExtrQ(c);
            //extragem din coada
            fprintf(out,"%d-%s", aux->frec, aux->nume);
            if (i!=(z-1))
            //daca nu suntem la ultimul nod
            fprintf(out," ");
            if (aux->st != NULL) {
                IntrQ(c, aux->st);
                //introducere in coada
                j++;
                //avem copil pe stanga
            }
            if (aux->dr != NULL) {
                IntrQ(c, aux->dr);
                //introducere in coada
                j++;
                //avem copil pe dreapta
            }
        }
        fprintf(out, "\n");
        z = j;
        //numarul de noduri de pe nivelul urmator(j)
    }
    free(c);
    c=NULL;
}

void cerinta2(FILE *in, Tarbore * arb, FILE *out) {
    int nr_codif;
    fscanf(in, "%d", &nr_codif);
    for (int i = 0; i < nr_codif; i++) {
        char str[1000];
        fscanf(in, "%s", str);
        Tarbore *ant,*p = arb;
        for (int j = 0;j < strlen(str); j++) {
            if (str[j] == '0') {  
                ant = p;
                p = p->st;
                //mergem pe stanga
            }
            if (str[j] == '1') {   
                ant = p;
                p = p->dr;
                //mergem pe dreapta
            }
            if (p == NULL) {
                //nodul era frunza si afisam frunza
                fprintf(out, "%s ", ant->nume);
                p = arb;
                //continuam de la radacina
                j--;
            }
        }
        if (p == NULL)
            fprintf(out, "%s ", ant->nume);
            //se poate si fara else
        else if (p->st == NULL && p->dr == NULL)
                fprintf(out, "%s", p->nume);
        fprintf(out, "\n");
    }
}

void ParcurgereRSD(Tarbore *arb, char *nume_cod, char rezultat[1000], int nr, int *ok) {
    if (arb == NULL || (*ok) == 1) return;
    if (strcmp(arb->nume,nume_cod) == 0 ){  
        rezultat[nr] = '\0';
        *ok = 1;
        return;
    }
    if (arb->st && (*ok) == 0) {
        rezultat[nr] = '0';
        ParcurgereRSD(arb->st, nume_cod, rezultat, nr+1, ok);
    }
    if (arb->dr && (*ok) == 0) {
        rezultat[nr] = '1';
        ParcurgereRSD(arb->dr, nume_cod, rezultat, nr+1, ok);
    }
    return ;
}

void cerinta3(FILE * in, Tarbore *arb, FILE *out ) {
    int nr_nod;
    fscanf(in, "%d", &nr_nod);
    //numarul de noduri ce trebuiesc codificate
    int lg = strlen(arb->nume);
    char *codificare_finala = malloc(1000 * nr_nod*sizeof(char));
    if (codificare_finala == NULL)
    return;
    //am folosit 1000 ca m am gandit de la exercitiul anterior,ca o codificare poate avea maxim 1000 de caractere
    //in caz de este pentru nod de pe ultimul nivel
    codificare_finala[0] = '\0';
    for (int i = 0; i < nr_nod; i++) {
        char *nume_nod = malloc(sizeof(char) * lg);//char nod[15]
        if (nume_nod == NULL) {
            free(codificare_finala);
            return;
        }
        fscanf(in, "%s", nume_nod);
        char rezultat[1000] = "";
        int ok = 0;
        ParcurgereRSD (arb, nume_nod, rezultat, 0, &ok);
        strcat(codificare_finala, rezultat);
        free(nume_nod);
    }
    fprintf(out,"%s", codificare_finala);
    free(codificare_finala);
}

void ParcurgereRSD4(Tarbore * arb, int *ok, char * cautat) {
    if (!arb) return;
    if (strcmp(arb->nume,cautat) == 0) {
        (*ok) = 1;
        return;
    }
    ParcurgereRSD4(arb->st, ok, cautat);
    ParcurgereRSD4(arb->dr, ok ,cautat);
    return ;
    //parcurgerea RSD pentru acest exercitiu
}

void verificare_existenta_in_subarbore(Tarbore *arb, char ** nume_cod, int nr_cod, char **rez) {
    int i;
    int ok;
    int nr = 0;
    if (arb == NULL) return;
    for (i =0 ; i <nr_cod; i++) {
        ok = 0;
        ParcurgereRSD4(arb, &ok, nume_cod[i]);
        //cautam in subarborele curent
        if (ok == 1) nr++;
    }
    if (nr == nr_cod) {
        if (*rez != NULL)
            free(*rez);
        (*rez) = strdup(arb->nume);
    }
    nr = 0;
    //o sa verificam toti subarbori ce au ca radacina copiii nodului curent
    verificare_existenta_in_subarbore(arb->st, nume_cod, nr_cod,rez);
    verificare_existenta_in_subarbore(arb->dr, nume_cod, nr_cod,rez);
    return ;
}

void cerinta4(FILE *in, Tarbore *arb, FILE * out) {
    int nr_cod;
    fscanf(in, "%d", &nr_cod);
    char **nume_cod = (char **)malloc(nr_cod * sizeof(char*));
    if (nume_cod == NULL) return;
    int lg = strlen(arb->nume);
    //lungimea maxima a unui nume a unui nod
    for (int i = 0; i < nr_cod; i++) {
        nume_cod[i] = (char*)malloc(lg * sizeof(char));
        if (nume_cod[i] == NULL) {
            free(nume_cod);
            return ;
        }
        fscanf(in, "%s", nume_cod[i]);
    }
    char *rez = NULL;
    //verificam daca exista in subarbori
    verificare_existenta_in_subarbore(arb, nume_cod, nr_cod, &rez);
    fprintf(out, "%s", rez);
    free(rez);
    for (int i = 0; i < nr_cod; i++) {
        free(nume_cod[i]);
    }
    free(nume_cod);
}

void Distrugerearb(Tarbore * arb) {
    if (arb == NULL) return;
    Distrugerearb(arb->st);
    Distrugerearb(arb->dr);
    free(arb->nume);
    free(arb);
}

int main(int argc, char * argv[]) {
   char *input = argv[2];
   char *output = argv[3];
   char *cerinta = argv[1];
    FILE * in = fopen(input, "r");
    if (in == NULL)
    return 0;
    FILE * out = fopen(output, "wa");
    if (out == NULL)
    return 0;
    //task comun tuturor
    Tarbore  *arb = (cerinta1(in));
    if (arb == NULL) return 0;
    //task1
    if (strcmp(cerinta, "-c1") == 0)
    afisare_cerinta1(arb, out);
    //task2
    if (strcmp(cerinta, "-c2") == 0)
    cerinta2(in, arb ,out);
    //task3
    if (strcmp(cerinta, "-c3") == 0)
    cerinta3(in, arb, out);
    //task4
    if (strcmp(cerinta, "-c4") == 0)
    cerinta4(in, arb, out);
    //DistrugeHeap(&h);
    Distrugerearb(arb);
    //eliberare memorie arbore
    arb = NULL;
    fclose(in);
    fclose(out);
} 