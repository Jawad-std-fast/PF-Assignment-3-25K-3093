#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[100];
    char designation[100];
    double salary;
} Employee;

void displayEmployees(Employee *arr, int n) {
    int i;
    printf("ID\tName\t\tDesignation\t\tSalary\n");
    printf("-------------------------------------------------------------\n");
    for (i = 0; i < n; ++i) {
        printf("%d\t%-20s\t%-20s\t%.2f\n", arr[i].id, arr[i].name, arr[i].designation, arr[i].salary);
    }
}

int findHighestSalary(Employee *arr, int n) {
    int i, idx = 0;
    for (i = 1; i < n; ++i) if (arr[i].salary > arr[idx].salary) idx = i;
    return idx;
}

int searchEmployeeByID(Employee *arr, int n, int id) {
    int i;
    for (i = 0; i < n; ++i) if (arr[i].id == id) return i;
    return -1;
}

int searchEmployeeByName(Employee *arr, int n, const char *name) {
    int i;
    for (i = 0; i < n; ++i) if (strcmp(arr[i].name, name) == 0) return i;
    return -1;
}

void applyBonusBelowThreshold(Employee *arr, int n, double threshold, double bonusPercent) {
    int i;
    for (i = 0; i < n; ++i)
        if (arr[i].salary < threshold)
            arr[i].salary += arr[i].salary * (bonusPercent / 100.0);
}

int readInt(const char *msg) {
    char buf[200];
    int x;
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (sscanf(buf, "%d", &x) == 1) return x;
        printf("Invalid integer. Try again.\n");
    }
}

double readDouble(const char *msg) {
    char buf[200];
    double x;
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (sscanf(buf, "%lf", &x) == 1) return x;
        printf("Invalid number. Try again.\n");
    }
}

void readString(const char *msg, char *dest, int size) {
    char buf[300];
    printf("%s", msg);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;
    strncpy(dest, buf, size - 1);
    dest[size - 1] = '\0';
}

int main(void) {
    int n = readInt("Enter number of employees: ");
    if (n <= 0) {
        printf("Invalid number.\n");
        return 1;
    }

    Employee *employees = malloc(sizeof(Employee) * n);
    int i;

    for (i = 0; i < n; ++i) {
        employees[i].id = readInt("Employee ID: ");
        readString("Employee Name: ", employees[i].name, sizeof(employees[i].name));
        readString("Employee Designation: ", employees[i].designation, sizeof(employees[i].designation));
        employees[i].salary = readDouble("Employee Salary: ");
    }

    printf("\nAll Employees:\n");
    displayEmployees(employees, n);

    int idx = findHighestSalary(employees, n);
    printf("\nHighest salary -> ID: %d, Name: %s, Salary: %.2f\n",
           employees[idx].id, employees[idx].name, employees[idx].salary);

    int searchID = readInt("\nEnter an ID to search: ");
    int res = searchEmployeeByID(employees, n, searchID);
    if (res >= 0)
        printf("Found -> ID: %d, Name: %s, Designation: %s, Salary: %.2f\n",
               employees[res].id, employees[res].name, employees[res].designation, employees[res].salary);
    else
        printf("Employee ID %d not found.\n", searchID);

    char nameSearch[100];
    readString("\nEnter name to search (case-sensitive): ", nameSearch, sizeof(nameSearch));
    if (strlen(nameSearch) > 0) {
        int res2 = searchEmployeeByName(employees, n, nameSearch);
        if (res2 >= 0)
            printf("Found -> ID: %d, Name: %s, Designation: %s, Salary: %.2f\n",
                   employees[res2].id, employees[res2].name, employees[res2].designation, employees[res2].salary);
        else
            printf("Employee '%s' not found.\n", nameSearch);
    }

    applyBonusBelowThreshold(employees, n, 50000.0, 10.0);
    printf("\nAfter applying 10%% bonus:\n");
    displayEmployees(employees, n);

    free(employees);
    return 0;
}

