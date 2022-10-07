#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "limits.h"

#define INPUT_LEN 100
#define PARAMETER_NUM 4
#define PARAMETER_LEN 30

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
void hash_square(struct hash_elem* e, void* aux) {
    int data = hash_entry(e, struct hash_item, elem)->data;
    data *= data;
    hash_entry(e, struct hash_item, elem)->data = data;
}
void hash_triple(struct hash_elem* e, void* aux) {
    int data = hash_entry(e, struct hash_item, elem)->data;
    data *= data * data;
    hash_entry(e, struct hash_item, elem)->data = data;
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
    char input[PARAMETER_NUM][PARAMETER_LEN] = {0,};
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
        
        for (int i=0; i<PARAMETER_NUM; i++){
            for (int j=0; j<PARAMETER_LEN; j++){
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
                struct hash_iterator i;
                hash_first(&i, hash_array[hash_num]);
                if (!hash_empty(hash_array[hash_num])){
                    while (hash_next(&i) != NULL) {
                        printf("%d ", hash_entry(hash_cur(&i), struct hash_item, elem)->data);
                    }
                    printf("\n");
                }
            }
            else if (input[1][0]=='b'){
                bitmap_num = input[1][2]-'0';
                if (bitmap_array[bitmap_num] != NULL){
                    for (int i=0; i<bitmap_size(bitmap_array[bitmap_num]); i++){
                        if (bitmap_test(bitmap_array[bitmap_num],i))
                            printf("1");
                        else 
                            printf("0");
                    }
                    printf("\n");
                }
            }
        }
    
        /****************************************/
        /*LIST*/
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
        else if (!strcmp(input[0], "list_shuffle")){
            list_num = input[1][4] - '0';
            list_shuffle(list_array[list_num]);
        }
        /****************************************/
        /*HASH*/
        /****************************************/
        else if (!strcmp(input[0], "hash_insert")){
            hash_num = input[1][4] - '0';
            struct hash_item* new = (struct hash_item*)malloc(sizeof(struct hash_item));
            new->data = atoi(input[2]);
            hash_insert(hash_array[hash_num], &new->elem);
        }
        else if (!strcmp(input[0], "hash_apply")){
            hash_num = input[1][4] - '0';
            if (!strcmp(input[2], "square")){
                hash_apply(hash_array[hash_num], hash_square);
            }
            else if (!strcmp(input[2], "triple")){
                hash_apply(hash_array[hash_num], hash_triple);
            }
        }
        else if (!strcmp(input[0], "hash_delete")){
            hash_num = input[1][4] - '0';
            struct hash_item* target = (struct hash_item*)malloc(sizeof(struct hash_item));
            target->data = atoi(input[2]);
            hash_delete(hash_array[hash_num], &target->elem);
        }
        else if (!strcmp(input[0], "hash_empty")){
            hash_num = input[1][4] - '0';
            if (hash_empty(hash_array[hash_num]))
				printf("true\n");
			else
				printf("false\n");
        }
        else if (!strcmp(input[0], "hash_size")){
            hash_num = input[1][4] - '0';
            printf("%zu\n", hash_size(hash_array[hash_num]));
        }
        else if (!strcmp(input[0], "hash_clear")){
            hash_clear(hash_array[hash_num], NULL);
        }
        else if (!strcmp(input[0], "hash_find")){
            hash_num = input[1][4] - '0';
            struct hash_item* target = (struct hash_item*)malloc(sizeof(struct hash_item));
            target->data = atoi(input[2]);
            if (hash_find(hash_array[hash_num], &target->elem)){
                printf("%d\n", target->data);
            }
        }
        else if (!strcmp(input[0], "hash_replace")){
            hash_num = input[1][4] - '0';
            struct hash_item* target = (struct hash_item*)malloc(sizeof(struct hash_item));
            target->data = atoi(input[2]);
            hash_replace(hash_array[hash_num], &target->elem);
        }
        /****************************************/
        /*BITMAP*/
        /****************************************/
        else if (!strcmp(input[0], "bitmap_mark")){
            bitmap_num = input[1][2] - '0';
            bitmap_mark(bitmap_array[bitmap_num], atoi(input[2]));
        }
        else if (!strcmp(input[0], "bitmap_all")){
            bitmap_num = input[1][2] - '0';
            int start = atoi(input[2]);
            int cnt = atoi(input[3]);
            if (bitmap_all(bitmap_array[bitmap_num], start, cnt)) 
                printf("true\n");
            else
                printf("false\n");
        }
        else if (!strcmp(input[0], "bitmap_any")){
            bitmap_num = input[1][2] - '0';
            int start = atoi(input[2]);
            int cnt = atoi(input[3]);
            if (bitmap_any(bitmap_array[bitmap_num], start, cnt)) 
                printf("true\n");
            else
                printf("false\n");
        }
        else if (!strcmp(input[0], "bitmap_contains")){
            bitmap_num = input[1][2] - '0';
            int start = atoi(input[2]);
            int cnt = atoi(input[3]);
            
            if (!strcmp(input[4], "true")){
                if (bitmap_contains(bitmap_array[bitmap_num], start, cnt, true))
                    printf("true\n");
                else 
                    printf("false\n");
            }
            else if (!strcmp(input[4], "false")){
                if (bitmap_contains(bitmap_array[bitmap_num], start, cnt, false))
                    printf("true\n");
                else 
                    printf("false\n");
            }
        }
        else if (!strcmp(input[0], "bitmap_count")){
            bitmap_num = input[1][2] - '0';
            int start = atoi(input[2]);
            int cnt = atoi(input[3]);

            if (!strcmp(input[4], "true"))
                printf("%zu\n", bitmap_count(bitmap_array[bitmap_num], start, cnt, true));
			else if (!strcmp(input[4], "false"))
				printf("%zu\n", bitmap_count(bitmap_array[bitmap_num], start, cnt, false));
			
        }
        else if (!strcmp(input[0], "bitmap_dump")){
            bitmap_num = input[1][2] - '0';
            bitmap_dump(bitmap_array[bitmap_num]);
        }
        else if (!strcmp(input[0], "bitmap_set")){
            bitmap_num = input[1][2] - '0';
            if (!strcmp(input[3], "true"))
                bitmap_set(bitmap_array[bitmap_num], atoi(input[2]), true);
            else if (!strcmp(input[3], "false"))
                bitmap_set(bitmap_array[bitmap_num], atoi(input[2]), false);
        }
        else if (!strcmp(input[0], "bitmap_set_all")){
            bitmap_num = input[1][2] - '0';
            if (!strcmp(input[2], "true"))
                bitmap_set_all(bitmap_array[bitmap_num], true);
            else if (!strcmp(input[2], "false"))
                bitmap_set_all(bitmap_array[bitmap_num], false);
        }
        else if (!strcmp(input[0], "bitmap_set_multiple")){
            bitmap_num = input[1][2] - '0';
            int start = atoi(input[2]);
            int cnt = atoi(input[3]);

            if (!strcmp(input[4], "true"))
                bitmap_set_multiple(bitmap_array[bitmap_num], start, cnt, true);
            else if (!strcmp(input[4], "false"))
                bitmap_set_multiple(bitmap_array[bitmap_num], start, cnt, false);
        }
        else if (!strcmp(input[0], "bitmap_expand")){
            bitmap_num = input[1][2] - '0';
            bitmap_expand(bitmap_array[bitmap_num], atoi(input[2]));
        }
        else if (!strcmp(input[0], "bitmap_flip")){
            bitmap_num = input[1][2] - '0';
            bitmap_flip(bitmap_array[bitmap_num], atoi(input[2]));
        }
        else if (!strcmp(input[0], "bitmap_none")){
            bitmap_num = input[1][2] - '0';
            int start = atoi(input[2]);
            int cnt = atoi(input[3]);
            if (bitmap_none(bitmap_array[bitmap_num], start, cnt)) 
                printf("true\n");
            else
                printf("false\n");
        }
        else if (!strcmp(input[0], "bitmap_reset")){}
        else if (!strcmp(input[0], "bitmap_scan")){}
        else if (!strcmp(input[0], "bitmap_scan_and_flip")){}
        else if (!strcmp(input[0], "bitmap_size")){}
        else if (!strcmp(input[0], "bitmap_test")){}
    }
    return 0;
}