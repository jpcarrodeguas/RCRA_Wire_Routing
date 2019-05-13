RCRA_Wire_routing
=================

Programa que resuelve puzles de Wirerouting utilizando telingo.

Realizado como práctica para la asignatura Representación del Conocimiento y Razonamiento Automático en la Facultad de Informática de La Coruña - Año 18/19

## Compilación

Toda la compilación puede hacerse utilizando el comando make.

```bash
$ make 	
```
	
## Uso

El programa recibe la situación inicial como argumento, esta situación
inicial debe tener el número de filas en la primera linea y el número de
columnas en la segunda fila.
Despues de esto debe ir una representación de la situación inicial con un
caracter por celda del tablero: '.' para representar una celda vacía, '#' para
representar un obstáculo. Los extremos del cable se representarán con los
caracteres 'a', 'b', 'c', 'd' y 'e'. Esta versión solo acepta tableros con
hasta cinco tipos de cables, pero podrian añadirse más de ser necesario.

```bash
$ ./wirerouting <inputfile>
```

Un ejemplo de fichero que el programa toma como situación inicial es:

```
6
7
.a....b
.......
..b..##
.##..##
.##...a
.....##
```

## Salida
El programa muestra por pantalla los pasos que ha tomado para resolver el puzle
con una sencilla animación, de no importar los pasos intermedios, puede verse
el estado final del programa en el fichero `sol_wirerouting.txt`.

Para la situación inicial descrita previamente, la solución escrita en el
fichero sería:

```
.a..bbb
.a..b..
aabbb##
a##..##
a##.aaa
aaaaa##
```

## Resultados
Tiempo que se tarda en resolver los puzles utilizados como benchmark 
(obtenidos con el comando time):

| Tablero	    | Tiempo 		| 
| ------------- |--------------:|
| grid0	        |		0m0,076s|
| grid1         |       0m0,352s|
| grid2         |       0m0,338s|
| grid3         |       0m2,913s|
| grid4         |      1m13,605s| 







