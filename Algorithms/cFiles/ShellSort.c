#include <stdio.h>

void swap(double array[], int i, int j) {
  double tmp=array[i];
  array[i]=array[j];
  array[j]=tmp;
}

void shell_sort(double array[], int n) {
  int gap, i, j;
  for (gap= n/2; gap > 0; gap /= 2) {
	for (i = gap; i < n; i++) {
	  for (j=i-gap; j>=0; j-=gap) {
		if (array[j] > array[j+gap]) {
		  swap(array, j, j+gap);
		}
		else
		  break;
	  }
	}
  }
}

int main(){
  /*Test*/
  int i=0;
  int array[10] = {1,5,7,2,9,3,6,8,0,4};
  
  /*We call our sort algorithm*/
  shell_sort(array, 10);
  
  /*Writing a sorted array*/
  for(i=0; i<10; i++)
	printf("%d ", array[i]);
  printf("\n");
  
  return 0;
}
