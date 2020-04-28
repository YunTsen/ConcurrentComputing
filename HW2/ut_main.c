#include <stdio.h>
#include <stdlib.h>

int modifiedBinarySearch(int target, int *list, int start, int end);

void main() {
    //4 8 15 235 17 26 20 65 9 98 0 1 34 784 48 103
    int target = 1;
    int order;
    int list[] = {9,98};
    order = modifiedBinarySearch(target, list, 0, 1);
    printf("order: %d\n", order);
    return;
}

int modifiedBinarySearch(int target, int *list, int start, int end) {
    int middle = (start + end) / 2;
    int element = list[middle];
    printf("middle: %d\n", middle);
    if (start==end) {
        if (target < element)
            return middle;
        else
            return middle + 1;
    } else if (target < element) {
        return modifiedBinarySearch(target, list, start, middle);
    } else {
        return modifiedBinarySearch(target, list, middle + 1, end);
    }
}
