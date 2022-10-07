#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "limits.h"

#define INPUT_LEN 100

void print_state(void);

int list_cnt = 0;
int hash_cnt = 0;
int bitmap_cnt = 0;


/*hash function*/
unsigned hash_hash(const struct hash_elem* e, void* aux) {
	return hash_int(list_entry(&e->list_elem, struct list_item, elem)->data);
}
bool hash_less(const struct hash_elem* a, struct hash_elem* b, void* aux) {
	int data_a = list_entry(&a->list_elem, struct list_item, elem)->data;
	int data_b = list_entry(&b->list_elem, struct list_item, elem)->data;
    if (data_a < data_b) {
        return true;
    } else {
        return false;
    }
}
void hash_free(struct hash_elem *e, void *aux){
    free(e);
}


// for debugging

void print_state(void){
    printf("%s", "list ");
    for (int i=0; i<10; i++){
        if (list_array[i] == NULL) {
            printf("%s", "X ");
        }
        else {
            printf("%d ", list_array[i]);
        }
    }
    printf("\n");
    printf("%s", "hash ");
    for (int i=0; i<10; i++){
        if (hash_array[i] == NULL) {
            printf("%s", "X ");
        }
        else {
            printf("%d ", hash_array[i]);
        }
    }
    printf("\n");
    printf("%s", "bitmap ");
    for (int i=0; i<10; i++){
        if (bitmap_array[i] == NULL) {
            printf("%s", "X ");
        }
        else {
            printf("%d ", bitmap_array[i]);
        }
    }
    printf("\n");
}

int main(){
    char input_og[INPUT_LEN];
    char input[4][30] = {0,};
    int list_num;
    int hash_num;
    int bitmap_num;

    while(1) {
        list_num, hash_num, bitmap_num = 0;
        fgets(input_og, sizeof(input_og), stdin);
        input_og[strlen(input_og)-1]='\0';
        if (!strcmp(input_og, "quit")) {
            break;
        }
        
        for (int i=0; i<4; i++){
            for (int j=0; j<30; j++){
                input[i][j] = NULL;
            }
        }
        //divide input
        char *ptr = strtok(input_og, " ");
        int i=0;
        while (ptr != NULL) {
            strcpy(input[i],ptr);
            ptr = strtok(NULL, " ");
            i++;
            if (i==5) {
                printf("%s", "input error");
                exit(1);
            }
        }

        if (!strcmp(input[0], "create")){
            if (!strcmp(input[1], "list")) {
                // create list list0
                list_num = input[2][4]-'0';
                list_array[list_num] = (struct list*)malloc(sizeof(struct list));
                list_init(list_array[list_num]);
                /* debugging
                list_cnt ++;
                printf("%d\n", list_cnt);
                print_state();*/
            }
            else if (!strcmp(input[1], "hashtable")) {
                // create hashtable hash0
                hash_num = input[2][4]-'0';
                hash_array[hash_num] = (struct hash*)malloc(sizeof(struct hash));
                hash_init(hash_array[hash_num],hash_hash,hash_less,NULL);
                /* debugging
                hash_cnt ++;
                printf("%d\n", hash_cnt);
                print_state();*/
            }
            else if (!strcmp(input[1], "bitmap")) {
                // create bitmap bm0 16
                bitmap_num = input[2][2]-'0';
                size_t bitmap_size = atoi(input[3]);
                bitmap_array[bitmap_num] = bitmap_create(bitmap_size);
                /* debugging
                bitmap_cnt ++;
                printf("%d\n", bitmap_cnt);
                print_state();*/
            }
        }
        
        else if (!strcmp(input[0], "delete")){
            if (input[1][0]=='l') {
                list_num = input[1][4]-'0';
                if (list_array[list_num] != NULL){
                    free(list_array[list_num]);
                    list_array[list_num]=NULL;
                }
                /* debugging
                list_cnt --;
                print_state();*/
            }
            else if (input[1][0]=='h') {
                hash_num = input[1][4]-'0';
                if (hash_array[hash_num] != NULL){
                    hash_destroy(hash_array[hash_num], hash_free);
                    hash_array[hash_num]=NULL;
                }
                /* debugging
                hash_cnt --;
                print_state();*/

            }
            else if (input[1][0]=='b') {
                bitmap_num = input[1][2]-'0';
                if (bitmap_array[bitmap_num]!=NULL){
                    bitmap_destroy(bitmap_array[bitmap_num]);
                    bitmap_array[bitmap_num]=NULL;
                }
                /* debugging
                bitmap_cnt --;
                print_state();*/
            }
        }
        
        else if (!strcmp(input[0], "dumpdata")){
            if (input[1][0]=='l'){
                list_num = input[1][4]-'0';
                if (list_array[list_num] != NULL){
                    struct list_elem *curr = list_begin(list_array[list_num]);
                    struct list_item* tmp;
                    if (curr->next != NULL) {
                        while (curr->next != NULL){
                            tmp = list_entry(curr, struct list_item, elem);
                            printf("%d ", tmp->data);
                            curr = curr->next;
                        }
                        printf("\n");
                    }
                }
			}
            
            else if (input[1][0]=='h'){
                hash_num = input[1][4]-'0';

            }
            else if (input[1][0]=='b'){
                bitmap_num = input[1][2]-'0';
            }
        }
    
        /****************************************/
        /*LIST***********************************/
        /****************************************/
        else if (!strcmp(input[0], "list_front")){
            list_num = input[1][4] -'0';
            struct list_elem* front = list_front(list_array[list_num]);
            printf("%d\n", list_entry(front, struct list_item, elem)->data);
        }
        else if (!strcmp(input[0], "list_back")){
            list_num = input[1][4] -'0';
            struct list_elem* back = list_front(list_array[list_num]);
            printf("%d\n", list_entry(back, struct list_item, elem)->data);
        }
        else if (!strcmp(input[0], "list_push_back")){
            list_num = input[1][4] -'0';
            struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
            new->data = atoi(input[2]);
            list_push_back(list_array[list_num], &new->elem);
        }
    }
}