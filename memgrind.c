#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include "mymalloc.h"
#include <sys/time.h>

bool A(){
	printf("A\n");
	/*
	A: malloc() 1 byte 3000 times, then free() the 3000 1 byte pointers one by one
	*/
    
	char *Aptr[3000];

	for(int i = 0; i < 3000; i++)
	{
		Aptr[i] = (char*)malloc(sizeof(char));
		*(Aptr[i]) = 'A';
	}

	
	for(int k = 0; k < 3000; k++)
	{
		free(Aptr[k]);
	}
	
	return false;
}

bool B(){
	printf("B\n");
	/*
	B: malloc() 1 byte and immediately free it 3000 times in a row
	*/
	
   	for(int i = 0; i <= 3000; i++)
	{
		char* test_B = (char*)malloc(sizeof(char));
		free(test_B);
	}
    	
	return false;
}

bool C(){
	printf("C\n");
	/*
	C: Randomly choose between a 1 byte malloc() or free() 6000 times
	- Keep track of each operation so that you eventually malloc() 3000 bytes, in total
	- Keep track of each operation so that you eventually free() all pointers
	*/
    	
	int rand6000[6000];
	int plusminus;
	char* Bptr[6000];
	int nummalloced = 0;
	int numfreed = 0;

	for(int i = 0; i<6000; i++)
	{
		rand6000[i] = rand() % 2;
	
		if (rand6000[i] == 0)
		{
			
			Bptr[nummalloced] = (char*)malloc(sizeof(char));
			nummalloced++;
		}
        
        if (numfreed < nummalloced){
		if (rand6000[i] == 1)
		{
			free(Bptr[numfreed]);
			numfreed++;
		}
        }

	}
	
	if (nummalloced <3000)
	{
		while(nummalloced < 3000)
		{
			Bptr[nummalloced] = (char*)malloc(sizeof(char));
			if (Bptr[nummalloced] == NULL)
			{
				free(Bptr[numfreed]);
				numfreed++;

				Bptr[nummalloced] = (char*)malloc(sizeof(char));
			}
			nummalloced++;
		}
	}

	plusminus = nummalloced - numfreed;

	if (plusminus>1)
	{
		while (numfreed < nummalloced)
		{
			free(Bptr[numfreed]);
			numfreed++;
		}
	}
	
	return false;
}

bool D(){
	printf("D\n");
	/*
	D: Randomly choose between a randomly-sized malloc() or free 6000 times
	- Keep track of each malloc so that your mallocs do not exceed your memory capacity
	- Keep track of each operation so that you eventually malloc() 3000 times
	- Keep track of each operation so that you eventually free() all pointers
	*/
 
	int rand6000[6000];
	int plusminus;
	char* Bptr[6000];
	int nummalloced = 0;
	int numfreed = 0;

	for(int i = 0; i<6000; i++)
	{
		rand6000[i] = rand() % 2;
	
		if (rand6000[i] == 0)
		{
			int randsize = rand() % 200;
			Bptr[nummalloced] = (char*)malloc(randsize);
			if (Bptr[nummalloced] != NULL)
			{
				nummalloced++;
			}
		}

		if (numfreed < nummalloced){
			if (rand6000[i] == 1)
			{
				free(Bptr[numfreed]);
				numfreed++;
			}
		}
	}
	
	if (nummalloced < 3000)
	{
		while(nummalloced < 3000)
		{
			int randsize = rand() % 200;
			Bptr[nummalloced] = (char*)malloc(randsize);
			
            if (Bptr[nummalloced] != NULL)
            {
                nummalloced++;
            }
		}
	}

	plusminus = nummalloced - numfreed;

	if (plusminus>1)
	{
		while (numfreed < nummalloced)
		{
			free(Bptr[numfreed]);
			numfreed++;
		}
	}
	
	return false;

}

bool E(){
	printf("E\n");
	char *Eptr[1000];
	for(int i = 0; i < 500 ; i++)
	{
		Eptr[i] = (char*)malloc(sizeof(char));
	}

	for(int j = 1; j < 500; j+=2) //frees all odd # pointers
	{
		free(Eptr[j]);
	}

    //printf("---------------------------------------------EVEN--------------------------------------\n");

	for(int k = 0; k< 500; k+=2) //frees all even # pointers and merges consecutive free blocks
	{
		free(Eptr[k]);
	}
	return false;
}


