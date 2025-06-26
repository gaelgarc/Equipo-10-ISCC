#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define BTN_INICIAR_SESION  1
#define BTN_CREAR_CUENTA    2
#define BTN_REGRESAR_1      3
#define BTN_LOGIN           4
#define BTN_CREAR           5
#define BTN_VOLVER          6
#define BTN_REGRESAR_2      7

struct usuario {
    char nombre[100];
    char clave[100];
    double puntajeMaximo;
};

 struct usuario* usuarios;
 struct usuario USUARIOACTUAL;

int cantidad = 0;

void cerrarPrograma(){
	PostQuitMessage(0);
}

void imprimirUsuarios() {
	printf("entre a imprimir jotos");
    if (usuarios == NULL || cantidad == 0) {
        printf("No hay usuarios para mostrar.\n");
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        printf("Usuario #%d:\n", i + 1);
        printf("  Nombre: %s\n", usuarios[i].nombre);
        printf("  Clave: %s\n", usuarios[i].clave);
        printf("  Puntaje Maximo: %.2f\n", usuarios[i].puntajeMaximo);
    }
}


bool leerUsuarios(const char* archivo) {
    FILE* f = fopen(archivo, "rb");
    if (!f) {
        cantidad = 0;
        return false;
    }

    struct usuario* usuarios1 = NULL;
    struct usuario temp;
    int count = 0;

    while (fread(&temp, sizeof(struct usuario), 1, f) == 1) {
        usuarios1 = (struct usuario*) realloc(usuarios1, (count + 1) * sizeof(struct usuario));
        if (usuarios1 == NULL) {
            perror("Fallo realloc");
            fclose(f);
            exit(EXIT_FAILURE);
        }
        usuarios1[count++] = temp;
    }

    fclose(f);
    cantidad = count;
    usuarios=usuarios1;
    return true;
}

void verificarLista() {
    if (usuarios == NULL || cantidad == 0) {
        printf("No se pudo leer el archivo o no hay usuarios.\n");
    } else {
        printf("Usuarios cargados: %d\n", cantidad);
        for (int i = 0; i < cantidad; i++) {
            printf("Nombre: %s, Clave: %s, Puntaje: %.2f\n",
                   usuarios[i].nombre, usuarios[i].clave, usuarios[i].puntajeMaximo);
        }
    }
}

bool iniciarSesion(char nombre[100],char clave[100]) {

    for (int i = 0; i < cantidad; i++) {
        if (strcmp(usuarios[i].nombre, nombre) == 0 &&
            strcmp(usuarios[i].clave, clave) == 0) {
            USUARIOACTUAL = usuarios[i];
            printf("Inicio de sesion exitoso. Bienvenido, %s\n", USUARIOACTUAL.nombre);
            return true;
        }
    }

    printf("Nombre o clave incorrectos.\n");
    return false;
}

bool cerrarSesion(){
	printf("USUARIO: %s CLAVE %s PUNTAJE %d",USUARIOACTUAL.nombre,USUARIOACTUAL.clave,USUARIOACTUAL.puntajeMaximo);
	strcpy(USUARIOACTUAL.clave,"");
	strcpy(USUARIOACTUAL.nombre,"");
	USUARIOACTUAL.puntajeMaximo=0;
	printf("USUARIO: %s CLAVE %s PUNTAJE %d",USUARIOACTUAL.nombre,USUARIOACTUAL.clave,USUARIOACTUAL.puntajeMaximo);
}

struct usuario* agregarUsuario(const char* nombre, const char* clave, double puntaje) {
    struct usuario nuevo;
    strncpy(nuevo.nombre, nombre, sizeof(nuevo.nombre));
    strncpy(nuevo.clave, clave, sizeof(nuevo.clave));
    nuevo.puntajeMaximo = puntaje;

    struct usuario* temp = (struct usuario*) realloc(usuarios, (cantidad + 1) * sizeof(struct usuario));
    if (temp == NULL) {
        perror("Fallo realloc");
        exit(EXIT_FAILURE);
    }

    temp[cantidad] = nuevo;
    cantidad++;

    return temp;
}
bool existeUsuario(const char* nombre) {
    for (int i = 0; i < cantidad; i++) {
        if (strcmp(usuarios[i].nombre, nombre) == 0) {
            return true; // Usuario encontrado
        }
    }
    return false; // Usuario no existe
}

