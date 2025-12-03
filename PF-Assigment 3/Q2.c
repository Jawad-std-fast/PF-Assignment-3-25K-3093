#include <stdio.h>
#include <stdlib.h>

int simulate(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets) {
    if (fuel <= 0) {
        printf("Planet %d: Fuel exhausted before arrival (fuel=%d)\n", planet, fuel);
        return 0;
    }
    if (planet > totalPlanets) {
        printf("All %d planets visited successfully. Final fuel = %d\n", totalPlanets, fuel);
        return 1;
    }
    fuel -= consumption;
    if (fuel < 0) fuel = 0;
    if (planet % 3 == 0) fuel += recharge;
    if (planet % 4 == 0) fuel += solarBonus;
    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);
    if (fuel <= 0) {
        printf("Mission failed at planet %d: fuel exhausted.\n", planet);
        return 0;
    }
    return simulate(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}

void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int read_int_positive(const char *prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1) {
            clear_stdin();
            if (x >= 0) return x;
            printf("Please enter a non-negative integer.\n");
        } else {
            clear_stdin();
            printf("Invalid input. Please enter an integer.\n");
        }
    }
}

int main(void) {
    int initialFuel = read_int_positive("Enter initial fuel: ");
    int consumption = read_int_positive("Enter consumption per planet: ");
    int recharge = read_int_positive("Enter recharge amount at gravitational-assist planets: ");
    int solarBonus = read_int_positive("Enter solar bonus amount (applies every 4th planet): ");
    int totalPlanets;
    while (1) {
        totalPlanets = read_int_positive("Enter total number of planets to visit: ");
        if (totalPlanets > 0) break;
        printf("Total planets must be at least 1.\n");
    }

    int success = simulate(initialFuel, consumption, recharge, solarBonus, 1, totalPlanets);
    if (success) printf("Mission completed successfully.\n");
    else printf("Mission could not be completed.\n");
    return 0;
}

