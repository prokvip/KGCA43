#include "TObjectPool.h"
/*
* �ü��
* 1)cache memory, chache line(block)
* 2)multi core, & false sharing
* 3)memory ��ġ, �����ּ�, �Ҵ�
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