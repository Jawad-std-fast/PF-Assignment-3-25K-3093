#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define FILENAME "members.dat"

typedef struct {
    int studentID;
    char name[MAX_NAME];
    char batch[20];
    char membershipType[10];
    char registrationDate[11];
    char dateOfBirth[11];
    char interest[10];
} Student;

Student* students = NULL;
int studentCount = 0;
int studentCapacity = 0;

void loadDatabase() {
    FILE* file = fopen(FILENAME, "rb");
    if (!file) return;
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    
    studentCount = fileSize / sizeof(Student);
    studentCapacity = studentCount + 10;
    
    students = malloc(studentCapacity * sizeof(Student));
    if (!students) {
        fclose(file);
        return;
    }
    
    fread(students, sizeof(Student), studentCount, file);
    fclose(file);
}

void saveDatabase() {
    FILE* file = fopen(FILENAME, "wb");
    if (!file) {
        printf("Error saving file\n");
        return;
    }
    
    fwrite(students, sizeof(Student), studentCount, file);
    fclose(file);
}

int isIDUnique(int id) {
	int i;
    for ( i = 0; i < studentCount; i++) {
        if (students[i].studentID == id) return 0;
    }
    return 1;
}

void addStudent() {
    if (studentCount >= studentCapacity) {
        studentCapacity += 10;
        Student* temp = realloc(students, studentCapacity * sizeof(Student));
        if (!temp) {
            printf("Memory error\n");
            return;
        }
        students = temp;
    }
    
    printf("Enter Student ID: ");
    scanf("%d", &students[studentCount].studentID);
    
    if (!isIDUnique(students[studentCount].studentID)) {
        printf("ID already exists\n");
        return;
    }
    
    getchar();
    
    printf("Enter Full Name: ");
    fgets(students[studentCount].name, MAX_NAME, stdin);
    students[studentCount].name[strcspn(students[studentCount].name, "\n")] = '\0';
    
    printf("Enter Batch (CS/SE/Cyber Security/AI): ");
    fgets(students[studentCount].batch, 20, stdin);
    students[studentCount].batch[strcspn(students[studentCount].batch, "\n")] = '\0';
    
    printf("Enter Membership Type (IEEE/ACM): ");
    fgets(students[studentCount].membershipType, 10, stdin);
    students[studentCount].membershipType[strcspn(students[studentCount].membershipType, "\n")] = '\0';
    
    printf("Enter Registration Date (YYYY-MM-DD): ");
    scanf("%s", students[studentCount].registrationDate);
    
    printf("Enter Date of Birth (YYYY-MM-DD): ");
    scanf("%s", students[studentCount].dateOfBirth);
    
    printf("Enter Interest (IEEE/ACM/Both): ");
    scanf("%s", students[studentCount].interest);
    
    studentCount++;
    saveDatabase();
    printf("Student added successfully\n");
}

void updateStudent() {
    int id;
    printf("Enter Student ID to update: ");
    scanf("%d", &id);
    int i;
    for ( i = 0; i < studentCount; i++) {
        if (students[i].studentID == id) {
            printf("Enter new Batch (CS/SE/Cyber Security/AI): ");
            getchar();
            fgets(students[i].batch, 20, stdin);
            students[i].batch[strcspn(students[i].batch, "\n")] = '\0';
            
            printf("Enter new Membership Type (IEEE/ACM): ");
            fgets(students[i].membershipType, 10, stdin);
            students[i].membershipType[strcspn(students[i].membershipType, "\n")] = '\0';
            
            saveDatabase();
            printf("Student updated successfully\n");
            return;
        }
    }
    
    printf("Student not found\n");
}

void deleteStudent() {
    int id;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id);
    int i;
    for ( i = 0; i < studentCount; i++) {
        if (students[i].studentID == id) {
        	int j;
            for ( j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            
            FILE* tempFile = fopen("temp.dat", "wb");
            if (tempFile) {
                fwrite(students, sizeof(Student), studentCount, tempFile);
                fclose(tempFile);
                remove(FILENAME);
                rename("temp.dat", FILENAME);
            }
            
            printf("Student deleted successfully\n");
            return;
        }
    }
    
    printf("Student not found\n");
}

void displayAll() {
    if (studentCount == 0) {
        printf("No students registered\n");
        return;
    }
    
    printf("\n=== All Registered Students ===\n");
    printf("%-10s %-20s %-15s %-12s %-15s %-12s %-10s\n", 
           "ID", "Name", "Batch", "Type", "Reg Date", "DOB", "Interest");
    printf("--------------------------------------------------------------------------------\n");
    int i;
    for (i = 0; i < studentCount; i++) {
        printf("%-10d %-20s %-15s %-12s %-15s %-12s %-10s\n",
               students[i].studentID,
               students[i].name,
               students[i].batch,
               students[i].membershipType,
               students[i].registrationDate,
               students[i].dateOfBirth,
               students[i].interest);
    }
}

void batchReport() {
    char batch[20];
    printf("Enter Batch (CS/SE/Cyber Security/AI): ");
    getchar();
    fgets(batch, 20, stdin);
    batch[strcspn(batch, "\n")] = '\0';
    
    printf("\n=== %s Batch Report ===\n", batch);
    printf("%-10s %-20s %-12s %-10s\n", "ID", "Name", "Type", "Interest");
    printf("----------------------------------------\n");
    
    int found = 0;
    int i;
    for (i = 0; i < studentCount; i++) {
        if (strcmp(students[i].batch, batch) == 0) {
            printf("%-10d %-20s %-12s %-10s\n",
                   students[i].studentID,
                   students[i].name,
                   students[i].membershipType,
                   students[i].interest);
            found = 1;
        }
    }
    
    if (!found) printf("No students found in this batch\n");
}

int main() {
    loadDatabase();
    
    int choice;
    
    do {
        printf("\n=== IEEE/ACM Membership System ===\n");
        printf("1. Register new student\n");
        printf("2. Update student\n");
        printf("3. Delete student\n");
        printf("4. View all registrations\n");
        printf("5. Generate batch report\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: updateStudent(); break;
            case 3: deleteStudent(); break;
            case 4: displayAll(); break;
            case 5: batchReport(); break;
            case 6: 
                saveDatabase();
                if (students) free(students);
                printf("Exiting...\n");
                break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 6);
    
    return 0;
}
