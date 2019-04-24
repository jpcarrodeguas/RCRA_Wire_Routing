/*
 * AUTORES
 * Xin Lin
 * Julián Penedo Carrodeguas
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

void writestep(int *vector, int numrows, int numcols){
	int i;
	
	//system("clear"); 
    printf("\033[H\033[J");
    printf("Wire Routing - RCRA P2\n\n");

    printf("WARNING! Solving large grids may take a while!\n");
    printf("Generating initial configuration...\n");
    printf("Solving...\n\n");

	for (i=0; i<numrows*numcols; i++){
        printf("%c", vector[i]);
		if (((i) % numcols) == numcols-1){
            printf("\n");
		}
	}
	
	usleep(240000);
	
}

int pos(int row, int col, int numcols){
	return ((row-1)*numcols+(col-1));
}

int row(int pos, int numcols){
	return (pos/numcols)+1;
}

int col(int pos, int numcols){
	return (pos%numcols)+1;
}

int numfromletter(char letter){
	if (letter >= 97 ) 			//la letra es minuscula
		return (letter - 96);
	else if(letter >= 65) 		//la letra es mayuscula
		return (letter - 64);
	else{						// no es una letra, entrada inválida
		printf("Error numfromletter!\n");
		exit(1);
	}
}

char letterfromnum(int num){
	if (num <= 26 ) 			//hay una equivalencia con una letra
		return (num+96);
	else{						//no hay equivalencia, algo va mal
		printf("Error letterfromnum!\n");
		exit(1);
	}
}

void write_init(int *vector, int numrows, int numcols, int numwires){
	FILE *file;
	int i;
	
	file = fopen("Wirerouting_EDB.txt", "w+");
	
	fprintf(file, "#program initial.\ntype(");
	
	for(i=0; i < numwires; i++){	
        fprintf(file, "%c", letterfromnum(i+1));
        if ((i+1) != numwires) {
            fprintf(file, ";");
        }
	}
	
	fprintf(file, ").\n");
	
	for(i=0; i<numrows*numcols; i++){
		switch (vector[i]){
			case '#':
				fprintf(file, "obstacle(%d,%d).\n", row(i, numcols), col(i, numcols));
				break;
			case '.':
				break;
			default:
				fprintf(file, "point(%d,%d,%c).\n", row(i, numcols), col(i, numcols), vector[i]);
				break;
		}
	}
    fclose(file);
}

void telingo_solve(int *vector, int numrows, int numcols, int numwires){
	FILE *fp;
	char c, type;
	char command[160], rowchar[3], colchar[3];
	char *out=calloc(numrows*numcols, sizeof(char));
	char *state=malloc(numrows*numcols*15*sizeof(char));;
	char *atom=malloc(20*sizeof(char));
	int i, numrow, numcol, j=0;
	const char s[2] = " ";
	
	snprintf(command, sizeof(command), "telingo 2>/dev/null -c numrows=%d -c numcols=%d -c numwires=%d 1 "
        "Wirerouting_KB.txt Wirerouting_EDB.txt --opt-mode=optN --quiet=1,1,2 --verbose=0 --imax=%d", numrows, numcols, numwires, numrows*numcols);
	
	fp = popen(command, "r");
	

	c = getc(fp);
	
	for (i=0; c!=EOF; i++){

		if (c == ':'){ 								// principio de un State
		//	printf("\nestoy dentro de un state\n");
			c = getc(fp);
		//	printf("hola1\n");
			fgets(state, numrows*numcols*15, fp); 	// copiar toda la lina
		//	printf("hola2\n");
			atom = strtok(state, s); 		 		// partir por espacios
		//	printf("hola3\n");
			
			while (atom != NULL){			 		// mientras queden atomos
	//			printf("hola4\n");
				//printf("voy a procesar un atomo\n");
                if (atom[0]=='c'){                  // el atomo es un currentpoint					
					rowchar[0]=atom[13];			// primera cifra de la fila
					if(atom[14] == ','){		    // la fila es de una sola cifra
						rowchar[1] = '\0';			// null terminated string
						colchar[0] = atom[15];		// primera cifra de la columna
						if(atom[16] == ','){		    // la columna es de una sola cifra
							colchar[1] = '\0';		// null terminated string
							type = atom[17];		// tipo del cable
						} else{						// la columna tiene mas de una cifra 
							colchar[1] = atom[16];	// segunda cifra de la columna
							colchar[2] = '\0';		// null terminated string
							type = atom[18];		// tipo del cable
						}
					}else{							// la fila tiene mas de una cifra
						rowchar[1] = atom[14];		// segunda cifra de la fila
						rowchar[2] = '\0';			// null terminated string
						colchar[0] = atom[16];		// primera cifra de la columna
						if(atom[17] == ','){		// la columna es de una sola cifra
							colchar[1] = '\0';		// null terminated string
							type = atom[18];		// tipo del cable
						} else{						// la columna tiene mas de una cifra 
							colchar[1] = atom[17];	// segunda cifra de la columna
							colchar[2] = '\0';		// null terminated string
							type = atom[19];		// tipo del cable
						}
					}
					numrow = atoi(rowchar);
					numcol = atoi(colchar);			
					vector[pos(numrow, numcol, numcols)] = type;

				}/* else if (atom[0]=='o'){ 	 		// el atomo es un obstacle
					rowchar[0]=atom[9];				// primera cifra de la fila
					if(atom[10] == ','){			// la fila es de una sola cifra
						rowchar[1] = '\0';			// null terminated string
						colchar[0] = atom[11];		// primera cifra de la columna
						if(atom[12] == ','){		// la columna es de una sola cifra
							colchar[1] = '\0';		// null terminated string
						} else{						// la columna tiene mas de una cifra 
							colchar[1] = atom[12];	// segunda cifra de la columna
							colchar[2] = '\0';		// null terminated string
						}
					}else{							// la fila tiene mas de una cifra
						rowchar[1] = atom[10];		// segunda cifra de la fila
						rowchar[2] = '\0';			// null terminated string
						colchar[0] = atom[12];		// primera cifra de la columna
						if(atom[13] == ','){		// la columna es de una sola cifra
							colchar[1] = '\0';		// null terminated string
						} else{						// la columna tiene mas de una cifra 
							colchar[1] = atom[13];	// segunda cifra de la columna
							colchar[2] = '\0';		// null terminated string
						}
					}
					numrow=atoi(rowchar);
					numcol=atoi(colchar);			
					out[pos(numrow, numcol, numcols)]='#';
					
				}else if (atom[0]=='p'){ 			// el atomo es un point					
					rowchar[0]=atom[6];				// primera cifra de la fila
					if(atom[7] == ','){			    // la fila es de una sola cifra
						rowchar[1] = '\0';			// null terminated string
						colchar[0] = atom[8];		// primera cifra de la columna
						if(atom[9] == ','){		    // la columna es de una sola cifra
							colchar[1] = '\0';		// null terminated string
							type = atom[10];		// tipo del cable
						} else{						// la columna tiene mas de una cifra 
							colchar[1] = atom[9];	// segunda cifra de la columna
							colchar[2] = '\0';		// null terminated string
							type = atom[11];		// tipo del cable
						}
					}else{							// la fila tiene mas de una cifra
						rowchar[1] = atom[7];		// segunda cifra de la fila
						rowchar[2] = '\0';			// null terminated string
						colchar[0] = atom[9];		// primera cifra de la columna
						if(atom[10] == ','){		// la columna es de una sola cifra
							colchar[1] = '\0';		// null terminated string
							type = atom[11];		// tipo del cable
						} else{						// la columna tiene mas de una cifra 
							colchar[1] = atom[10];	// segunda cifra de la columna
							colchar[2] = '\0';		// null terminated string
							type = atom[12];		// tipo del cable
						}
					}
					numrow = atoi(rowchar);
					numcol = atoi(colchar);			
					out[pos(numrow, numcol, numcols)] = type;
				}*/
			//printf("termine de procesar un atomo\n");
			atom = strtok(NULL, s);
			}
		j++;
		writestep(vector, numrows, numcols);
		}											// fin de State
		c = getc(fp);
	}
	// WRITE OUT TO FILE
	pclose(fp);

	fp = fopen("sol_wirerouting.txt", "w+");
	for (i=0; i<numrows*numcols; i++){
        fprintf(fp, "%c", vector[i]);
        //printf("%c", vector[i]);
		if (((i) % numcols) == numcols-1){
            fprintf(fp, "\n");
            //printf("\n");
		}
	}
    fclose(fp);
	free(atom);
	free(state);
	free(out);
}

