/*DRAGAN Marius*/
#include "tema2.h"

/*Allocates memory for the Min Heap structure*/
THeap *alloc_heap(int max_size, TFCmp comp)
{
	THeap *h = (THeap *)malloc(sizeof(struct Heap));
	if (!h)
	{
		return NULL;
	}

	h->v = (TSatellite *)malloc(max_size * sizeof(TSatellite));
	if (!h->v)
	{
		return NULL;
	}

	h->max_size = max_size;
	h->current_size = 0;
	h->comp = comp;

	return h;
}

/*Allocates and initializes a new binary tree node*/
Ttree *create_nod(int freq, char *str, Ttree *left, Ttree *right)
{
	Ttree *aux = (Ttree *)malloc(sizeof(Ttree));
	if (aux == NULL)
		return NULL;
	aux->freq = freq;
	aux->name = strdup(str);
	aux->right = right;
	aux->left = left;
	return aux;
}

/*Comparator for the Min Heap: sorts by frequency, then lexicographically*/
int RelMinHeap(TSatellite a, TSatellite b)
{
	if (a.freq != b.freq)
		return a.freq < b.freq;
	return strcmp(a.name, b.name) < 0;
}

/*Recursively displays the heap*/
void displayHeap(THeap *h, int pos, FILE *out)
{
	if (pos >= h->current_size)
	{
		fprintf(out, "-");
		return;
	}

	fprintf(out, " %d<->%s ", h->v[pos].freq, h->v[pos].name);

	if (2 * pos + 1 >= h->max_size && 2 * pos + 2 >= h->max_size)
		return;

	fprintf(out, "(");

	displayHeap(h, 2 * pos + 1, out);
	fprintf(out, ",");

	displayHeap(h, 2 * pos + 2, out);
	fprintf(out, ")");
}

/*Frees all memory allocated for the heap and its elements*/
void destroyHeap(THeap **h)
{
	// eliberare heap
	for (int i = 0; i < (*(h))->current_size; i++)
		free((*(h))->v[i].name);
	free((*h)->v);
	free(*h);
	*h = NULL;
}

/*Restores the Min Heap properly by bubbling up the element at 'pos*/
void pushUP(THeap *h, int pos)
{
	int indice_tata = (pos - 1) / 2;
	while (pos > 0 && !h->comp(h->v[indice_tata], h->v[pos]))
	{
		TSatellite aux = h->v[pos];
		h->v[pos] = h->v[indice_tata];
		h->v[indice_tata] = aux;
		pos = indice_tata;
		indice_tata = (pos - 1) / 2;
	}
}

/*Inserts a new satellite node into the Min Heap*/
void InsertHeap(THeap *h, int freq, char *str, Ttree *leftChild, Ttree *rightChild)
{
	h->v[h->current_size].freq = freq;
	h->v[h->current_size].name = strdup(str);
	h->v[h->current_size].node = create_nod(freq, str, leftChild, rightChild);
	h->current_size++;
	h->max_size++;
	pushUP(h, h->current_size - 1);
}

/*Restore the Min-Heap properly by bubbling down the element at 'pos'*/
void pushDown(int pos, THeap *h)
{
	int leftChildIdx = 2 * pos + 1;
	while (leftChildIdx < h->current_size)
	{
		int s = leftChildIdx;
		int rightChildIdx = 2 * pos + 2;
		if (rightChildIdx < h->current_size)
		{
			if (h->v[s].freq != h->v[rightChildIdx].freq)
			{
				if (h->v[s].freq > h->v[rightChildIdx].freq)
				{
					s = rightChildIdx;
				}
			}
			else
			{
				if (strcmp(h->v[s].name, h->v[rightChildIdx].name) > 0)
				{
					s = rightChildIdx;
				}
			}
		}
		if (h->v[pos].freq < h->v[s].freq)
			break;
		else if (h->v[pos].freq == h->v[s].freq && strcmp(h->v[pos].name, h->v[s].name) < 0)
		{
			break;
		}
		else
		{
			TSatellite aux = h->v[pos];
			h->v[pos] = h->v[s];
			h->v[s] = aux;
			pos = s;
			leftChildIdx = 2 * pos + 1;
		}
	}
}

/*Extract the minimul element from the Min Heap*/
TSatellite ExtrHeap(THeap *h)
{
	h->current_size--;
	TSatellite aux = h->v[0];
	h->v[0] = h->v[h->current_size];
	h->max_size--;
	pushDown(0, h);
	return aux;
}

