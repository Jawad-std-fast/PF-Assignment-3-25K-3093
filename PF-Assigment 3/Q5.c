#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

typedef struct {
    char** lines;
    int count;
    int capacity;
} TextBuffer;

TextBuffer* createBuffer() {
    TextBuffer* buffer = malloc(sizeof(TextBuffer));
    if (!buffer) return NULL;
    
    buffer->lines = malloc(INITIAL_CAPACITY * sizeof(char*));
    if (!buffer->lines) {
        free(buffer);
        return NULL;
    }
    
    buffer->count = 0;
    buffer->capacity = INITIAL_CAPACITY;
    return buffer;
}

int ensureCapacity(TextBuffer* buffer) {
    if (buffer->count >= buffer->capacity) {
        int newCapacity = buffer->capacity * 2;
        char** newLines = realloc(buffer->lines, newCapacity * sizeof(char*));
        if (!newLines) return 0;
        
        buffer->lines = newLines;
        buffer->capacity = newCapacity;
    }
    return 1;
}

int insertLine(TextBuffer* buffer, int index, const char* text) {
    if (index < 0 || index > buffer->count) return 0;
    
    if (!ensureCapacity(buffer)) return 0;
    
    char* newLine = malloc(strlen(text) + 1);
    if (!newLine) return 0;
    
    strcpy(newLine, text);
    
    if (index < buffer->count) {
        memmove(&buffer->lines[index + 1], &buffer->lines[index], 
                (buffer->count - index) * sizeof(char*));
    }
    
    buffer->lines[index] = newLine;
    buffer->count++;
    return 1;
}

int deleteLine(TextBuffer* buffer, int index) {
    if (index < 0 || index >= buffer->count) return 0;
    
    free(buffer->lines[index]);
    
    if (index < buffer->count - 1) {
        memmove(&buffer->lines[index], &buffer->lines[index + 1],
                (buffer->count - index - 1) * sizeof(char*));
    }
    
    buffer->count--;
    return 1;
}

void printAllLines(TextBuffer* buffer) {
    if (buffer->count == 0) {
        printf("Buffer is empty.\n");
        return;
    }
     int i;
    for ( i = 0; i < buffer->count; i++) {
        printf("%d: %s\n", i + 1, buffer->lines[i]);
    }
}

int saveToFile(TextBuffer* buffer, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return 0;
     int i;
    for (i = 0; i < buffer->count; i++) {
        fprintf(file, "%s\n", buffer->lines[i]);
    }
    
    fclose(file);
    return 1;
}

int loadFromFile(TextBuffer* buffer, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    while (buffer->count > 0) {
        deleteLine(buffer, 0);
    }
    
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (!insertLine(buffer, buffer->count, line)) {
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 1;
}

void shrinkToFit(TextBuffer* buffer) {
    if (buffer->capacity > buffer->count) {
        char** newLines = realloc(buffer->lines, buffer->count * sizeof(char*));
        if (newLines || buffer->count == 0) {
            buffer->lines = newLines;
            buffer->capacity = buffer->count;
        }
    }
}

void freeAll(TextBuffer* buffer) {
    if (!buffer) return;
    int i;
    for ( i = 0; i < buffer->count; i++) {
        free(buffer->lines[i]);
    }
    
    free(buffer->lines);
    free(buffer);
}

int main() {
    TextBuffer* buffer = createBuffer();
    if (!buffer) {
        printf("Error: Memory allocation failed!\n");
        return 1;
    }
    
    int choice;
    char filename[100];
    char text[1024];
    int index;
    
    do {
        printf("\n=== Minimal Text Editor ===\n");
        printf("1. Insert line\n");
        printf("2. Delete line\n");
        printf("3. Print all lines\n");
        printf("4. Save to file\n");
        printf("5. Load from file\n");
        printf("6. Shrink to fit\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                printf("Enter position (1-%d): ", buffer->count + 1);
                scanf("%d", &index);
                while (getchar() != '\n');
                printf("Enter text: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0';
                
                if (!insertLine(buffer, index - 1, text)) {
                    printf("Error: Failed to insert line!\n");
                } else {
                    printf("Line inserted successfully.\n");
                }
                break;
                
            case 2:
                printf("Enter position (1-%d): ", buffer->count);
                scanf("%d", &index);
                while (getchar() != '\n');
                
                if (!deleteLine(buffer, index - 1)) {
                    printf("Error: Invalid position!\n");
                } else {
                    printf("Line deleted successfully.\n");
                }
                break;
                
            case 3:
                printAllLines(buffer);
                break;
                
            case 4:
                printf("Enter filename: ");
                scanf("%s", filename);
                
                if (!saveToFile(buffer, filename)) {
                    printf("Error: Failed to save file!\n");
                } else {
                    printf("File saved successfully.\n");
                }
                break;
                
            case 5:
                printf("Enter filename: ");
                scanf("%s", filename);
                
                if (!loadFromFile(buffer, filename)) {
                    printf("Error: Failed to load file!\n");
                } else {
                    printf("File loaded successfully.\n");
                }
                break;
                
            case 6:
                shrinkToFit(buffer);
                printf("Buffer shrunk. New capacity: %d\n", buffer->capacity);
                break;
                
            case 7:
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 7);
    
    freeAll(buffer);
    return 0;
}
