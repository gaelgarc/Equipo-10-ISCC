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








///////////////////////////////partes de la interfaz grafica

HWND hwndPrincipal, hwndLogin, hwndCrear, hwndFinal;
HWND hUser1, hPass1, hUser2, hPass2;

LRESULT CALLBACK VentanaPrincipal(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaLogin(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaCrear(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaFinal(HWND, UINT, WPARAM, LPARAM);

void MostrarVentana(HWND mostrar, HWND ocultar) {
    ShowWindow(ocultar, SW_HIDE);
    ShowWindow(mostrar, SW_SHOW);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
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

    hwndPrincipal = CreateWindow("Principal", "Ventana Principal", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndLogin = CreateWindow("Login", "Iniciar Sesion", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndCrear = CreateWindow("CrearCuenta", "Crear Cuenta", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndFinal = CreateWindow("Final", "Bienvenido", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

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
            PostQuitMessage(0);
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
                 printf("%s %s",usuario,clave);
                 if(iniciarSesion(usuario,clave)){
                 	MessageBox(NULL, "BIENVENIDO", "Exito", MB_OK | MB_ICONINFORMATION);
                 	MostrarVentana(hwndFinal, hwnd);
				 }else{
				 	MessageBox(NULL, "Intente de nuevo", "Error", MB_OK | MB_ICONERROR);
				 }
            } else if (LOWORD(wParam) == BTN_REGRESAR_1) {
                MostrarVentana(hwndPrincipal, hwnd);
            }
            break;
        case WM_CLOSE:
            MostrarVentana(hwndPrincipal, hwnd);
            return 0;
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
                MostrarVentana(hwndFinal, hwnd);
            } else if (LOWORD(wParam) == BTN_REGRESAR_2) {
                MostrarVentana(hwndPrincipal, hwnd);
            }
            break;
        case WM_CLOSE:
            MostrarVentana(hwndPrincipal, hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VentanaFinal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            CreateWindow("BUTTON", "Volver a Principal", WS_VISIBLE | WS_CHILD,
                         120, 70, 150, 30, hwnd, (HMENU)BTN_VOLVER, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == BTN_VOLVER) {
                MostrarVentana(hwndPrincipal, hwnd);
            }
            break;
        case WM_CLOSE:
            MostrarVentana(hwndPrincipal, hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

