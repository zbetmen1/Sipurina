#include <stdio.h>

void swap(double array[], int i, int j) {
  double tmp=array[i];
  array[i]=array[j];
  array[j]=tmp;
}

void bubble_sort(double array[], int n){
  int i,j;
   for (i = n - 1; i > 0; i--){
	for (j = 0; j < i; j++){
	  if (array[j] > array[j + 1]){
		swap(array, i, j);
	  }
	}
  }
}

int main(){
  /*Test*/
  int i=0;
  double array[10] = {1,5,7,2,9,3,6,8,0,4};
  
  /*We call our sort algorithm*/
  bubble_sort(array, 10);
  
  /*Writing a sorted array*/
  for(i=0; i<10; i++)
    printf("%d ", array[i]);
  printf("\n");
  
  return 0;
}