bool F(){
	printf("F\n");
	char *Fptr[500];
	for(int i = 0; i < 500; i++)
	{
		Fptr[i] = (char*)malloc(sizeof(char));
	}
	for(int j = 1; j < 500; j*=10)
	{
		free(Fptr[j]);
	}
	for(int k = 1; k < 500; k*=9)
	{
		free(Fptr[k]);
	}
	for(int l = 1; l < 500; l*=8)
	{
		free(Fptr[l]);
	}
	for(int m = 1; m < 500; m*=7)
	{
		free(Fptr[m]);
	}
	for(int n = 1; n < 500; n*=6)
	{
		free(Fptr[n]);
	}
	for(int o = 1; o < 500; o*=5)
	{
		free(Fptr[o]);
	}
	for(int p = 1; p < 500; p*=4)
	{
		free(Fptr[p]);
	}
	for(int q = 1; q < 500; q*=3)
	{
		free(Fptr[q]);
	}
	for(int r = 1; r < 500; r*=2)
	{
		free(Fptr[r]);
	}
	
	free(Fptr[0]);

	return false;
}


int main(int argc, char const *argv[])
{
//	clock_t starta, enda, startb, endb, startc, endc, startd, endd, starte, ende, startf, endf;
//	double cpu_time_usedA, cpu_time_usedB, cpu_time_usedC, cpu_time_usedD, cpu_time_usedE, cpu_time_usedF;

    unsigned long at, bt, ct, dt, et, ft = 0;

  //  for (int x = 0; x < 100; x++)
   // {
    
//	starta = clock();
    struct timeval stopa, starta;
    gettimeofday(&starta, NULL);
	bool a = A();
    gettimeofday(&stopa, NULL);
//	enda = clock();
	at += stopa.tv_usec-starta.tv_usec;
        
    struct timeval stopb, startb;
    gettimeofday(&startb, NULL);

//	startb = clock();
	bool b = B();
//	endb = clock();
    gettimeofday(&stopb, NULL);

    bt +=stopb.tv_usec-startb.tv_usec; 
 
    struct timeval stopc, startc;
    gettimeofday(&startc, NULL);
	
//	startc = clock();
	bool c = C();
//	endc = clock();
   gettimeofday(&stopc, NULL);
ct+=stopc.tv_usec-startc.tv_usec; 
    struct timeval stopd, startd;
    gettimeofday(&startd, NULL);

//	startd = clock();
	bool d = D();
//	endd = clock();
gettimeofday(&stopd, NULL);
dt+=stopd.tv_usec-startd.tv_usec; 
    struct timeval stope, starte;
    gettimeofday(&starte, NULL);

//	starte = clock();
	bool e = E();
//	ende = clock();
gettimeofday(&stope, NULL);
et+=stope.tv_usec-starte.tv_usec;
    struct timeval stopf, startf;
    gettimeofday(&startf, NULL);

//	startf = clock();
	bool f = F();
//	endf = clock();
gettimeofday(&stopf, NULL);
ft +=stopf.tv_usec-startf.tv_usec; 


/*	cpu_time_usedA= ((double)(enda-starta))/CLOCKS_PER_SEC;
	cpu_time_usedB= ((double)(endb-startb))/CLOCKS_PER_SEC;
	cpu_time_usedC= ((double)(endc-startc))/CLOCKS_PER_SEC;
	cpu_time_usedD= ((double)(endd-startd))/CLOCKS_PER_SEC;
	cpu_time_usedE= ((double)(ende-starte))/CLOCKS_PER_SEC;
	cpu_time_usedF= ((double)(endf-startf))/CLOCKS_PER_SEC;*/
   // }

	printf("The average time to execute A is: %lu\n", at/100);
	printf("The average time to execute B is: %lu\n",bt/100 );
	printf("The average time to execute C is: %lu\n", ct/100)  ;
	printf("The average time to execute D is: %lu\n", dt/100 );
	printf("The average time to execute E is: %lu\n", et/ 100 );
	printf("The average time to execute F is: %lu\n", ft/100 );
	printf("------------------------------------------------------\n");
    
		
	return 0;
}
