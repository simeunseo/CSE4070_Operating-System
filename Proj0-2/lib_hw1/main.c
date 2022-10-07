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

/*list function*/
bool list_less(const struct list_elem* a, struct list_elem* b, void* aux) {
	int data_a = list_entry(a, struct list_item, elem)->data;
	int data_b = list_entry(b, struct list_item, elem)->data;
    if (data_a < data_b) {
        return true;
    } else {
        return false;
    }
}

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
            struct list_elem* back = list_back(list_array[list_num]);
            printf("%d\n", list_entry(back, struct list_item, elem)->data);
        }
        else if (!strcmp(input[0], "list_push_front")){
            list_num = input[1][4] -'0';
            struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
            new->data = atoi(input[2]);
            list_push_front(list_array[list_num], &new->elem);
        }
        else if (!strcmp(input[0], "list_push_back")){
            list_num = input[1][4] -'0';
            struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
            new->data = atoi(input[2]);
            list_push_back(list_array[list_num], &new->elem);
        }
        else if (!strcmp(input[0], "list_pop_front")){
            list_num = input[1][4] -'0';
            list_pop_front(list_array[list_num]);
        }
        else if (!strcmp(input[0], "list_pop_back")){
            list_num = input[1][4] -'0';
            list_pop_back(list_array[list_num]);
        }
        else if (!strcmp(input[0], "list_insert")){
            list_num = input[1][4] - '0';
            int index = atoi(input[2]);

            struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
            new->data = atoi(input[3]);

            struct list_elem* tmp = list_head(list_array[list_num]);
            if (index==0){
                list_push_front(list_array[list_num], &new->elem);
            }
            else {
                for (int i=0; i<=index; i++){
                    tmp = tmp->next;
                }
                list_insert(tmp, &new->elem);
            }
        }
        else if (!strcmp(input[0], "list_insert_ordered")){
            list_num = input[1][4] - '0';

            struct list_item* new = (struct list_item*)malloc(sizeof(struct list_item));
            new->data = atoi(input[2]);

            list_insert_ordered(list_array[list_num], &new->elem, list_less, NULL);
        }
        else if (!strcmp(input[0], "list_remove")){
            list_num = input[1][4] - '0';
            int index = atoi(input[2]);

            struct list_elem* tmp = list_head(list_array[list_num]);
            for (int i=0; i<=index; i++){
                tmp = tmp->next;
            }
            list_remove(tmp);
        }
        else if (!strcmp(input[0], "list_empty")) {
            list_num = input[1][4] - '0';
            if (list_empty(list_array[list_num]))
                printf("true\n");
            else 
                printf("false\n");
        }
        else if (!strcmp(input[0], "list_size")) {
            list_num = input[1][4] - '0';
            printf("%zu\n", list_size(list_array[list_num]));
        }
        else if (!strcmp(input[0], "list_max")) {
            list_num = input[1][4] - '0';
            struct list_elem* max_elem = list_max(list_array[list_num], list_less, NULL);
            printf("%d\n", list_entry(max_elem, struct list_item, elem)->data);
        }
        else if (!strcmp(input[0], "list_min")) {
            list_num = input[1][4] - '0';
            struct list_elem* min_elem = list_min(list_array[list_num], list_less, NULL);
            printf("%d\n", list_entry(min_elem, struct list_item, elem)->data);
        }
        else if (!strcmp(input[0], "list_reverse")){
            list_num = input[1][4] - '0';
            list_reverse(list_array[list_num]);
        }
        else if (!strcmp(input[0], "list_sort")){
            list_num = input[1][4] - '0';
            list_sort(list_array[list_num],list_less,NULL);
        }
        else if (!strcmp(input[0], "list_splice")){
            list_num = input[1][4] - '0';
            int dst_list_num = input[3][4] - '0';
            int target_idx = atoi(input[2]);
            int first_idx = atoi(input[4]);
            int last_idx = atoi(input[5]);
            
            struct list_elem* before = list_head(list_array[list_num]);
            struct list_elem* first = list_head(list_array[dst_list_num]);
            struct list_elem* last = list_head(list_array[dst_list_num]);
            
            for (int i=0; i<=target_idx; i++){
                before = before->next;
            }
            for (int i=0; i<=first_idx; i++){
                first = first->next;
            }
            for (int i=0; i<=last_idx; i++){
                last = last->next;
            }

            list_splice(before, first, last);
        }
        else if (!strcmp(input[0], "list_unique")){
            list_num = input[1][4] - '0';
            if (input[2][0] == 'l') {
                int dup_list_num = input[2][4] - '0';
                list_unique(list_array[list_num], list_array[dup_list_num], list_less, NULL);
            }
            else {
                list_unique(list_array[list_num], NULL, list_less, NULL);
            }

        }
        else if (!strcmp(input[0], "list_swap")){
            list_num = input[1][4] - '0';
            int idx_a = atoi(input[2]);
            int idx_b = atoi(input[3]);

            struct list_elem* a = list_head(list_array[list_num]);
            struct list_elem* b = list_head(list_array[list_num]);

            for (int i=0; i<=idx_a; i++){
                a = a->next;
            }
            for (int i=0; i<=idx_b; i++){
                b = b->next;
            }

            list_swap(a,b);
        }
    }
}