#include <stdio.h>
#include <conio.h>
#include <windows.h>

/* 
    Código ascii teclas
	119 -> w
    115 -> s 
    98 -> b
*/
const int ARRIBA = 119;
const int ABAJO = 115;
const int SALIR = 98;
const int YMAX = 0;
const int YMIN = 10;
const int LONGMAX = 50; /* ancho máximo */
const int LONGPISO = 5;
const int LONGROCA = 1; /* tamaño barras */

void gotoxy(int x, int y);
void subir();
void bajar();
void mostrarMatriz(int x, int matriz[100][100], int mostrarNumeros);
void llenarMatriz(int x, int filSup, int filInf, int desde);
void moverMatriz(int longmax, int matriz[100][100]);
void iniciarJuego();
void intervaloTiempo(int num);
void revisarPosicion();
void pantallaInicio();
void pantallaFin();
void pantallaSalir();
void pantallaDificultad();
void println(char texto[300]);

int xObj = 10, yObj = 3, tecla = 0, vida = 1, 
	barra[100][100], puntos = 0, rapidez = 500;


main()
{
	//	Muestra el menu de inicio
	pantallaInicio();
	iniciarJuego();
	getch();
}

void iniciarJuego() {
	
	vida = 1;
	puntos = 0;

	pantallaDificultad();
	
	llenarMatriz(LONGMAX, YMAX, YMIN, 0);
//	mostrarMatriz(LONGMAX, barra, 1);
//	mostrarMatriz(LONGMAX, barra, 0);
//	getch();
			
	while(vida && tecla != SALIR) {
		printf("Puntos: %d", puntos);
		moverMatriz(LONGMAX, barra);
		gotoxy(xObj,yObj);
		
		printf(">"); // Objeto a mover (nave)
		
		// Verdadero cuando se presiona una tecla
		// Evita detener el while
		if(kbhit()){ 
			tecla = getch();	
		}
		
		if(tecla == ARRIBA) {
			subir();
			tecla=0;
			
		} else {
			if(tecla == ABAJO) {
				bajar();
				tecla=0;
			}
		}
		
		revisarPosicion();
		intervaloTiempo(rapidez);
		puntos++;
		system("cls");
	}		
	
	if(!vida) {
		pantallaFin();
	} else {
		pantallaSalir();	
	}
	
	tecla = getch();

	if(tecla == ABAJO) {
		iniciarJuego();
	} else {
		println("|======== Gracias por jugar Gravitix ========|");
	}
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*
 * Disminuye la pasicion de la nave
 */
void subir() {
	if(yObj != YMAX)
		yObj--;
}

/*
 * Aumenta la pasicion de la nave
 */
void bajar() {
	if(yObj != YMIN)
		yObj++;
}


/* Funciones para la matriz */

/*
 * Llena la matriz con 0, 1, 2 donde
 * 0: Espacio que puede ocupar la nave
 * 1: Barreras que chocan con la nave
 * 2: Piso 
 */
void llenarMatriz(int longmax, int filSup, int filInf, int desde) {
	int i, j;
	/*
	i para llenar hacia abajo (columnas)
	j para llenar hacia la derecha (filas)	
	*/

	for(i = 0; i < longmax; i++) {
		for(j = 0; j < longmax; j++){
			if(j == filSup || j == filInf) {
							
				if(i%LONGPISO==0){		
					barra[i][j] = 1;
				} else{ 
					
					barra[i][j] = 2;
				}
				/* printf("Pos(i) %d LON piso: %d es: %d\n", i,LONGPISO,i%LONGPISO); */
			} else {
				if(i % 5 == 0) {
					barra[i][j] = randomNum(2);
				} else {
					barra[i][j] = 0;
				}
				
			}
		}
	}
}

/*
 * Muestra la matriz con los obstaculos
 * o solo la matriz de numeros
 */
void mostrarMatriz(int x, int matriz[100][100], int mostrarNumeros) {
	int i, j;
	for(i = 0; i <= YMIN; i++) {
		for(j = 0; j < x; j++) {
			
			if(mostrarNumeros) {
				printf("%d|", barra[j][i]);
				continue;
			}
			switch(barra[j][i]){
				case 0:
					printf(" ");
					break;
				case 1:
					printf("°");
					break;
				case 2:
					printf("_");
					break;
			}
		}
		printf("\n");
	}
}

/*
 *  Mueve las columnas una posicion antes
 */
void moverMatriz(int longmax, int matriz[100][100]) {
	int i, j, aux;
	/*
	i para llenar hacia abajo (columnas)
	j para llenar hacia la derecha (filas)	
	*/

	for(i = 1; i < longmax; i++) {
		for(j = 0; j < longmax; j++){
			
			aux = barra[i-1][j];
		
			barra[i-1][j] = barra[i][j];
			barra[i][j] = aux;
		}
	}
	
	mostrarMatriz(LONGMAX, barra, 0);
	
}

/*
 * Revisa la posición de la nave para poder
 * comprobar si la nave chocó
 */
void revisarPosicion() {
	/*
	// Debug
	printf("\nx: %d\n", xObj);
	printf("y: %d\n", yObj);
	printf("posicion barra[%d][%d]: %d", xObj, yObj, barra[xObj][yObj]);
	*/
	if(barra[xObj][yObj] == 1){
		vida = 0;
	}
}


/* Funciones de pantalla */

void pantallaInicio() {
	println("|============================================|");
	println("|                                            |");
	println("|                Bienvenido a                |");
	println("|                  Gravitix                  |");
	println("|                                            |");
	println("|Controles:                                  |");
	println("|    w: Arriba                               |");
	println("|    s: Abajo                                |");
	println("|    b: Salir                                |");
	println("|                                            |");
	println("|Objetivo:                                   |");
	println("|    -Navegar por el espacio esquivando      |");
	println("|     los obstaculos.                        |");
	println("|                                            |");
	println("|    -Presione una tecla para jugar          |");
	println("|============================================|");
	tecla = getch();
}

void pantallaDificultad() {
	int opc;
	do {
		println("|============================================|");
		println("|Seleccione la dificultad:                   |");
		println("|   1. Facil                                 |");
		println("|   2. Medio                                 |");
		println("|   3. Dificil                               |");
		println("|============================================|");
		scanf("%d", &opc);
		system("cls");
	} while (opc < 1 || opc > 3);
	
	switch(opc) {
		case 1: 
			rapidez = 800;
			break;
		case 2:
			rapidez = 500;
			break;
		case 3:
			rapidez = 200;
			break;
		default:
			rapidez = 500;
	}
}

void pantallaFin() {
	println("|============================================|");
	println("|      No has sobrevivido al espacio :(      |");
	printf("|           Tu puntaje es: %d                 |\n", puntos);
	println("|       Pulsa 's' para volver a jugar        |");
	println("|============================================|");
}

void pantallaSalir() {
	println("|============================================|");
	println("|         Haz abandonado el espacio :(       |");
	printf("|           Tu puntaje es: %d                 |\n", puntos);
	println("|                                            |");
	println("|       Pulsa 's' para volver a jugar        |");
	println("|============================================|");
}

/* Funciones de ayuda */

void intervaloTiempo(int num) {
	int i;
	for(i=0;i<100000*num;i++);
}

int randomNum(int num) {
    return rand() % num;
}

void println(char texto[300]) {
	printf("%s\n", texto);
}


