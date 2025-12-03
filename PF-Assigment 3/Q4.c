#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    int score;
    int accessTime;
};

int main() {
    int capacity, Q,i,j;
    
  
    if (scanf("%d %d", &capacity, &Q) != 2) {
        printf("Error reading input\n");
        return 1;
    }
    
    struct Book* shelf = (struct Book*)malloc(capacity * sizeof(struct Book));
    int size = 0;
    int time = 0;
    

    for ( i = 0; i < capacity; i++) {
        shelf[i].id = -1;
        shelf[i].score = 0;
        shelf[i].accessTime = -1;
    }
    
 
    for ( i = 0; i < Q; i++) {
        char operation[10];
        
        if (scanf("%s", operation) != 1) {
            printf("Error reading operation\n");
            break;
        }
        
        if (strcmp(operation, "ADD") == 0) {
            int id, score;
            if (scanf("%d %d", &id, &score) != 2) {
                printf("Error reading ADD parameters\n");
                break;
            }
            
         
            int found = -1;
            for (j = 0; j < size; j++) {
                if (shelf[j].id == id) {
                    found = j;
                    break;
                }
            }
            
            if (found != -1) {
          
                shelf[found].score = score;
                shelf[found].accessTime = time++;
            } else {
                if (size < capacity) {
          
                    shelf[size].id = id;
                    shelf[size].score = score;
                    shelf[size].accessTime = time++;
                    size++;
                } else {
               
                    int lruIndex = 0;
                    for ( j = 1; j < capacity; j++) {
                        if (shelf[j].accessTime < shelf[lruIndex].accessTime) {
                            lruIndex = j;
                        }
                    }
             
                    shelf[lruIndex].id = id;
                    shelf[lruIndex].score = score;
                    shelf[lruIndex].accessTime = time++;
                }
            }
        }
        else if (strcmp(operation, "ACCESS") == 0) {
            int id;
            if (scanf("%d", &id) != 1) {
                printf("Error reading ACCESS parameter\n");
                break;
            }
            
      
            int found = -1;
            for ( j = 0; j < size; j++) {
                if (shelf[j].id == id) {
                    found = j;
                    break;
                }
            }
            
            if (found != -1) {
    
                printf("%d\n", shelf[found].score);
                shelf[found].accessTime = time++;
            } else {
        
                printf("-1\n");
            }
        }
    }
    
    free(shelf);
    
    return 0;
}