bool crearCuenta(char nombre[],char clave[] ) {
     if (existeUsuario(nombre)) {
        printf("El usuario '%s' ya existe.\n", nombre);
        return false;
    }
    double puntaje = 0.0;
    // Agregar a la lista
    usuarios = agregarUsuario(nombre, clave, puntaje);

    // Establecer como usuario actual
    strncpy(USUARIOACTUAL.nombre, nombre, sizeof(USUARIOACTUAL.nombre));
    strncpy(USUARIOACTUAL.clave, clave, sizeof(USUARIOACTUAL.clave));
    USUARIOACTUAL.puntajeMaximo = puntaje;
    imprimirUsuarios();

    printf("Usuario creado y activo: %s\n",USUARIOACTUAL.nombre);
    return true;
}



#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mezclar(int* arreglo, int tam) {
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
}

void generarMatriz24(int matriz[6][4]) {
    int filas = 6;
    int columnas = 4;
    int total = filas * columnas; // 24

    // Vector con valores del 1 al 12
    int vector[12];
    for (int i = 0; i < 12; i++) {
        vector[i] = i + 1;
    }

    // Crear un arreglo temporal con cada n�mero dos veces
    int arregloTemporal[24];
    for (int i = 0; i < 12; i++) {
        arregloTemporal[2 * i] = vector[i];
        arregloTemporal[2 * i + 1] = vector[i];
    }

    // Mezclar los 24 elementos
    mezclar(arregloTemporal, 24);

    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = arregloTemporal[i * columnas + j];
        }
    }

    // Mostrar la matriz
    printf("Matriz generada:\n");
    for (int i = 0; i < columnas; i++) {
        for (int j = 0; j < filas; j++) {
            printf("%2d ", matriz[i][j]);
        }
        printf("\n");
    }
    
}









///////////////////////////////partes de la interfaz grafica

HWND hwndPrincipal, hwndLogin, hwndCrear, hwndFinal, hwndJuego;
HWND hUser1, hPass1, hUser2, hPass2,hLabelUsuario;

