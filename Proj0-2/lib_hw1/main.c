#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "limits.h"

#define INPUT_LEN 100

unsigned hash_hash(const struct hash_elem* e, void* aux) {
	return hash_int(list_entry(&e->list_elem, struct list_item, elem)->data);
}

bool hash_less(const struct hash_elem* a, const struct hash_elem* b, void* aux) {
	int data_a = list_entry(&a->list_elem, struct list_item, elem)->data;
	int data_b = list_entry(&b->list_elem, struct list_item, elem)->data;
    if (data_a < data_b) {
        return true;
    } else {
        return false;
    }
}

// for debugging

void current_state(void){
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
    int list_cnt = 0;
    int hash_cnt = 0;
    int bitmap_cnt = 0;

    while(1) {
        fgets(input_og, sizeof(input_og), stdin);
        input_og[strlen(input_og)-1]='\0';
        if (!strcmp(input_og, "quit")) {
            break;
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
                int list_num = input[2][4]-'0';
                list_array[list_num] = (struct list*)malloc(sizeof(struct list));
                list_init(list_array[list_num]);
                list_cnt ++;
                printf("%d\n", list_cnt);
                current_state();
            }
            else if (!strcmp(input[1], "hashtable")) {
                // create hashtable hash0
                int hash_num = input[2][4]-'0';
                hash_array[hash_num] = (struct hash*)malloc(sizeof(struct hash));
                hash_init(hash_array[hash_num],hash_hash,hash_less,NULL);
                hash_cnt ++;
                printf("%d\n", hash_cnt);
                current_state();
            }
            else if (!strcmp(input[1], "bitmap")) {
                // create bitmap bm0 16
                int bitmap_num = input[2][2]-'0';
                size_t bitmap_size = atoi(input[3]);
                bitmap_array[bitmap_num] = bitmap_create(bitmap_size);
                bitmap_cnt ++;
                printf("%d\n", bitmap_cnt);
                current_state();
            }
        }
    }
}