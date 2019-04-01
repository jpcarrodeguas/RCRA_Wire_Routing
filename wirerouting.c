/*
* AUTORES
* Xin Lin
* Julián Penedo Carrodeguas
*/

#include <stdio.h>
#include <stdlib.h>


int pos(int row, int col, int numrows){
	return ((row-1)*numrows+(col-1));
}

int row(int pos, int numrows){
	return (pos/numrows)+1;
}

int col(int pos, int numrows){
	return (pos%numrows)+1;
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

void write_init(int *vector, int numrows, int numcols){
	FILE *file;
	int i;
	
	file = fopen("Wirerouting_EDB.txt", "w+");
	
	for(i=0; i<numrows*numcols; i++){
		switch (vector[i]){
			case -1:
				fprintf(file, "block(%d,%d).\n", row(i, numrows), col(i, numrows));
				break;
			case 0:
				break;
			default:
				fprintf(file, "cable(%d,%d,%d).\n", row(i, numrows), col(i, numrows), vector[i]);
				break;
		}
	}
    fclose(file);
}

void telingo_solve(int numrows, int numcols, int numtypes){
	FILE *fp, *f;
	char c;//, row, col, color;
	char *out=malloc(numrows*numcols*sizeof(char));
	int i;
	char command[128];
	//char rowchars[3];
	//char colchars[3];
	
	snprintf(command, sizeof(command), "telingo -c numrows=%d -c numcols=%d -c numtypes=%d Wirerouting_KB.txt Wirerouting_EDB.txt --verbose=0", numrows, numcols, numtypes);
	
	fp = popen(command, "r");
	f = fopen("wirerouting.txt", "w+");

	for (i=0; c!=EOF; i++){
		c = getc(fp);
		fprintf(f, "%c", c);
	}
	
	/*
	c = getc(fp);
	for (i=0; c!=EOF; i++){
		if (c == '('){ 
			rowchars[0] = getc(fp); 	//primer posible numero de fila
			c = getc(fp);
			
			if (c != ','){				//si leo una coma, el numero es de 1 sola cifra
				rowchars[1] = c;		//he leido otro numero, lo guardo en el array
				colchars[2] = '\0';		//null terminated string para que el atoi lea solo hasta donde debe
				row=atoi(rowchars);		//genero número de fila con los dos numeros
				getc(fp);				//como máximo, va a haber 2 numeros, puedo ignorar la coma
			}else{						//he leido una coma, el numero es una sola cifra
				rowchars[1] = '\0';		//null terminated string para que el atoi lea solo hasta donde debe
				row=atoi(rowchars);		//genero número de fila con el numero que lei al principio
			}

			colchars[0] = getc(fp); 	//primer posible numero de columna
			c = getc(fp);
			
			if (c != ','){				//si leo una coma, el numero es de 1 sola cifra
				colchars[1] = c;		//he leido otro numero, lo guardo en el array
				colchars[2] = '\0';		//null terminated string para que el atoi lea solo hasta donde debe
				col=atoi(colchars);		//genero número de columna con los dos numeros
				getc(fp);				//como máximo, va a haber 2 numeros, puedo ignorar la coma
			}else{						//he leido una coma, el numero es una sola cifra
				colchars[1] = '\0';		//null terminated string para que el atoi lea solo hasta donde debe
				col=atoi(colchars);		//genero número de columna con el numero que lei al principio
			}

			int posit = pos(row , col, numrows);
			
			color = getc(fp);
			switch (color){
				case 'b' :
					out[posit]=1;
					break;
				case 'w':
					out[posit]=0;
					break;
				default:
					break;
			}
		}
		c = getc(fp);
	}
	
	pclose(fp);
	
	fp = fopen("binairoasp.txt", "w+");
	for (i=0; i<dimension*dimension; i++){
		fprintf(fp, "%d", out[i]);
		printf("%d", out[i]);
		if (((i) % dimension) == dimension-1){
			fprintf(fp, "\n");
            printf("\n");
		}
	}
	*/
	free(out);
	
}


void read_file(char *filepath){
	int numrows,numcols,numcables=0, c, i=0;
	FILE *file = NULL;
	int *vector;
	
	file = fopen(filepath, "r");
	
	if(file){		
		fscanf (file, "%d", &numrows);
		fscanf (file, "%d", &numcols);
		
		vector = malloc(numrows*numcols*sizeof(int));
		
		/*
		*  # = -1
		*  . = 0
		*  a = 1
		*  b = 2
		* 	 .
		*    .
		*    .
		*/
		
		while ((c = getc(file)) != EOF) {
            switch (c) {
                case '#':
                    vector[i] = -1;
                    i++;
                    break;
                case '.':
                    vector[i] = 0;
                    i++;
                    break;
                case '\n':
                    break;
                default:
                    vector[i] = numfromletter(c);
                    i++;
                    numcables++;
					break;
            }
        }
        
        fclose(file);

        printf("Generating initial configuration...\n");
        write_init(vector, numrows, numcols);
        free(vector);

        printf("Solving...\n");
        telingo_solve(numrows,numcols, numcables/2);
        printf("\nDone! (solution also in 'wirerouting.txt')\n");
	}
}

int main(int argc, char **argv) {

    printf("Wire Routing - RCRA P2\n\n");

    if (argc != 2) {
        printf("*** Usage: wirerouting <filename>\n");
        return 1;
    }

    printf("Input file: %s\n\n", argv[1]);

    read_file(argv[1]);

    return 0;
}