void read_file(char *filepath){
	int numrows,numcols,numwires=0, c, i=0;
	FILE *file = NULL;
	int *vector;
	
	file = fopen(filepath, "r");
	
	if(file){		
		fscanf (file, "%d", &numrows);
		fscanf (file, "%d", &numcols);
		
		vector = malloc(numrows*numcols*sizeof(int));
		
		while ((c = getc(file)) != EOF) {
            switch (c) {
                case '#':
                    vector[i] = c;
                    i++;
                    break;
                case '.':
                    vector[i] = c;
                    i++;
                    break;
                case '\n':
                    break;
                default:
                    vector[i] = c;
                    i++;
                    numwires++;
					break;
            }
        }
        
        fclose(file);

        printf("WARNING! Solving large grids may take a while!\n");
        printf("Generating initial configuration...\n");
        write_init(vector, numrows, numcols, numwires/2);
        //free(vector);

        printf("Solving...\n\n");

        for (i=0; i<numrows*numcols; i++){
            printf("%c", vector[i]);
		    if (((i) % numcols) == numcols-1){
                printf("\n");
		    }
	    }

        telingo_solve(vector, numrows,numcols, numwires/2);
        printf("\nDone! (solution also in 'sol_wirerouting.txt')\n");
        free(vector);
	}
}

int main(int argc, char **argv) {

    printf("\033[H\033[J");
    printf("Wire Routing - RCRA P2\n\n");
	
    if (argc != 2) {
        printf("*** Usage: wirerouting <filename>\n");
        return 1;
    }

    //printf("Input file: %s\n\n", argv[1]);

    read_file(argv[1]);

    return 0;
}
