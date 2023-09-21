/*
* test_file_descriptor.c
* Por: Luis Alberto Rivera
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
   FILE *pFile;
   char buffer[100], dummy;

   printf("\nAntes de abrir el archivo\n");
   fflush(stdout);
   scanf("%c", &dummy);	// blocking function
   
   pFile = fopen("test_file_descriptor.c" , "r");
   if(pFile == NULL)
		perror("Error al abrir el archivo.");
   else
   {
	// Lee y despliega las lineas de este archivo .c
		while(!feof(pFile))
		{
			if(fgets(buffer, 100, pFile) == NULL)
			break;
			fputs(buffer, stdout);
		}
     
	// Despliega el descriptor del archivo (uno de los elementos de la estructura FILE)
		printf("\n\nAntes de cerrar el archivo. fd = %d\n", fileno(pFile));
		fflush(stdout);
		scanf("%c", &dummy);	// blocking function
	   
		fclose(pFile);
		 
		printf("\nDespués de cerrar el archivo.\n");
		fflush(stdout);
		scanf("%c", &dummy);	// blocking function
   }
   
   return 0;
}