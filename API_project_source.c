//
// Created by Sara Leka on 17/07/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXCARS 512


/* Station is defined as the node of a binary search tree
 * For optimized results the BST used is going to be implemented as a Red-Black tree
 * */

struct station {
    int distance; //distance from the start of the highway (km 0)
    int num_cars; //maximum number of cars in the parking of the station (max 512)
    int max_autonomy; //maximum autonomy among the cars available in the station
    int *autonomies; //pointer to the array that containes the autonomies of the cars in the station
    char colour; //r for red, b for black
    struct station *left;
    struct station *right;
    struct station *father;
};

typedef struct station *nodeSTATION;


/* Highways is defined as a binary search tree whose nodes are stations
 * For optimized results the BST used is going to be implemented as a Red-Black tree
 * */

typedef struct {
    nodeSTATION root; //pointer to the root
    nodeSTATION Tnil; //pointer to the nil node
} highway;


/* Declaration of function used
 * */

void swap (int *a, int *b);
nodeSTATION find_station(highway *A, nodeSTATION x, int search);
void create_highway (highway *A);
void rotate_left (highway *A, nodeSTATION x);
void rotate_right (highway *A, nodeSTATION x);
void insert_in_highway_fixup (highway *A, nodeSTATION ins);
void insert_in_highway_RB (highway *A, nodeSTATION ins);
void add_station (highway *A, int dist, int num, int *autonomies_auto);
nodeSTATION min_station (highway *A, nodeSTATION x);
nodeSTATION max_station (highway *A, nodeSTATION x);
nodeSTATION next_station (highway *A, nodeSTATION x);
nodeSTATION previous_station (highway *A, nodeSTATION x);
void delete_from_highway_fixup (highway *A, nodeSTATION x);
void transplant_RB  (highway *A, nodeSTATION x, nodeSTATION y);
void delete_from_highway_RB (highway *A, nodeSTATION canc);
void delete_station (highway *A, int dist);
void add_car (highway *A, int dist, int km);
void delete_car (highway *A, int dist, int km);
void plan_route (highway *A, int a, int b);
void plan_direct_route (highway *A, int a, int b);
int max (int a, int b);
void plan_reverse_route (highway *A, int a, int b);


/* Main function
 * Read from stdin
 * Write in stdout
 * */

int main() {
    int distance = -1;
    int num_cars= -1;
    int autonomy = -1;
    int start = -1;
    int finish = -1;
    int autonomies[MAXCARS];
    char instruction [30];
    highway *A= (highway*) malloc(sizeof(highway));
    create_highway (A); //initialized the highway as a RB BST
    while(scanf("%s", instruction) != EOF) {
        if(strcmp(instruction, "aggiungi-stazione")==0) {//aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n
            if(scanf("%d %d", &distance, &num_cars) != EOF) {
                for (int i=0; i<num_cars; i++) {
                    if(scanf("%d ", &autonomies[i])==EOF)
                        break;
                }
                add_station(A, distance, num_cars, autonomies);
            }
        }

        if(strcmp(instruction, "demolisci-stazione")==0) {//demolisci-stazione distanza
            if(scanf("%d", &distance) != EOF) {
                delete_station(A, distance);
            }
        }

        if(strcmp(instruction, "aggiungi-auto")==0) {//aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
            if(scanf("%d %d", &distance, &autonomy) != EOF) {
                add_car(A, distance, autonomy);
            }
        }

        if(strcmp(instruction, "rottama-auto")==0) {//rottama-auto distanza-stazione autonomia-auto-da-rottamare
            if(scanf("%d %d", &distance, &autonomy) != EOF) {
                delete_car(A, distance, autonomy);
            }
        }

        if(strcmp(instruction, "pianifica-percorso")==0) { //pianifica-percorso stazione-partenza stazione-arrivo
            if(scanf("%d %d", &start, &finish) != EOF) {
                plan_route(A, start, finish);
            }
        }
    }
    return 0;
}


void swap (int *a, int *b) {
    int temp = *a;
    *a=*b;
    *b=temp;
    return;
}

