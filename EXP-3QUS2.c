#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int start;
    int finish;
} Activity;

int compareActivities(const void *a, const void *b) {
    Activity *actA = (Activity *)a;
    Activity *actB = (Activity *)b;
    return actA->finish - actB->finish;
}

void printActivities(Activity activities[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Activity %d: [%d, %d]\n", activities[i].id, activities[i].start, activities[i].finish);
    }
}

void selectActivities(Activity activities[], int n) {
    if (n <= 0) {
        printf("No activities available.\n");
        return;
    }

    printf("Selected activities (ID, Start, Finish):\n");
    
    int count = 1;
    int last_finish_time = activities[0].finish;
    printf("- Activity %d: [%d, %d]\n", activities[0].id, activities[0].start, activities[0].finish);

    for (int i = 1; i < n; i++) {
        if (activities[i].start >= last_finish_time) {
            printf("- Activity %d: [%d, %d]\n", activities[i].id, activities[i].start, activities[i].finish);
            count++;
            last_finish_time = activities[i].finish;
        }
    }

    printf("Total number of selected activities: %d\n", count);
}

void runTestCase(int testNum, Activity activities[], int n) {
    printf("=========================================\n");
    printf("           RUNNING TEST CASE %d           \n", testNum);
    printf("=========================================\n");
    
    printf("\n--- Before Sorting ---\n");
    printActivities(activities, n);

    qsort(activities, n, sizeof(Activity), compareActivities);

    printf("\n--- After Sorting (by finish time) ---\n");
    printActivities(activities, n);

    printf("\n--- Selection Process ---\n");
    selectActivities(activities, n);
    printf("\n");
}

int main() {
    Activity test1[] = {
        {1, 3, 4},
        {2, 0, 6},
        {3, 1, 2},
        {4, 8, 9},
        {5, 5, 7},
        {6, 5, 9}
    };
    int n1 = sizeof(test1) / sizeof(test1[0]);
    runTestCase(1, test1, n1);

    Activity test2[] = {
        {1, 5, 9},
        {2, 1, 4},
        {3, 0, 4},
        {4, 3, 5},
        {5, 6, 10},
        {6, 8, 10}
    };
    int n2 = sizeof(test2) / sizeof(test2[0]);
    runTestCase(2, test2, n2);

    return 0;
}