/*Build the Huffman-style tree from input data*/
Ttree *build_huffmann_tree(FILE *in)
{
	int n;
	fscanf(in, "%d", &n);
	TSatellite *v = malloc(sizeof(TSatellite) * n);
	if (v == NULL)
		return NULL;
	THeap *h = NULL;
	/*Read initial sattelite nodes*/
	for (int i = 0; i < n; i++)
	{
		fscanf(in, "%d", &v[i].freq);
		char str[15];
		fscanf(in, "%s", str);
		v[i].name = strdup(str);
		if (v[i].name == NULL)
		{
			for (int j = 0; j < i; j++)
				free(v[j].name);
			free(v);
			return NULL;
		}
		v[i].node = create_nod(v[i].freq, v[i].name, NULL, NULL);
		if (v[i].node == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(v[j].node->name);
				free(v[j].node);
			}
			for (int j = 0; j < n; j++)
			{
				free(v[j].name);
				free(v);
				return NULL;
			}
		}
	}
	h = alloc_heap(n, RelMinHeap);
	if (h == NULL)
	{
		for (int i = 0; i < n; i++)
		{
			free(v[i].name);
			free(v[i].node->name);
			free(v[i].node);
		}
	}
	/*Populate the Min Heap*/
	for (int i = 0; i < n; i++)
	{
		InsertHeap(h, v[i].freq, v[i].name, v[i].node->left, v[i].node->right);
	}
	/*Build the tree by extracting the two minimums and merging them*/
	while (h->current_size > 1)
	{
		TSatellite aux_1 = ExtrHeap(h);
		TSatellite aux_2 = ExtrHeap(h);
		if (aux_1.freq > aux_2.freq)
		{
			TSatellite aux = aux_1;
			aux_1 = aux_2;
			aux_2 = aux;
		}
		else if (aux_1.freq == aux_2.freq && strcmp(aux_1.name, aux_2.name) > 0)
		{
			TSatellite aux = aux_1;
			aux_1 = aux_2;
			aux_2 = aux;
		}
		char *str = malloc(strlen(aux_1.name) + strlen(aux_2.name) + 1);
		strcpy(str, aux_1.name);
		strcat(str, aux_2.name);
		int s = aux_1.freq;
		s = s + aux_2.freq;
		InsertHeap(h, s, str, aux_1.node, aux_2.node);
		free(str);
		free(aux_1.name);
		free(aux_2.name);
		;
	}
	/*Clean up temporary arrays*/
	for (int i = 0; i < n; i++)
	{
		free(v[i].name);
		free(v[i].node->name);
		free(v[i].node);
	}
	free(v);
	Ttree *aux;
	aux = h->v[h->current_size - 1].node;
	destroyHeap(&h);
	return aux;
}

/*Initializes an empty Queue for BFS traversal*/
TQueue *InitQ()
{
	TQueue *c;
	c = (TQueue *)malloc(sizeof(TQueue));
	if (!c)
		return NULL;
	c->head = NULL;
	c->tail = NULL;
	return c;
}

/*Enqueues a tree node*/
int enqueue(TQueue *c, Ttree *node)
{
	TList aux;
	aux = (TList)malloc(sizeof(TCell));
	if (!aux)
		return 0;
	aux->node = node;
	aux->next = NULL;
	if (c->tail == NULL)
	{
		c->head = aux;
		c->tail = aux;
	}
	else
	{
		c->tail->next = aux;
		c->tail = aux;
	}
	return 1;
}

/*Dequeues and returns a tree node*/
Ttree *dequeue(TQueue *c)
{
	TList aux;
	Ttree *aux_ret;
	aux = c->head;
	aux_ret = aux->node;
	aux = c->head;
	c->head = aux->next;
	if (c->head == NULL)
		c->tail = NULL;
	free(aux);
	return aux_ret;
}

/*Perfoms a level-order travers(BFS) and prints the network topology*/
void printTreeLevelOrder(Ttree *tree, FILE *out)
{
	TQueue *c = InitQ();
	if (c == NULL)
		return;
	enqueue(c, tree);
	int z = 1; /*Nodes on the current level*/
	while ((c->head) != NULL)
	{
		int i, j;
		j = 0;
		for (i = 0; i < z; i++)
		{
			Ttree *aux = dequeue(c);
			fprintf(out, "%d-%s", aux->freq, aux->name);
			if (i != (z - 1))
				fprintf(out, " ");
			if (aux->left != NULL)
			{
				enqueue(c, aux->left);
				j++;
			}
			if (aux->right != NULL)
			{
				enqueue(c, aux->right);
				j++;
			}
		}
		fprintf(out, "\n");
		z = j;
		/*Nodes on the next level*/
	}
	free(c);
	c = NULL;
}

/*Decodes a binary sequence(0=left 1=right) into satellite names*/
void decodeSattelitePaths(FILE *in, Ttree *tree, FILE *out)
{
	int numCodes;
	fscanf(in, "%d", &numCodes);
	for (int i = 0; i < numCodes; i++)
	{
		char str[1000];
		fscanf(in, "%s", str);
		Ttree *pre, *p = tree;
		for (int j = 0; j < strlen(str); j++)
		{
			if (str[j] == '0')
			{
				pre = p;
				p = p->left;
			}
			if (str[j] == '1')
			{
				pre = p;
				p = p->right;
			}
			/*if a leaf is reached,print and reset to root*/
			if (p == NULL)
			{
				fprintf(out, "%s ", pre->name);
				p = tree;
				j--;
			}
		}
		if (p == NULL)
			fprintf(out, "%s ", pre->name);
		// se poate si fara else
		else if (p->left == NULL && p->right == NULL)
			fprintf(out, "%s", p->name);
		fprintf(out, "\n");
	}
}

