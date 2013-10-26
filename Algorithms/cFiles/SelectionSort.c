#include <stdio.h>

void swap(double array[], int i, int j) {
  double tmp=array[i];
  array[i]=array[j];
  array[j]=tmp;
}

void selection_sort(double array[], int n){
  int i,j;
  for (i=0; i < n-1; i++) {
	int min=i;
	for (j=i+1; j < n; j++) {
	  if (array[j] < array[min])
		min=j;
	}
	swap(array, i, min);  
  }
}


int main(){
  /*Test*/
  int i=0;
  int array[10] = {1,5,7,2,9,3,6,8,0,4};
  
  /*We call our sort algorithm*/
  selection_sort(array, 10);
  
  /*Writing a sorted array*/
  for(i=0; i<10; i++)
    printf("%d ", array[i]);
  printf("\n");
  
  return 0;
}
