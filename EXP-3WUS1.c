#include <stdio.h>

typedef struct {
    int id;
    int start;
    int finish;
} Activity;

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

int main() {
    printf("=========================================\n");
    printf("           RUNNING TEST CASE 1           \n");
    printf("=========================================\n");
    Activity test1[] = {
        {1, 1, 2},
        {2, 3, 4},
        {3, 0, 6},
        {4, 5, 7},
        {5, 8, 9},
        {6, 5, 9}
    };
    int n1 = sizeof(test1) / sizeof(test1[0]);
    selectActivities(test1, n1);

    printf("\n");

    printf("=========================================\n");
    printf("           RUNNING TEST CASE 2           \n");
    printf("=========================================\n");
    Activity test2[] = {
        {1, 1, 3},
        {2, 2, 5},
        {3, 4, 7},
        {4, 1, 8},
        {5, 5, 9},
        {6, 8, 10},
        {7, 9, 11},
        {8, 11, 14}
    };
    int n2 = sizeof(test2) / sizeof(test2[0]);
    selectActivities(test2, n2);

    return 0;
}