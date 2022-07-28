/*
 ============================================================================
 Name        : avgArray.c
 Author      : Milton Sierra Solano
 Version     :
 Copyright   : This example code is in the public domain.
 Description : This program calculates and return the average,
               maximum and minimum value of an array with “n” positions. 
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct {
	int32_t max;
	int32_t min;
	int32_t *arrayEvenNumbers;
	int32_t arrayEvenNumbersSize;
	float average;
}avg_obj_t;



/**
 * @brief Checks if num is a even number
 * 
 * @param num Number to be checked
 * @return uint8_t Return 1 for even number and 0 for non even numbers.
 */
uint8_t isEvenNumber(int32_t num) {
	if ((num%2) == 0) {
		return 1;
	}else {
		return 0;
	}
}

/**
 * @brief This function calculates avg, max, min and return a array with 
 *        even numbers
 * 
 * @param a Original array 
 * @param n Size of original array
 * @return avg_obj_t* Return a object with answer
 */
avg_obj_t *avgArray(int32_t *a, int32_t n) {
	int32_t min, max, cntEvenNumbers, sum;
	int32_t *eveNumbersArray;
	avg_obj_t *ret = (avg_obj_t *) malloc(sizeof(avg_obj_t));
	min = a[0];
	max = a[0];
	cntEvenNumbers = 0;
	sum = 0;

	if (isEvenNumber(a[0])) {
		eveNumbersArray = (int32_t *) malloc (sizeof(int32_t));
		eveNumbersArray[cntEvenNumbers] = a[0];
		cntEvenNumbers++;
	}
	printf("%d ", a[0]);

	sum += a[0];

	for (int32_t i = 1; i < n; i++) {
		printf("%d ", a[i]);

		sum += a[i];

		if(min>a[i]) {
			min=a[i];  
		}

		if(max<a[i]) {
			max=a[i];
		}	     

		if (isEvenNumber(a[i])) {
			if (cntEvenNumbers == 0) {
				//if this is the first even number
				eveNumbersArray = (int32_t *) malloc (sizeof(int32_t));
				eveNumbersArray[cntEvenNumbers] = a[i];
			} else {
				//Resizing array for add a new even number
				uint32_t newSize = (cntEvenNumbers + 1) * sizeof(int32_t);
				eveNumbersArray = (int32_t *) realloc (eveNumbersArray, newSize);
				eveNumbersArray[cntEvenNumbers] = a[i];
			}
			cntEvenNumbers++;
		}

	}
	ret->arrayEvenNumbersSize = cntEvenNumbers;
	ret->arrayEvenNumbers = eveNumbersArray;
	ret->max = max;
	ret->min = min;
	ret->average = (float) sum / n;

	return ret;

}

int main(void) {
	avg_obj_t *answer;
	int32_t array [] = {12, 5222, 2, 789, 3, 4, 7, 6, 45, 5, 5555, 20, 15};
	int32_t n = sizeof(array)/sizeof(int32_t);

	printf("Original Array size: %d positions\n", n ); 

	answer = avgArray(&array, n);

	printf("\n\nAverage of original array: %0.2f\n", answer->average);
	printf("Maximum value: %d\n", answer->max);
	printf("Minimum value: %d\n", answer->min);
	printf("\nEven numbers found: %d\nArray with even numbers: ", answer->arrayEvenNumbersSize);

	for(int32_t i = 0; i < answer->arrayEvenNumbersSize; i++) {
		printf("%d ",answer->arrayEvenNumbers[i] );
	}
	printf("\n");

	free(answer);


	return EXIT_SUCCESS;
}