/*DFS heklper to find the binary path to a target node*/
void traverseAndEncode(Ttree *tree, char *code_name, char result[1000], int nr, int *ok)
{
	if (tree == NULL || (*ok) == 1)
		return;
	if (strcmp(tree->name, code_name) == 0)
	{
		result[nr] = '\0';
		*ok = 1;
		return;
	}
	if (tree->left && (*ok) == 0)
	{
		result[nr] = '0';
		traverseAndEncode(tree->left, code_name, result, nr + 1, ok);
	}
	if (tree->right && (*ok) == 0)
	{
		result[nr] = '1';
		traverseAndEncode(tree->right, code_name, result, nr + 1, ok);
	}
	return;
}

/*Encodes satellite names into binary routing paths*/
void encodeSattelitePaths(FILE *in, Ttree *tree, FILE *out)
{
	int num_nodes;
	fscanf(in, "%d", &num_nodes);
	// numarul de noduri ce trebuiesc codificate
	int max_len = strlen(tree->name);
	char *final_encoding = malloc(1000 * num_nodes * sizeof(char));
	if (final_encoding == NULL)
		return;
	final_encoding[0] = '\0';
	for (int i = 0; i < num_nodes; i++)
	{
		char *name_nod = malloc(sizeof(char) * max_len);
		if (name_nod == NULL)
		{
			free(final_encoding);
			return;
		}
		fscanf(in, "%s", name_nod);
		char result[1000] = "";
		int ok = 0;
		traverseAndEncode(tree, name_nod, result, 0, &ok);
		strcat(final_encoding, result);
		free(name_nod);
	}
	fprintf(out, "%s", final_encoding);
	free(final_encoding);
}

/*Checks if a target node exists in the given subtree*/
void findInSubtree(Ttree *tree, int *ok, char *target)
{
	if (!tree)
		return;
	if (strcmp(tree->name, target) == 0)
	{
		(*ok) = 1;
		return;
	}
	findInSubtree(tree->left, ok, target);
	findInSubtree(tree->right, ok, target);
	return;
}

/*Helper for finding the Lowest Common Ancestor by validating node existence*/
void checkExistenceInSubtree(Ttree *tree, char **code_name, int count, char **res)
{
	int i;
	int ok;
	int found_count = 0;
	if (tree == NULL)
		return;
	for (i = 0; i < count; i++)
	{
		ok = 0;
		findInSubtree(tree, &ok, code_name[i]);
		if (ok == 1)
			found_count++;
	}
	if (found_count == count)
	{
		if (*res != NULL)
			free(*res);
		(*res) = strdup(tree->name);
	}
	found_count = 0;
	checkExistenceInSubtree(tree->left, code_name, count, res);
	checkExistenceInSubtree(tree->right, code_name, count, res);
	return;
}

/*Finds the Lowest Common Ancestor for a list of nodes*/
void findCommonAncestor(FILE *in, Ttree *tree, FILE *out)
{
	int count;
	fscanf(in, "%d", &count);
	char **code_name = (char **)malloc(count * sizeof(char *));
	if (code_name == NULL)
		return;
	int max_len = strlen(tree->name);
	for (int i = 0; i < count; i++)
	{
		code_name[i] = (char *)malloc(max_len * sizeof(char));
		if (code_name[i] == NULL)
		{
			free(code_name);
			return;
		}
		fscanf(in, "%s", code_name[i]);
	}
	char *res = NULL;
	checkExistenceInSubtree(tree, code_name, count, &res);
	fprintf(out, "%s", res);
	free(res);
	for (int i = 0; i < count; i++)
	{
		free(code_name[i]);
	}
	free(code_name);
}

/*Recursively frees the entire binary tree*/
void destroytree(Ttree *tree)
{
	if (tree == NULL)
		return;
	destroytree(tree->left);
	destroytree(tree->right);
	free(tree->name);
	free(tree);
}

int main(int argc, char *argv[])
{
	char *input = argv[2];
	char *output = argv[3];
	char *task = argv[1];
	FILE *in = fopen(input, "r");
	if (in == NULL)
		return 0;
	FILE *out = fopen(output, "wa");
	if (out == NULL)
		return 0;
	/*Core task: Build the network tree*/
	Ttree *tree = (build_huffmann_tree(in));
	if (tree == NULL)
		return 0;
	/*Execute requested task based on flag*/
	if (strcmp(task, "-c1") == 0)
		printTreeLevelOrder(tree, out);
	if (strcmp(task, "-c2") == 0)
		decodeSattelitePaths(in, tree, out);
	if (strcmp(task, "-c3") == 0)
		encodeSattelitePaths(in, tree, out);
	if (strcmp(task, "-c4") == 0)
		findCommonAncestor(in, tree, out);
	/*Clean up*/
	destroytree(tree);
	tree = NULL;
	fclose(in);
	fclose(out);
}