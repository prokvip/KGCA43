#include "TObjectPool.h"
/*
* 운영체제
* 1)cache memory, chache line(block)
* 2)multi core, & false sharing
* 3)memory 배치, 기준주소, 할당
* ex)
* int [][] array = new int[64*1024][1024];
* 1)
* for( int a= 0; a < 1024; a++) {
*   for( int b= 0; b < 64*1024; b++) {
*		array[b][a]++;
*   }
* }
* 2)
* for( int a= 0; a < 64*1024; a++) {
*   for( int b= 0; b < 1024; b++) {
*		array[a][b]++;
*   }
* }
*/