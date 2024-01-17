#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_DIM 20

// Definizione auto
typedef struct car_node {
    int autonomia;
    struct car_node *left;
    struct car_node *right;
    struct car_node *p;
} car_node_t;

typedef car_node_t *Car_node;

typedef struct car_bst {
    struct car_node *root;
} car_bst_t;

typedef car_bst_t *Car_bst;

// Definizione BST
typedef struct staz_node {
    int distanza;
    int num_auto;
    struct car_bst *cars;
    int max_auto;
    struct staz_node *left;
    struct staz_node *right;
    struct staz_node *p;
} staz_node_t;

typedef staz_node_t *Staz_node;

typedef struct staz_bst {
    struct staz_node *root;
} staz_bst_t;

typedef staz_bst_t *Staz_bst;

typedef struct list_node {
    int dist;
    int max_auto;
    struct list_node *next;
    struct list_node *prev;
} list_node_t;

typedef list_node_t *list;

// Definizione struttura ausiliaria
typedef struct {
    int val;
    struct list_node *l;
} aux_t;

/* Definizione funzioni */
/* Lista */
list list_insert(list l, int dist, int max_auto) {
    list new_node = NULL;
    new_node = malloc(sizeof(list_node_t));
    if (new_node != NULL) {
        new_node->dist = dist;
        new_node->max_auto = max_auto;
        new_node->prev = NULL;
        if (l == NULL) {
            new_node->next = NULL;
        } else {
            new_node->next = l;
            l->prev = new_node;
        }
        l = new_node;
    } else
        printf("Errore allocazione\n");

    return l;
}

/* Auto */
// Ritorna l'auto con autonomia massima
Car_node car_maximum(Car_node c) {
    while (c->right != NULL)
        c = c->right;
    return c;
}

// Ritorna l'auto con autonomia minima
Car_node car_minimum(Car_node c) {
    while (c->left != NULL)
        c = c->left;
    return c;
}