nodeSTATION find_station(highway *A, nodeSTATION x, int search) {
    while (x!=A->Tnil && x->distance!=search) {
        if (search<x->distance)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

void create_highway (highway *A) {
    A->root = malloc(sizeof (struct station));
    A->Tnil = malloc(sizeof (struct station));
    A->Tnil->colour = 'b';
    A->Tnil->num_cars= -1;
    A->Tnil->distance = -1;
    A->Tnil->max_autonomy= -1;
    A->Tnil->autonomies= NULL;
    A->Tnil->father = A->Tnil;
    A->Tnil->left = A->Tnil;
    A->Tnil->right = A->Tnil;
    A->root = A->Tnil;
    return;
}

void rotate_left (highway *A, nodeSTATION x) {
    nodeSTATION y = x->right;
    x->right = y->left;
    if(y->left != A->Tnil)
        y->left->father = x;
    y->father = x->father;
    if (x->father == A->Tnil)
        A->root = y;
    else if (x == x->father->left)
        x->father->left=y;
    else
        x->father->right=y;
    y->left = x;
    x->father = y;
    return;
}

void rotate_right (highway *A, nodeSTATION x) {
    nodeSTATION y = x->left;
    x->left = y->right;
    if(y->right != A->Tnil)
        y->right->father = x;
    y->father = x->father;
    if (x->father == A->Tnil)
        A->root = y;
    else if (x == x->father->left)
        x->father->left=y;
    else
        x->father->right=y;
    y->right = x;
    x->father = y;
    return;
}

void insert_in_highway_fixup (highway *A, nodeSTATION ins) {
    while (ins->father->colour == 'r') {
        nodeSTATION y;
        if(ins->father == ins->father->father->left) {
            y = ins->father->father->right;
            if (y->colour == 'r') {
                ins->father->colour = 'b';
                y->colour = 'b';
                ins->father->father->colour = 'r';
                ins = ins->father->father;
            }
            else {
                if (ins == ins->father->right) {
                    ins = ins->father;
                    rotate_left (A, ins);
                }
                ins->father->colour = 'b';
                ins->father->father->colour = 'r';
                rotate_right (A, ins->father->father);
            }
        }
        else { //specular as before, left swapped with right
            y = ins->father->father->left;
            if (y->colour == 'r') {
                ins->father->colour = 'b';
                y->colour = 'b';
                ins->father->father->colour = 'r';
                ins = ins->father->father;
            }
            else {
                if (ins == ins->father->left) {
                    ins = ins->father;
                    rotate_right (A, ins);
                }
                ins->father->colour = 'b';
                ins->father->father->colour = 'r';
                rotate_left (A, ins->father->father);
            }
        }
    }
    A->root->colour = 'b';
    return;
}

void insert_in_highway_RB (highway *A, nodeSTATION ins) {
    nodeSTATION y = A->Tnil;
    nodeSTATION x = A->root;
    while (x != A->Tnil) {
        y=x;
        if (ins->distance < x->distance)
            x=x->left;
        else
            x=x->right;
    }
    ins->father = y;
    if (y==A->Tnil)
        A->root = ins;
    else if (ins->distance < y->distance)
        y->left = ins;
    else
        y->right = ins;
    ins->left = A->Tnil;
    ins->right = A->Tnil;
    ins->colour = 'r';
    insert_in_highway_fixup (A, ins);
    return;
}

void add_station (highway *A, int dist, int num, int *autonomies_auto) {
    if(find_station(A, A->root, dist)==A->Tnil) { //if there is already a station in the highway at the specified distance, it's not possibile to add another one
        nodeSTATION new = malloc(sizeof(struct station));
        new->distance = dist;
        new->num_cars= num;
        new->autonomies= (int*) malloc(MAXCARS * sizeof(int));
        for (int i=0; i<new->num_cars; i++) {
            new->autonomies[i] = autonomies_auto[i];
        }
        new->max_autonomy= 0;
        for (int i=0; i<new->num_cars; i++) {
            if(new->autonomies[i] > new->max_autonomy)
                new->max_autonomy= new->autonomies[i];
        }
        new->left = A->Tnil;
        new->right = A->Tnil;
        new->father = A->Tnil;
        insert_in_highway_RB(A, new);
        printf("aggiunta\n");
    }
    else
        printf("non aggiunta\n");
    return;
}

nodeSTATION min_station(highway *A, nodeSTATION x) {
    while (x->left != A->Tnil)
        x = x->left;
    return x;
}

nodeSTATION max_station(highway *A, nodeSTATION x) {
    while (x->right != A->Tnil)
        x = x->right;
    return x;
}

nodeSTATION next_station (highway *A, nodeSTATION x) {
    if (x->right !=A->Tnil)
        return min_station(A, x->right);
    nodeSTATION y = x->father;
    while(y!=A->Tnil && x==y->right) {
        x = y;
        y = y->father;
    }
    return y;
}

nodeSTATION previous_station (highway *A, nodeSTATION x) {
    if (x->left !=A->Tnil)
        return max_station(A, x->left);
    nodeSTATION y = x->father;
    while(y!=A->Tnil && x==y->left) {
        x = y;
        y = y->father;
    }
    return y;
}

void delete_from_highway_fixup (highway *A, nodeSTATION x) {
    while (x!=A->root && x->colour=='b') {
        nodeSTATION w;
        if (x == x->father->left) {
            w = x->father->right;
            if(w->colour =='r') {
                w->colour = 'b';
                x->father->colour = 'r';
                rotate_left(A, x->father);
                w=x->father->right;
            }
            if(w->left->colour == 'b' && w->right->colour == 'b') {
                w->colour = 'r';
                x = x->father;
            } else {
                if(w->right->colour == 'b') {
                    w->left->colour = 'b';
                    w->colour = 'r';
                    rotate_right(A, w);
                    w = x->father->right;
                }
                w->colour = x->father->colour;
                x->father->colour = 'b';
                w->right->colour = 'b';
                rotate_left(A, x->father);
                x = A->root;
            }
        }
        else {
            w = x->father->left;
            if (w->colour == 'r') {
                w->colour = 'b';
                x->father->colour = 'r';
                rotate_right(A, x->father);
                w=x->father->left;
            }
            if(w->left->colour == 'b' && w->right->colour == 'b') {
                w->colour = 'r';
                x = x->father;
            } else {
                if(w->left->colour == 'b') {
                    w->right->colour = 'b';
                    w->colour = 'r';
                    rotate_left(A, w);
                    w = x->father->left;
                }
                w->colour = x->father->colour;
                x->father->colour = 'b';
                w->left->colour = 'b';
                rotate_right(A, x->father);
                x = A->root;
            }
        }
    }
    x->colour = 'b';
    return;
}

void transplant_RB  (highway *A, nodeSTATION x, nodeSTATION y) {
    if (x->father == A->Tnil)
        A->root = y;
    else if (x == x->father->left)
        x->father->left = y;
    else
        x->father->right = y;
    y->father = x->father;
    return;
}

void delete_from_highway_RB (highway *A, nodeSTATION canc) {
    nodeSTATION x;
    nodeSTATION y = canc;
    char y_og = y->colour;
    if (canc->left == A->Tnil) {
        x = canc->right;
        transplant_RB (A, canc, canc->right);
    }
    else if (canc->right == A->Tnil) {
        x = canc->left;
        transplant_RB (A, canc, canc->left);
    }
    else {
        y = next_station (A, canc);
        y_og = y->colour;
        x = y->right;
        if (y->father == canc)
            x->father = y;
        else {
            transplant_RB (A, y, y->right);
            y->right = canc->right;
            y->right->father = y;
        }
        transplant_RB (A, canc, y);
        y->left= canc->left;
        y->left->father = y;
        y->colour = canc->colour;
    }
    if (y_og == 'b')
        delete_from_highway_fixup (A, x);
    free(canc->autonomies);
    free(canc);
    return;
}

void delete_station (highway *A, int dist) {
    nodeSTATION canc = find_station(A, A->root, dist);
    if(canc==A->Tnil) //if there isn't a station at the specified distance, it's not possible to delete it
        printf("non demolita\n");
    else {
        delete_from_highway_RB (A, canc);
        printf("demolita\n");
    }
    return;
}

void add_car (highway *A, int dist, int km) {
    nodeSTATION x = find_station(A, A->root, dist);
    if (x==A->Tnil || x->num_cars==MAXCARS) //if there isn't enough space in the parking space at the specified distance, it's not possible to add a new car
        printf("non aggiunta\n");
    else {
        x->num_cars++;
        x->autonomies[x->num_cars-1] = km;
        if (km > x->max_autonomy)
            x->max_autonomy= km;
        printf("aggiunta\n");
    }
    return;
}

void delete_car (highway *A, int dist, int km) {
    bool trovato = false;
    nodeSTATION x = find_station(A, A->root, dist);
    if (x==A->Tnil || x->num_cars==0)
        printf("non rottamata\n");
    else {
        int i;
        for(i=0; i<x->num_cars;i++) {
            if(x->autonomies[i]==km) {
                swap(&x->autonomies[i], &x->autonomies[x->num_cars-1]);
                trovato = true;
                break;
            }
        }
        if (trovato) {
            x->num_cars--;
            if (x->max_autonomy== km) {
                x->max_autonomy= 0;
                for(i=0; i<x->num_cars;i++) {
                    if(x->autonomies[i]==km)
                        break;
                    if(x->autonomies[i] > x->max_autonomy)
                        x->max_autonomy= x->autonomies[i];
                }
            }
            printf("rottamata\n");
        } else
            printf("non rottamata\n");
    }
    return;
}

int max (int a, int b) {
    if(a>b)
        return a;
    else
        return b;
}

void plan_route (highway *A, int a, int b) {
    if (a==b) //if start is equal to finish
        printf("%d\n", a);
    if (a<b) //plan_direct_route
        plan_direct_route (A, a, b);
    if (a>b) //plane_reverse_route
        plan_reverse_route (A, a, b);
    return;
}

void plan_direct_route (highway *A, int a, int b) {
    int n = 100;
    int *percorso = (int*)malloc(n * sizeof(int));
    int tappe = 1;
    percorso[tappe-1] = a;
    nodeSTATION start = find_station (A, A->root, a);
    nodeSTATION destinazione = find_station (A, A->root, b);
    nodeSTATION y = next_station (A, start);
    nodeSTATION curr = start;
    nodeSTATION next = curr;
    while(1) {
        while ((curr->distance + curr->max_autonomy>= y->distance) && (y != A->Tnil)) { //look at the reachable stations from curr
            if ((next->distance + next->max_autonomy< y->distance + y->max_autonomy) && y!=destinazione) {
                next = y;
            }
            if (y == destinazione) {
                next = y;
                tappe++;
                percorso[tappe-1] = destinazione->distance;
                break;
            }
            y = next_station (A, y);
        }
        if(curr == next) { //if there are no reachable stations from curr
            free(percorso);
            printf("nessun percorso\n");
            return;
        }
        if(next == destinazione) {
            break;
        }
        curr = next;
        tappe++;
        if(tappe+1>n) { //dimensioning of the variable length array if it's too short
            n = 2*n;
            percorso = (int*)realloc(percorso, n*sizeof (int));
        }
        percorso[tappe-1] = curr->distance;
    }
    int i;
    if(tappe>2) {
        nodeSTATION prec_tappa;
        nodeSTATION next_tappa;
        for (i = tappe - 2; i > 0; i--) {
            prec_tappa = find_station(A, A->root, percorso[i - 1]);
            next_tappa = find_station(A, A->root, percorso[i+1]);
            next = previous_station (A, curr);
            while (next != prec_tappa) {
                if (next->distance + next->max_autonomy>= next_tappa->distance) {
                    percorso[i] = next->distance;
                }
                next = previous_station (A, next);
            }
            curr = prec_tappa;
        }
    }
    for(i=0; i<tappe-1; i++) {
        printf("%d ", percorso[i]);
    }
    printf("%d\n", percorso[tappe-1]);
    free(percorso);
    return;
}

void plan_reverse_route (highway *A, int a, int b) {
    int n = 100;
    int *percorso = (int*)malloc(n * sizeof(int));
    int tappe = 1;
    percorso[tappe-1] = a;
    nodeSTATION start = find_station (A, A->root, a);
    nodeSTATION destinazione = find_station (A, A->root, b);
    nodeSTATION y = previous_station (A, start);
    nodeSTATION curr = start;
    nodeSTATION next = curr;
    while(1) {
        while ((max(curr->distance - curr->max_autonomy, 0) <= y->distance) && (y != A->Tnil)) {
            if ((max(next->distance - next->max_autonomy, 0) > max(y->distance - y->max_autonomy, 0)) || ((max(next->distance - next->max_autonomy, 0) == max(y->distance - y->max_autonomy,0)) && (next->distance > y->distance)) || ((max(next->distance - next->max_autonomy, 0) < destinazione->distance && max(y->distance - y->max_autonomy,0) < destinazione->distance) && (next->distance > y->distance))) {
                next = y;
            }
            if (y == destinazione) {
                tappe++;
                percorso[tappe-1] = destinazione->distance;
                break;
            }
            y = previous_station (A, y);
        }
        if(curr == next) {
            free(percorso);
            printf("nessun percorso\n");
            return;
        }
        if(next == destinazione) {
            break;
        }
        curr = next;
        tappe++;
        if(tappe+1>n) {
            n = 2*n;
            percorso = (int*)realloc(percorso, n*sizeof (int));
        }
        percorso[tappe-1] = curr->distance;
    }
    int i;
    if(tappe>2) {
        nodeSTATION prec_tappa;
        nodeSTATION next_tappa;
        curr = find_station(A, A->root, percorso[1]);
        for (i = tappe - 2; i > 0; i--) {
            prec_tappa = find_station(A, A->root, percorso[i+1]);
            next_tappa = find_station(A, A->root, percorso[i-1]);
            next = previous_station (A, curr);
            while (next != prec_tappa) {
                if (next->distance >= max(next_tappa->distance - next_tappa->max_autonomy, 0) && max(next->distance - next->max_autonomy, 0) <= prec_tappa->distance) {
                    percorso[i] = next->distance;
                }
                next = previous_station (A, next);
            }
            curr = next_tappa;
        }
    }
    for(i=0; i<tappe-1; i++) {
        printf("%d ", percorso[i]);
    }
    printf("%d\n", percorso[tappe-1]);
    free(percorso);
    return;
}