LRESULT CALLBACK VentanaPrincipal(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaLogin(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaCrear(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaFinal(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaJuego(HWND, UINT, WPARAM, LPARAM);
void MostrarVentana(HWND mostrar, HWND ocultar) {
    ShowWindow(ocultar, SW_HIDE);
    ShowWindow(mostrar, SW_SHOW);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	srand(time(NULL));
	///parte del back
	const char* archivo = "usuarios.dat";
     if( leerUsuarios(archivo)){
     	  MessageBox(NULL, "Base de datos cargada", "Exito", MB_OK | MB_ICONINFORMATION);
     	  verificarLista();
	 }else{
	 	 MessageBox(NULL, "Error al cargar base de datos", "Error", MB_OK | MB_ICONERROR);
         return 0; // Termina el programa
	 }
	
	////////////
    WNDCLASS wc = {0};

    wc.hInstance = hInstance;

    wc.lpszClassName = "Principal";
    wc.lpfnWndProc = VentanaPrincipal;
    RegisterClass(&wc);

    wc.lpszClassName = "Login";
    wc.lpfnWndProc = VentanaLogin;
    RegisterClass(&wc);

    wc.lpszClassName = "CrearCuenta";
    wc.lpfnWndProc = VentanaCrear;
    RegisterClass(&wc);

    wc.lpszClassName = "Final";
    wc.lpfnWndProc = VentanaFinal;
    RegisterClass(&wc);

    wc.lpfnWndProc = VentanaJuego;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Juego";
    RegisterClass(&wc);
    hwndPrincipal = CreateWindow("Principal", "Ventana Principal", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndLogin = CreateWindow("Login", "Iniciar Sesion", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndCrear = CreateWindow("CrearCuenta", "Crear Cuenta", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndFinal = CreateWindow("Final", "Bienvenido", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

    hwndJuego = CreateWindow("Juego", "Memorama", WS_OVERLAPPEDWINDOW,
                         100, 100, 600, 400, NULL, NULL, hInstance, NULL);

    ShowWindow(hwndPrincipal, nCmdShow);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK VentanaPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            CreateWindow("BUTTON", "Iniciar Sesion", WS_VISIBLE | WS_CHILD,
                         100, 50, 180, 30, hwnd, (HMENU)BTN_INICIAR_SESION, NULL, NULL);
            CreateWindow("BUTTON", "Crear Cuenta", WS_VISIBLE | WS_CHILD,
                         100, 100, 180, 30, hwnd, (HMENU)BTN_CREAR_CUENTA, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == BTN_INICIAR_SESION) {
                MostrarVentana(hwndLogin, hwnd);
            } else if (LOWORD(wParam) == BTN_CREAR_CUENTA) {
                MostrarVentana(hwndCrear, hwnd);
            }
            break;
        case WM_DESTROY:
            cerrarPrograma();
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VentanaLogin(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            CreateWindow("STATIC", "Usuario:", WS_VISIBLE | WS_CHILD, 50, 40, 60, 20, hwnd, NULL, NULL, NULL);
            hUser1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 40, 180, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Clave:", WS_VISIBLE | WS_CHILD, 50, 80, 60, 20, hwnd, NULL, NULL, NULL);
            hPass1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 120, 80, 180, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "Login", WS_VISIBLE | WS_CHILD,
                         50, 130, 100, 30, hwnd, (HMENU)BTN_LOGIN, NULL, NULL);
            CreateWindow("BUTTON", "Regresar", WS_VISIBLE | WS_CHILD,
                         180, 130, 100, 30, hwnd, (HMENU)BTN_REGRESAR_1, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == BTN_LOGIN) {
            	 char usuario[100];
                 char clave[100];
                 GetWindowText(hUser1, usuario, sizeof(usuario));
                 GetWindowText(hPass1, clave, sizeof(clave));
                 if(iniciarSesion(usuario,clave)){
                 	 char texto[200];
                     sprintf(texto, "Usuario actual: %s", USUARIOACTUAL.nombre);
                     SetWindowText(hLabelUsuario, texto);
                 	MessageBox(NULL, "BIENVENIDO", "Exito", MB_OK | MB_ICONINFORMATION);
                 	SetWindowText(hUser1,"");
                 	SetWindowText(hPass1,"");
                 	MostrarVentana(hwndFinal, hwnd);
				 }else{
				 	MessageBox(NULL, "Intente de nuevo", "Error", MB_OK | MB_ICONERROR);
				 	SetWindowText(hUser1,"");
                 	SetWindowText(hPass1,"");
				 }
            } else if (LOWORD(wParam) == BTN_REGRESAR_1) {
                MostrarVentana(hwndPrincipal, hwnd);
            }
            break;
        case WM_CLOSE:
             cerrarPrograma();
             break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VentanaCrear(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            CreateWindow("STATIC", "Nuevo Usuario:", WS_VISIBLE | WS_CHILD, 50, 40, 80, 20, hwnd, NULL, NULL, NULL);
            hUser2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 40, 180, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Nueva Clave:", WS_VISIBLE | WS_CHILD, 50, 80, 80, 20, hwnd, NULL, NULL, NULL);
            hPass2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 140, 80, 180, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "Crear", WS_VISIBLE | WS_CHILD,
                         50, 130, 100, 30, hwnd, (HMENU)1, NULL, NULL);
            CreateWindow("BUTTON", "Regresar", WS_VISIBLE | WS_CHILD,
                         180, 130, 100, 30, hwnd, (HMENU)BTN_REGRESAR_2, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
            	char usuario[100],clave[100];
            	GetWindowText(hUser2, usuario, sizeof(usuario));
                GetWindowText(hPass2, clave, sizeof(clave));
                if(crearCuenta(usuario,clave)){
                    char texto[200];
                     sprintf(texto, "Usuario actual: %s", USUARIOACTUAL.nombre);
                     SetWindowText(hLabelUsuario, texto);
                   MessageBox(NULL, "Creada la cuenta", "Correcto", MB_OK | MB_ICONINFORMATION);
                   SetWindowText(hUser2,"");
                   SetWindowText(hPass2,"");
                   MostrarVentana(hwndFinal, hwnd);
                }else{
                    MessageBox(NULL, "Intente de nuevo", "Error", MB_OK | MB_ICONERROR);
                    SetWindowText(hUser2,"");
                    SetWindowText(hPass2,"");
                }
            	
                //MostrarVentana(hwndFinal, hwnd);
            } else if (LOWORD(wParam) == BTN_REGRESAR_2) {
                MostrarVentana(hwndPrincipal, hwnd);
            }
            break;
        case WM_CLOSE:
             cerrarPrograma();
             break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VentanaFinal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            CreateWindow("BUTTON", "Volver a Principal", WS_VISIBLE | WS_CHILD,
                         120, 70, 150, 30, hwnd, (HMENU)BTN_VOLVER, NULL, NULL);
            hLabelUsuario = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD,
                                         50, 20, 250, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("BUTTON", "Jugar", WS_VISIBLE | WS_CHILD,
                 120, 120, 150, 30, hwnd, (HMENU)100, NULL, NULL);

            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == BTN_VOLVER) {
            	cerrarSesion();
                MostrarVentana(hwndPrincipal, hwnd);
            }
            if (LOWORD(wParam) == 100) {
                 MostrarVentana(hwndJuego, hwnd);
            }
            break;
        case WM_CLOSE:
            cerrarPrograma();
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

#define TOTAL_CARTAS 24

HWND botonesJuego[TOTAL_CARTAS];
HWND txtPuntaje;

LRESULT CALLBACK VentanaJuego(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int tablero[6][4];
    static int primerSeleccion = -1;
    static int segundoSeleccion = -1;
    static int contador = 0;
    static int puntaje = 0;

    switch (msg) {
        case WM_CREATE: {
            int x = 30, y = 30;
            int ancho = 60, alto = 60;
            int margen = 10;

            for (int i = 0; i < TOTAL_CARTAS; i++) {
                botonesJuego[i] = CreateWindow("BUTTON", "?", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                               x, y, ancho, alto, hwnd, (HMENU)(INT_PTR)(200 + i), NULL, NULL);

                EnableWindow(botonesJuego[i], FALSE); // ?? Desactivar al inicio

                x += ancho + margen;
                if ((i + 1) % 6 == 0) {
                    x = 30;
                    y += alto + margen;
                }
            }

            // Cuadro de puntaje
            CreateWindow("STATIC", "Puntaje:", WS_VISIBLE | WS_CHILD,
                         400, 80, 60, 20, hwnd, NULL, NULL, NULL);
            txtPuntaje = CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | ES_READONLY,
                                      470, 80, 60, 20, hwnd, NULL, NULL, NULL);

            // Botones de control
            CreateWindow("BUTTON", "Regresar", WS_VISIBLE | WS_CHILD,
                         400, 20, 100, 30, hwnd, (HMENU)300, NULL, NULL);
            CreateWindow("BUTTON", "Iniciar", WS_VISIBLE | WS_CHILD,
                         400, 150, 100, 30, hwnd, (HMENU)4, NULL, NULL);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 4) { // Bot�n "Iniciar"
                generarMatriz24(tablero);

                primerSeleccion = -1;
                segundoSeleccion = -1;
                contador = 0;
                puntaje = 0;

                // Actualizar campo de puntaje
                SetWindowText(txtPuntaje, "0");

                for (int i = 0; i < TOTAL_CARTAS; i++) {
                    SetWindowText(botonesJuego[i], "?");
                    EnableWindow(botonesJuego[i], TRUE); // ? Activar botones
                }
                HWND botonIniciar = GetDlgItem(hwnd, 4);
                EnableWindow(botonIniciar, FALSE);

            }
            else if (LOWORD(wParam) == 300) {
                MostrarVentana(hwndFinal, hwnd);
            }
            else if (LOWORD(wParam) >= 200 && LOWORD(wParam) < 200 + TOTAL_CARTAS) {
                int indice = LOWORD(wParam) - 200;
                int fila = indice / 6;
                int columna = indice % 6;

                char texto[4];
                sprintf(texto, "%d", tablero[fila][columna]);
                SetWindowText(botonesJuego[indice], texto);

                if (contador == 0) {
                    primerSeleccion = indice;
                    contador = 1;
                } else if (contador == 1 && indice != primerSeleccion) {
                    segundoSeleccion = indice;
                    contador = 2;
                    SetTimer(hwnd, 1, 1000, NULL); // Espera 1 segundo
                }
            }
            break;

        case WM_TIMER:
            if (wParam == 1) {
                KillTimer(hwnd, 1);

                int fila1 = primerSeleccion / 6;
                int col1 = primerSeleccion % 6;
                int fila2 = segundoSeleccion / 6;
                int col2 = segundoSeleccion % 6;

                if (tablero[fila1][col1] == tablero[fila2][col2]) {
                    EnableWindow(botonesJuego[primerSeleccion], FALSE);
                    EnableWindow(botonesJuego[segundoSeleccion], FALSE);

                    // ?? Aumentar puntaje
                    puntaje += 10;
                    char nuevoPuntaje[10];
                    sprintf(nuevoPuntaje, "%d", puntaje);
                    SetWindowText(txtPuntaje, nuevoPuntaje);
                } else {
                    SetWindowText(botonesJuego[primerSeleccion], "?");
                    SetWindowText(botonesJuego[segundoSeleccion], "?");
                }

                primerSeleccion = -1;
                segundoSeleccion = -1;
                contador = 0;
            }
            break;

        case WM_CLOSE:
            cerrarPrograma();
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