// Inserisce un'auto
void car_insert(Car_bst cars, Car_node c) {
    Car_node y = NULL;
    Car_node tmp = NULL;
    tmp = cars->root;
    while (tmp != NULL) {
        y = tmp;
        if (c->autonomia < tmp->autonomia)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    c->p = y;
    if (y == NULL) {
        cars->root = c;
    } else if (c->autonomia < y->autonomia)
        y->left = c;
    else
        y->right = c;
}

void cars_transplant(Car_bst cars, Car_node u, Car_node v) {
    if (u->p == NULL)
        cars->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;

    if (v != NULL)
        v->p = u->p;
}

// Elimina un'auto
void car_delete(Car_bst cars, Car_node to_del) {
    Car_node y = NULL;

    if (to_del->left == NULL)
        cars_transplant(cars, to_del, to_del->right);
    else if (to_del->right == NULL)
        cars_transplant(cars, to_del, to_del->left);
    else {
        y = car_minimum(to_del->right);
        if (y->p != to_del) {
            cars_transplant(cars, y, y->right);
            y->right = to_del->right;
            y->right->p = y;
        }
        cars_transplant(cars, to_del, y);
        y->left = to_del->left;
        y->left->p = y;
    }
}

// Ricerca un'auto
Car_node car_search(Car_node car, int autonomia) {
    if (car == NULL || autonomia == car->autonomia)
        return car;
    if (autonomia < car->autonomia)
        return car_search(car->left, autonomia);
    else
        return car_search(car->right, autonomia);
}

/* Stazioni */
// Ritorna la stazione a distanza minore
Staz_node staz_minimum(Staz_node s) {
    while (s->left != NULL)
        s = s->left;
    return s;
}

// Ricerca una stazione
Staz_node staz_search(Staz_node s, int distanza) {
    if (s == NULL || distanza == s->distanza)
        return s;
    if (distanza < s->distanza)
        return staz_search(s->left, distanza);
    else
        return staz_search(s->right, distanza);
}

// Inserisci una stazione
void staz_insert(Staz_bst staz, Staz_node s, int *num_staz) {
    Staz_node y = NULL;
    Staz_node tmp = NULL;
    tmp = staz->root;
    while (tmp != NULL) {
        y = tmp;
        if (s->distanza == tmp->distanza) {
            printf("non aggiunta\n");
            return;
        }
        if (s->distanza < tmp->distanza)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    s->p = y;
    if (y == NULL) {
        staz->root = s;
    } else if (s->distanza < y->distanza)
        y->left = s;
    else
        y->right = s;

    printf("aggiunta\n");
    *num_staz = *num_staz + 1;
}

void staz_transplant(Staz_bst staz, Staz_node u, Staz_node v) {
    if (u->p == NULL)
        staz->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;

    if (v != NULL)
        v->p = u->p;
}

// Elimina una stazione
void staz_delete(Staz_bst staz, Staz_node to_del) {
    Staz_node y = NULL;

    if (to_del->left == NULL)
        staz_transplant(staz, to_del, to_del->right);
    else if (to_del->right == NULL)
        staz_transplant(staz, to_del, to_del->left);
    else {
        y = staz_minimum(to_del->right);
        if (y->p != to_del) {
            staz_transplant(staz, y, y->right);
            y->right = to_del->right;
            y->right->p = y;
        }
        staz_transplant(staz, to_del, y);
        y->left = to_del->left;
        y->left->p = y;
    }
}

// Converte l'albero delle stazioni in una lista ordinata crescente
list staz_to_sorted_list(Staz_node staz, list l, int start, int end) {
    if (staz == NULL)
        return l;
    l = staz_to_sorted_list(staz->right, l, start, end);
    if (staz->distanza >= start && staz->distanza <= end) {
        l = list_insert(l, staz->distanza, staz->max_auto);
    }
    return staz_to_sorted_list(staz->left, l, start, end);
}

// Converte l'albero delle stazioni in una lista ordinata decrescente
list staz_to_reverse_sorted_list(Staz_node staz, list l, int start, int end) {
    if (staz == NULL)
        return l;
    l = staz_to_reverse_sorted_list(staz->left, l, start, end);
    if (staz->distanza >= end && staz->distanza <= start) {
        l = list_insert(l, staz->distanza, staz->max_auto);
    }
    return staz_to_reverse_sorted_list(staz->right, l, start, end);
}

// Funzioni relative al progetto
void aggiungi_stazione(Staz_bst staz, int *num_staz) {
    Staz_node new_staz = NULL;
    Car_node car = NULL;
    int distanza, num_auto = 0;

    if (scanf("%d", &distanza) != EOF) {}    // Lettura distanza
    if (scanf("%d", &num_auto) != EOF) {}   // Lettura numero auto

    // Creazione stazione
    new_staz = malloc(sizeof(staz_node_t));
    if (new_staz != NULL) {
        new_staz->distanza = distanza;
        new_staz->num_auto = num_auto;
        new_staz->max_auto = 0;
        new_staz->cars = malloc(sizeof(car_bst_t));
        if (new_staz->cars != NULL) {
            new_staz->cars->root = NULL;
            for (int i = 0; i < new_staz->num_auto; i++) {
                car = malloc(sizeof(car_node_t));
                if (car != NULL) {
                    if (scanf("%d", &car->autonomia) != EOF) {}

                    if (new_staz->max_auto < car->autonomia)
                        new_staz->max_auto = car->autonomia;

                    car_insert(new_staz->cars, car);
                } else
                    printf("Errore allocazione\n");
                car = NULL;
            }
        } else
            printf("Errore allocazione\n");

    } else
        printf("Errore allocazione\n");

    // Aggiunta stazione al BST
    staz_insert(staz, new_staz, num_staz);
}

void demolisci_stazione(Staz_bst staz, int *num_staz) {
    Staz_node staz_to_del = NULL;
    int distanza = 0;

    if (scanf("%d", &distanza) != EOF) {} // Lettura stazione da eliminare

    staz_to_del = staz_search(staz->root, distanza); // Ricerca della stazione da eliminare
    if (staz_to_del != NULL) {
        staz_delete(staz, staz_to_del); // Eliminazione
        printf("demolita\n");
        *num_staz = *num_staz - 1;
    } else
        printf("non demolita\n");
}

void aggiungi_auto(Staz_bst staz) {
    Staz_node curr_staz = NULL;
    Car_node new_car = NULL;
    int dist_staz, autonomia = 0;

    if (scanf("%d", &dist_staz) != EOF) {} // Lettura stazione in cui aggiungere l'auto
    if (scanf("%d", &autonomia) != EOF) {} // Lettura autonomia della nuova auto

    // Ricerca e aggiunta
    curr_staz = staz_search(staz->root, dist_staz);
    if (curr_staz != NULL) {
        new_car = malloc(sizeof(car_node_t));
        if (new_car != NULL) {
            new_car->autonomia = autonomia;
            car_insert(curr_staz->cars, new_car); // Inserimento nell'albero delle auto
            if (curr_staz->max_auto < new_car->autonomia) { // Update del massimo
                curr_staz->max_auto = new_car->autonomia;
            }
            curr_staz->num_auto = curr_staz->num_auto + 1;

            printf("aggiunta\n");
        } else
            printf("Errore allocazione\n");
    } else
        printf("non aggiunta\n");
}

void rottama_auto(Staz_bst staz) {
    Staz_node curr_staz = NULL;
    Car_node car_to_del = NULL;
    int dist_staz, autonomia = 0;

    if (scanf("%d", &dist_staz) != EOF) {} // Lettura stazione in cui aggiungere l'auto
    if (scanf("%d", &autonomia) != EOF) {} // Lettura autonomia dell'auto da rottamare

    // Ricerca ed eliminazione
    curr_staz = staz_search(staz->root, dist_staz);
    if (curr_staz != NULL) { // Se trova la stazione..
        if (curr_staz->num_auto > 0) { // e la stazione ha almeno un'auto...
            car_to_del = car_search(curr_staz->cars->root, autonomia); // Ricerco auto
            if (car_to_del != NULL) {
                car_delete(curr_staz->cars, car_to_del); // Elimino l'auto

                if (curr_staz->cars->root != NULL) {
                    curr_staz->max_auto = car_maximum(curr_staz->cars->root)->autonomia;
                    curr_staz->num_auto = curr_staz->num_auto - 1;
                } else {
                    curr_staz->max_auto = 0;
                    curr_staz->num_auto = 0;
                }

                printf("rottamata\n");
            } else
                printf("non rottamata\n");
        } else
            printf("non rottamata\n");
    } else
        printf("non rottamata\n");
}

// Pianificazione percorso: crescente
int pianifica_percorso_ric(list start, list end) {
    list tmp = start;

    // Scorro la lista fino ad avere il primo nodo che arriva a destinazione
    while (tmp != end && abs(tmp->dist - end->dist) > tmp->max_auto) {
        tmp = tmp->next;
    }

    // Se tmp rimane start, vuol dire che ho percorso diretto
    if (tmp == start) {
        printf("%d %d", tmp->dist, end->dist);
        return 1;
    }

    // Se invece tmp arriva a destinazione, non esiste un percorso
    if (tmp == end) {
        printf("nessun percorso");
        return 0;
    }

    // Altrimenti, ripete ricorsivamente a tmp come destinazione
    if (pianifica_percorso_ric(start, tmp) == 1) {
        printf(" %d", end->dist);
        return 1;
    }

    return 0;
}

aux_t pianifica_percorso_dec_ric(list start, list end, list tmp, aux_t ret) {
    // Se si può raggiungere direttamente, salvo partenza e destinazione
    if (abs(start->dist - end->dist) <= start->max_auto) {
        ret.l = list_insert(ret.l, end->dist, 0);
        ret.l = list_insert(ret.l, start->dist, 0);
        ret.val = 1;
        return ret;
    }

    // Altrimenti calcolo la stazione che mi porta più lontano a partire dalla partenza
    list min = start->next;
    list i = start->next;
    list j = NULL;
    int min_last = min->dist;
    while (i != end && abs(start->dist - i->dist) <= start->max_auto) {
        j = i;
        while (j != NULL && abs(i->dist - j->dist) <= i->max_auto) {
            j = j->next;
        }
        if (j == NULL) {
            min_last = end->dist;
            min = i;
        } else {
            j = j->prev;
            if (j->dist <= min_last) {
                min_last = j->dist;
                min = i;
            }
        }

        i = i->next;
    }

    if (min == start->next && i == start->next) { // Vuol dire che il percorso non esiste
        printf("nessun percorso");
        ret.val = 0;
        return ret;
    } else {
        // Chiamata ricorsiva
        ret = pianifica_percorso_dec_ric(min, end, i->prev, ret);
        if (ret.val == 1) {
            // Fase di aggiustamento del percorso: scelta delle tappe più vicine all'inizio
            list k = tmp;

            // Se l'ultima tappa raggiungibili non è quella che porta più lontano
            if (tmp != min && tmp != NULL) {
                // Controllo se ci sono tappe più vicine all'inizio che mi portano alla medesima stazione...
                while (k != start && k->max_auto < abs(ret.l->dist - k->dist)) {
                    k = k->prev;
                }

                // ...e la salvo
                ret.l = list_insert(ret.l, k->dist, 0);
            } else {
                // altrimenti, salvo direttamento la partenza
                ret.l = list_insert(ret.l, start->dist, 0);
            }

            ret.val = 1;
            return ret;
        } else {
            ret.val = 0;
            return ret;
        }
    }
}

void print_perc(list l) {
    while (l != NULL && l->next != NULL) {
        printf("%d ", l->dist);
        l = l->next;
    }
    if (l != NULL)
        printf("%d", l->dist);
}

void pianifica_percorso(Staz_bst staz) {
    int start_staz = 0;
    int end_staz = 0;
    list l = NULL;
    list last = NULL;
    aux_t ret;

    if (scanf("%d", &start_staz) != EOF) {} // Lettura stazione di partenza
    if (scanf("%d", &end_staz) != EOF) {}// Lettura stazione di arrivo

    // Scelta fra i due pianifica percorso da utilizzare
    if (start_staz <= end_staz) {
        // Lista delle stazioni comprese tra partenza e destinazione
        l = staz_to_sorted_list(staz->root, l, start_staz, end_staz);

        // Calcolo nodo stazione destinazione
        last = l;
        while (last != NULL && last->next != NULL){
            last = last->next;
        }

        // Pianificazione percorso in senso crescente di stazioni
        pianifica_percorso_ric(l, last);
    } else {
        // Lista delle stazioni comprese tra partenza e destinazione
        l = staz_to_reverse_sorted_list(staz->root, l, start_staz, end_staz);

        // Calcolo nodo stazione destinazione
        last = l;
        while (last != NULL && last->next != NULL){
            last = last->next;
        }

        // Controllo se la destinazione è raggiunta da qualche stazione, altrimenti è inutile calcolare il percorso
        list tmp;
        for (tmp = l; tmp != NULL && (tmp->max_auto < abs(tmp->dist - last->dist)); tmp = tmp->next);

        // Se non esistono, cioè la destinazione arriva a se stessa, non esiste il percorso
        if (tmp == last) {
            printf("nessun percorso");
        } else {
            // Se esiste, devo calcolarlo
            ret.l = NULL;

            // Pianificazione percorso in senso decrescente di stazioni
            ret = pianifica_percorso_dec_ric(l, last, NULL, ret);

            // Stampa percorso
            if (ret.val == 1)
                print_perc(ret.l);
        }
    }
    printf("\n");
}

int main() {
    char command_type[CMD_DIM];
    Staz_bst staz = NULL;
    int num_staz = 0; // Tengo traccia del numero di stazioni esistenti

    staz = malloc(sizeof(staz_bst_t));
    if (staz != NULL) {
        staz->root = NULL;

        // Lettura comandi
        while (scanf("%s", command_type) != EOF) {
            if (strcmp(command_type, "aggiungi-stazione") == 0) {
                aggiungi_stazione(staz, &num_staz);

            } else if (strcmp(command_type, "demolisci-stazione") == 0) {
                demolisci_stazione(staz, &num_staz);

            } else if (strcmp(command_type, "aggiungi-auto") == 0) {
                aggiungi_auto(staz);

            } else if (strcmp(command_type, "rottama-auto") == 0) {
                rottama_auto(staz);

            } else if (strcmp(command_type, "pianifica-percorso") == 0) {
                pianifica_percorso(staz);
            }
        }
    } else
        printf("Errore allocazione\n");
    return 0;
}