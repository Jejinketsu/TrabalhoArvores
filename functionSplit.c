#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main(){

	FILE *file;
	char wordsFile[255];
	
	char wordENG[50];
	char wordsPtbr[156];
	char *stringCat;
	file = fopen("palavras.txt", "r");

	while( (fscanf(file , "%s\n",wordsFile))!=EOF ){
		char *palavra_ingles = (char *) malloc(sizeof(char));
		char *palavra_portugues = (char *) malloc(sizeof(char));

		for(int i = 0 ; i < strlen(wordsFile) ; i++){
			if(wordsFile[i]!=':'){
				palavra_ingles[i] = wordsFile[i];
			}else{
				// Pego a palvara em Ingles
				palavra_ingles[i] = '\0';


				// Percorro a lista de palavras em portugues, cada uma vai ser um nó
				int idxENG = 1 ; //Indice do vetor de palvras em ingles
				int size = strlen(wordsFile);
				
				int x = i+1;
				
				while(wordsFile[x]!='\0'){
					palavra_portugues[idxENG-1] = wordsFile[x];

		            if(wordsFile[x]== ',' ){
		                palavra_portugues[idxENG-1] = '\0';
		                idxENG = 0;           
		            	printf("%s\n",palavra_portugues);
					}

					x++;
					idxENG++;

					if(wordsFile[x] =='\0'){
						palavra_portugues[idxENG-1] ='\0';
		            	printf("%s\n",palavra_portugues);
					}
					
					
				}
			}
		}
	}
}