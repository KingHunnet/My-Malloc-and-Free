#ifndef MYMALLOC_H
#define MYMALLOC_H

//quick and easy def for booleans
typedef enum {false, true} bool; 
/*
#define malloc( x ) mymalloc( x )
#define free( x ) myfree( x )
*/

#define malloc( x ) mymalloc( x )
#define free( x ) myfree( x )


//mymalloc
void* mymalloc(int x);
	

//myfree
void myfree(void* ptr);


#endif

/*

*/
