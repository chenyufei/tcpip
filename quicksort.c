/*
 * This is used for bisection search
 * 1. sort the numbers in array
 * 2. bisection search the sorted array
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SEARCH 1
/*
 * For sort the number array
 */
int partition(int array[], int start, int end)
{
    int left_point, right_point;
    int pivot;      //Basic point

    left_point = start;
    right_point = end;

    pivot = array[left_point];

    while(left_point < right_point)
    {
        while((right_point > left_point) && (array[right_point] >= pivot))
        {
            right_point-- ;
        }
        array[left_point] = array[right_point];

        while((right_point > left_point) && (array[left_point] <= pivot))
        {
            left_point++ ;
        }
        array[right_point] = array[left_point];
    }

    array[left_point] = pivot;

    return left_point;      //the left_point is where pivot place
}

void quick_sort(int array[], int start, int end)
{
    if(start >= end)
    {
        return ;
    }

    int mid = 0;
    mid = partition(array, start, end);
    quick_sort(array, start, mid-1);
    quick_sort(array, mid+1, end);
}

/*
 * Bisection of the array to search the number
 */
int bisection(int array[], int key, int start, int end)
{
    if(start >= end)
    {
        return -1;
    }

    int mid = (start+end)/2;

    if(array[mid] == key)
    {
        return mid;
    }

    else if(array[mid] < key)
    {
        bisection(array, key, mid+1, end);
    }

    else if(array[mid] > key)
    {
        bisection(array, key, start, mid);
    }

}

/*
 * Main function, only for test
 */

int main(int argc, char *argv[])
{
    int array[10] = {10,9,8,7,6,5,4,3,2,1};
    quick_sort(array, 0, 9);

    int i=0;
    for(i=0; i<10; i++)
    {
        printf("array[%d]=%d\n",i,array[i]);
    }
    
    int pos=0;

    pos = bisection(array, SEARCH, 0, 9);
    if(-1 == pos)
    {
        printf("There was no key=%d\n",SEARCH);
    }

    else
    {
        printf("The key=%d, pos is %d\n", SEARCH,pos);
    }
    return 0;
}
