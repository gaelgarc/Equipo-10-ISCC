#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *imagenes[] = {
    "descarga.bmp",
    "imagenes/imagen2.bmp",
    "imagenes/imagen3.bmp",
    "imagenes/imagen4.bmp",
    "imagenes/imagen5.bmp",
    "imagenes/imagen6.bmp",
    "imagenes/imagen7.bmp",
    "imagenes/imagen8.bmp",
    "imagenes/imagen9.bmp",
    "imagenes/imagen10.bmp",
    "imagenes/imagen11.bmp",
    "imagenes/imagen12.bmp"};

enum Botones
{
    BTN_INICIAR_SESION = 1,
    BTN_CREAR_CUENTA,
    BTN_REGRESAR_1,
    BTN_LOGIN,
    BTN_CREAR,
    BTN_VOLVER,
    BTN_REGRESAR_2,
    BTN_JUGAR,
    BTN_RANKING,
    BTN_REGRESAR_3

};

#define TOTAL_CARTAS 24

HWND botonesJuego[TOTAL_CARTAS];
HWND txtPuntaje;

struct usuario
{
    char nombre[100];
    char clave[100];
    double puntajeMaximo;
};

typedef struct
{
    int posicion;
    union
    {
        char nombre[50];
        int puntaje;
    } datos;
} EntradaRanking;

struct usuario *usuarios;
struct usuario USUARIOACTUAL;

int cantidad = 0;

void guardarUsuarios(const char *archivo)
{
    FILE *f = fopen(archivo, "wb");
    if (!f)
    {
        perror("No se pudo abrir el archivo para escritura");
        return;
    }

    fwrite(usuarios, sizeof(struct usuario), cantidad, f);
    fclose(f);
}

void borrarArchivo()
{
    char nombreArchivo[200];
    printf("ENTRE A BORRAR EL ARCHIVO");

    // Copiamos el nombre base y le agregamos la extensi�n .txt
    strcpy(nombreArchivo, USUARIOACTUAL.nombre);
    strcat(nombreArchivo, ".txt");

    // Intentamos borrar el archivo
    if (remove(nombreArchivo) == 0)
    {
        printf("Archivo '%s' eliminado exitosamente.\n", nombreArchivo);
    }
    else
    {
        perror("Error al eliminar el archivo");
    }
}

void cerrarPrograma()
{
    const char *archivo = "usuarios.dat";
    guardarUsuarios(archivo);
    PostQuitMessage(0);
}

void imprimirUsuarios()
{
    printf("entre a imprimir jotos");
    if (usuarios == NULL || cantidad == 0)
    {
        printf("No hay usuarios para mostrar.\n");
        return;
    }

    for (int i = 0; i < cantidad; i++)
    {
        printf("Usuario #%d:\n", i + 1);
        printf("  Nombre: %s\n", usuarios[i].nombre);
        printf("  Clave: %s\n", usuarios[i].clave);
        printf("  Puntaje Maximo: %.2f\n", usuarios[i].puntajeMaximo);
    }
}

void modificarPuntuacion()
{
    printf("MODIFICANDO PUNTUACION\n");
    for (int i = 0; i < cantidad; i++)
    {
        if (strcmp(USUARIOACTUAL.nombre, usuarios[i].nombre) == 0 && strcmp(USUARIOACTUAL.clave, usuarios[i].clave) == 0)
        {
            usuarios[i].puntajeMaximo = USUARIOACTUAL.puntajeMaximo;
        }
    }
    imprimirUsuarios();
}

bool leerUsuarios(const char *archivo)
{
    FILE *f = fopen(archivo, "rb");
    if (!f)
    {
        cantidad = 0;
        return false;
    }

    struct usuario *usuarios1 = NULL;
    struct usuario temp;
    int count = 0;

    while (fread(&temp, sizeof(struct usuario), 1, f) == 1)
    {
        usuarios1 = (struct usuario *)realloc(usuarios1, (count + 1) * sizeof(struct usuario));
        if (usuarios1 == NULL)
        {
            perror("Fallo realloc");
            fclose(f);
            exit(EXIT_FAILURE);
        }
        usuarios1[count++] = temp;
    }

    fclose(f);
    cantidad = count;
    usuarios = usuarios1;
    return true;
}

void verificarLista()
{
    if (usuarios == NULL || cantidad == 0)
    {
        printf("No se pudo leer el archivo o no hay usuarios.\n");
    }
    else
    {
        printf("Usuarios cargados: %d\n", cantidad);
        for (int i = 0; i < cantidad; i++)
        {
            printf("Nombre: %s, Clave: %s, Puntaje: %.2f\n",
                   usuarios[i].nombre, usuarios[i].clave, usuarios[i].puntajeMaximo);
        }
    }
}

bool iniciarSesion(char nombre[100], char clave[100])
{

    for (int i = 0; i < cantidad; i++)
    {
        if (strcmp(usuarios[i].nombre, nombre) == 0 &&
            strcmp(usuarios[i].clave, clave) == 0)
        {
            USUARIOACTUAL = usuarios[i];
            printf("Inicio de sesion exitoso. Bienvenido, %s\n", USUARIOACTUAL.nombre);
            return true;
        }
    }

    printf("Nombre o clave incorrectos.\n");
    return false;
}

bool cerrarSesion()
{
    printf("USUARIO: %s CLAVE %s PUNTAJE %d", USUARIOACTUAL.nombre, USUARIOACTUAL.clave, USUARIOACTUAL.puntajeMaximo);
    strcpy(USUARIOACTUAL.clave, "");
    strcpy(USUARIOACTUAL.nombre, "");
    USUARIOACTUAL.puntajeMaximo = 0;
    printf("USUARIO: %s CLAVE %s PUNTAJE %d", USUARIOACTUAL.nombre, USUARIOACTUAL.clave, USUARIOACTUAL.puntajeMaximo);
    MessageBox(NULL, "!Felicidades, ganaste!", "Mensaje", MB_OK | MB_ICONINFORMATION);
}

struct usuario *agregarUsuario(const char *nombre, const char *clave, double puntaje)
{
    struct usuario nuevo;
    strncpy(nuevo.nombre, nombre, sizeof(nuevo.nombre));
    strncpy(nuevo.clave, clave, sizeof(nuevo.clave));
    nuevo.puntajeMaximo = puntaje;

    struct usuario *temp = (struct usuario *)realloc(usuarios, (cantidad + 1) * sizeof(struct usuario));
    if (temp == NULL)
    {
        perror("Fallo realloc");
        exit(EXIT_FAILURE);
    }

    temp[cantidad] = nuevo;
    cantidad++;

    return temp;
}
bool existeUsuario(const char *nombre)
{
    for (int i = 0; i < cantidad; i++)
    {
        if (strcmp(usuarios[i].nombre, nombre) == 0)
        {
            return true; // Usuario encontrado
        }
    }
    return false; // Usuario no existe
}

bool crearCuenta(char nombre[], char clave[])
{
    if (existeUsuario(nombre))
    {
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

    printf("Usuario creado y activo: %s\n", USUARIOACTUAL.nombre);
    return true;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mezclar(int *arreglo, int tam)
{
    for (int i = tam - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
}

void generarMatriz24(int matriz[4][6])
{
    int total = 24;
    int pares = 12;

    // Paso 1: Crear arreglo con dos copias de cada n�mero del 1 al 12
    int arreglo[24];
    for (int i = 0; i < pares; i++)
    {
        arreglo[2 * i] = i + 1;
        arreglo[2 * i + 1] = i + 1;
    }

    // Paso 2: Mezclar el arreglo
    mezclar(arreglo, total);

    // Paso 3: Llenar matriz horizontalmente: fila por fila, izquierda a derecha
    int index = 0;
    for (int fila = 0; fila < 4; fila++)
    {
        for (int columna = 0; columna < 6; columna++)
        {
            matriz[fila][columna] = arreglo[index++];
        }
    }

    // [Opcional] Imprimir la matriz horizontalmente
    printf("Matriz generada horizontalmente (6 filas  4 columnas):\n");
    for (int fila = 0; fila < 4; fila++)
    {
        for (int columna = 0; columna < 6; columna++)
        {
            printf("%2d ", matriz[fila][columna]);
        }
        printf("\n");
    }

    // [Opcional] Validar repeticiones
    int conteo[13] = {0};
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 4; j++)
            conteo[matriz[i][j]]++;

    printf("\nConteo de numeros:\n");
    for (int i = 1; i <= 12; i++)
    {
        printf("%2d: %d veces\n", i, conteo[i]);
    }
}

bool cargar(HWND hwnd, int tablero[4][6], int *puntaje, int *encontradas)
{
    char archivo[150];
    sprintf(archivo, "%s.txt", USUARIOACTUAL.nombre);

    FILE *file = fopen(archivo, "r");
    if (!file)
    {
        MessageBox(hwnd, "No hay partida guardada para este usuario.", "Informacion", MB_OK | MB_ICONINFORMATION);
        return false;
    }

    // Leer la matriz
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 6; j++)
            fscanf(file, "%d", &tablero[i][j]);

    int encontradasTemp = 0;

    // Leer estado de botones
    for (int i = 0; i < 24; i++)
    {
        int estado;
        fscanf(file, "%d", &estado);
        int fila = i / 6;
        int col = i % 6;

        if (estado == 1)
        {
            char texto[4];
            sprintf(texto, "%d", tablero[fila][col]);
            SetWindowText(botonesJuego[i], texto);
            EnableWindow(botonesJuego[i], FALSE);
            encontradasTemp++; // contar los botones desactivados
        }
        else
        {
            SetWindowText(botonesJuego[i], "?");
            EnableWindow(botonesJuego[i], TRUE);
        }
    }

    // Calcular pares encontrados
    *encontradas = encontradasTemp / 2;

    // Leer puntaje
    fscanf(file, "%d", puntaje);
    char textoPuntaje[10];
    sprintf(textoPuntaje, "%d", *puntaje);
    SetWindowText(txtPuntaje, textoPuntaje);

    fclose(file);

    // Desactivar bot�n iniciar
    HWND botonIniciar = GetDlgItem(hwnd, 4);
    EnableWindow(botonIniciar, FALSE);

    return true;
}

void ReiniciarJuego(HWND hwnd, int tablero[4][6], int *puntaje, int *encontradas)
{
    generarMatriz24(tablero);

    *puntaje = 0;
    *encontradas = 0;
    // primerSeleccion = -1;
    // segundoSeleccion = -1;
    // contador = 0;

    // Actualiza el puntaje en pantalla
    SetWindowText(txtPuntaje, "0");

    for (int i = 0; i < TOTAL_CARTAS; i++)
    {
        SetWindowText(botonesJuego[i], "?");
        EnableWindow(botonesJuego[i], FALSE);
    }

    // Reactivar el bot�n "Iniciar" si quieres
    HWND botonIniciar = GetDlgItem(hwnd, 4);
    EnableWindow(botonIniciar, TRUE); // O TRUE si quieres permitir reiniciar manualmente
}

void guardarPartida(int tablero[4][6], HWND botones[], int puntaje)
{
    char archivo[150];
    sprintf(archivo, "%s.txt", USUARIOACTUAL.nombre);

    FILE *file = fopen(archivo, "w");
    if (!file)
        return;

    // Guardar la matriz
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            fprintf(file, "%d ", tablero[i][j]);
        }
        fprintf(file, "\n");
    }

    // Guardar estado de los botones: 1 = descubierta, 0 = tapada
    for (int i = 0; i < TOTAL_CARTAS; i++)
    {
        BOOL habilitado = IsWindowEnabled(botones[i]);
        fprintf(file, "%d ", habilitado ? 0 : 1); // Si est� deshabilitado, se descubri�
    }

    fprintf(file, "\n%d\n", puntaje);
    fclose(file);
}

///////////////////////////////partes de la interfaz grafica

HWND hwndPrincipal, hwndLogin, hwndCrear, hwndFinal, hwndJuego, hwndRanking;
HWND hUser1, hPass1, hUser2, hPass2, hLabelUsuario;

LRESULT CALLBACK VentanaPrincipal(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaLogin(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaCrear(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaFinal(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaJuego(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VentanaRanking(HWND, UINT, WPARAM, LPARAM);
void MostrarVentana(HWND mostrar, HWND ocultar)
{
    ShowWindow(ocultar, SW_HIDE);
    ShowWindow(mostrar, SW_SHOW);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    srand(time(NULL));
    /// parte del back
    const char *archivo = "usuarios.dat";
    if (leerUsuarios(archivo))
    {
        MessageBox(NULL, "Base de datos cargada", "Exito", MB_OK | MB_ICONINFORMATION);
        verificarLista();
    }
    else
    {
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

    wc.lpfnWndProc = VentanaRanking;
    wc.lpszClassName = "Ranking";
    RegisterClass(&wc);

    hwndPrincipal = CreateWindow("Principal", "Ventana Principal", WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndLogin = CreateWindow("Login", "Iniciar Sesion", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndCrear = CreateWindow("CrearCuenta", "Crear Cuenta", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndFinal = CreateWindow("Final", "Bienvenido", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, NULL, NULL, hInstance, NULL);

    hwndJuego = CreateWindow("Juego", "Memorama", WS_OVERLAPPEDWINDOW,
                             100, 100, 600, 400, NULL, NULL, hInstance, NULL);

    hwndRanking = CreateWindow("Ranking", "Ranking", WS_OVERLAPPEDWINDOW,
                               100, 100, 600, 400, NULL, NULL, hInstance, NULL);

    ShowWindow(hwndPrincipal, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK VentanaPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateWindow("BUTTON", "Iniciar Sesion", WS_VISIBLE | WS_CHILD,
                     100, 50, 180, 30, hwnd, (HMENU)BTN_INICIAR_SESION, NULL, NULL);
        CreateWindow("BUTTON", "Crear Cuenta", WS_VISIBLE | WS_CHILD,
                     100, 100, 180, 30, hwnd, (HMENU)BTN_CREAR_CUENTA, NULL, NULL);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == BTN_INICIAR_SESION)
        {
            MostrarVentana(hwndLogin, hwnd);
        }
        else if (LOWORD(wParam) == BTN_CREAR_CUENTA)
        {
            MostrarVentana(hwndCrear, hwnd);
        }
        break;
    case WM_DESTROY:
        cerrarPrograma();
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VentanaLogin(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
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
        if (LOWORD(wParam) == BTN_LOGIN)
        {
            char usuario[100];
            char clave[100];
            GetWindowText(hUser1, usuario, sizeof(usuario));
            GetWindowText(hPass1, clave, sizeof(clave));
            if (iniciarSesion(usuario, clave))
            {
                char texto[200];
                sprintf(texto, "Usuario actual: %s", USUARIOACTUAL.nombre);
                SetWindowText(hLabelUsuario, texto);
                MessageBox(NULL, "BIENVENIDO", "Exito", MB_OK | MB_ICONINFORMATION);
                SetWindowText(hUser1, "");
                SetWindowText(hPass1, "");
                MostrarVentana(hwndFinal, hwnd);
            }
            else
            {
                MessageBox(NULL, "Intente de nuevo", "Error", MB_OK | MB_ICONERROR);
                SetWindowText(hUser1, "");
                SetWindowText(hPass1, "");
            }
        }
        else if (LOWORD(wParam) == BTN_REGRESAR_1)
        {
            MostrarVentana(hwndPrincipal, hwnd);
        }
        break;
    case WM_CLOSE:
        cerrarPrograma();
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VentanaCrear(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
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
        if (LOWORD(wParam) == 1)
        {
            char usuario[100], clave[100];
            GetWindowText(hUser2, usuario, sizeof(usuario));
            GetWindowText(hPass2, clave, sizeof(clave));
            if (crearCuenta(usuario, clave))
            {
                char texto[200];
                sprintf(texto, "Usuario actual: %s", USUARIOACTUAL.nombre);
                SetWindowText(hLabelUsuario, texto);
                MessageBox(NULL, "Creada la cuenta", "Correcto", MB_OK | MB_ICONINFORMATION);
                SetWindowText(hUser2, "");
                SetWindowText(hPass2, "");
                MostrarVentana(hwndFinal, hwnd);
            }
            else
            {
                MessageBox(NULL, "Intente de nuevo", "Error", MB_OK | MB_ICONERROR);
                SetWindowText(hUser2, "");
                SetWindowText(hPass2, "");
            }

            // MostrarVentana(hwndFinal, hwnd);
        }
        else if (LOWORD(wParam) == BTN_REGRESAR_2)
        {
            MostrarVentana(hwndPrincipal, hwnd);
        }
        break;
    case WM_CLOSE:
        cerrarPrograma();
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK VentanaFinal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateWindow("BUTTON", "Cerrar Sesion", WS_VISIBLE | WS_CHILD,
                     120, 70, 150, 30, hwnd, (HMENU)BTN_VOLVER, NULL, NULL);
        hLabelUsuario = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD,
                                     50, 20, 250, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("BUTTON", "Jugar", WS_VISIBLE | WS_CHILD,
                     120, 120, 150, 30, hwnd, (HMENU)BTN_JUGAR, NULL, NULL);

        CreateWindow("BUTTON", "Rankings", WS_VISIBLE | WS_CHILD,
                     120, 170, 150, 30, hwnd, (HMENU)BTN_RANKING, NULL, NULL);

        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == BTN_VOLVER)
        {
            cerrarSesion();
            MostrarVentana(hwndPrincipal, hwnd);
        }
        if (LOWORD(wParam) == BTN_JUGAR)
        {
            MostrarVentana(hwndJuego, hwnd);
        }
        if (LOWORD(wParam) == BTN_RANKING)
        {
            MostrarVentana(hwndRanking, hwnd);
        }
        break;
    case WM_CLOSE:
        cerrarPrograma();
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

EntradaRanking *obtenerTopRanking(usuario *usuarios)
{
    if (cantidad == 0 || usuarios == NULL)
        return NULL;

    // Crear copia para ordenar sin afectar original
    usuario *copia = (usuario *)malloc(cantidad * sizeof(usuario));
    if (!copia)
        return NULL;

    memcpy(copia, usuarios, cantidad * sizeof(usuario));

    // Ordenar por puntajeMaximo descendente (bubble sort simple)
    for (int i = 0; i < cantidad - 1; i++)
    {
        for (int j = 0; j < cantidad - i - 1; j++)
        {
            if (copia[j].puntajeMaximo < copia[j + 1].puntajeMaximo)
            {
                usuario temp = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = temp;
            }
        }
    }

    // Crear el ranking de 10 (o menos si hay pocos usuarios)
    int top = (cantidad < 10) ? cantidad : 10;
    EntradaRanking *ranking = (EntradaRanking *)malloc(top * sizeof(EntradaRanking));
    if (!ranking)
    {
        free(copia);
        return NULL;
    }

    for (int i = 0; i < top; i++)
    {
        ranking[i].posicion = i + 1;
        strncpy(ranking[i].datos.nombre, copia[i].nombre, 49);
        ranking[i].datos.nombre[49] = '\0';
    }

    free(copia);
    return ranking;
}

#define IDC_LISTBOX 101
#define TOTAL_ENTRADAS 10
LRESULT CALLBACK VentanaRanking(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndListBox;
    static EntradaRanking *ranking = NULL;
    static int totalMostrado = 0;

    switch (msg)
    {
    case WM_CREATE:
    {
        hwndListBox = CreateWindow("LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
                                   10, 10, 260, 200, hwnd, (HMENU)IDC_LISTBOX,
                                   ((LPCREATESTRUCT)lParam)->hInstance, NULL);

        CreateWindow("BUTTON", "Regresar", WS_VISIBLE | WS_CHILD,
                     120, 250, 150, 30, hwnd, (HMENU)BTN_REGRESAR_3,
                     ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        break;
    }

    case WM_SHOWWINDOW:
        if (wParam == TRUE)
        { // La ventana se esta mostrando
            // Liberar ranking anterior si existe
            if (ranking)
            {
                free(ranking);
                ranking = NULL;
            }

            // Obtener nuevo ranking
            ranking = obtenerTopRanking(usuarios);
            if (!ranking)
            {
                MessageBox(hwnd, "No se pudo generar el ranking.", "Error", MB_ICONERROR);
                return -1;
            }

            totalMostrado = (cantidad < 10) ? cantidad : 10;

            // Actualizar contenido del listbox
            SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
            for (int i = 0; i < totalMostrado; i++)
            {
                char texto[120];
                snprintf(texto, sizeof(texto), "Posicion %d: %s",
                         ranking[i].posicion, ranking[i].datos.nombre);
                SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)texto);
            }
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_LISTBOX && HIWORD(wParam) == LBN_SELCHANGE)
        {
            int index = (int)SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
            if (index != LB_ERR && index < totalMostrado)
            {
                char texto[100];
                snprintf(texto, sizeof(texto), "Seleccionaste: %s", ranking[index].datos.nombre);
                MessageBox(hwnd, texto, "Seleccion", MB_OK);
            }
        }

        if (LOWORD(wParam) == BTN_REGRESAR_3)
        {
            if (ranking)
            {
                free(ranking);
                ranking = NULL;
            }
            MostrarVentana(hwndFinal, hwnd);
        }
        break;

    case WM_DESTROY:
        if (ranking)
        {
            free(ranking);
            ranking = NULL;
        }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK VentanaJuego(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int tablero[4][6];
    static int primerSeleccion = -1;
    static int segundoSeleccion = -1;
    static int contador = 0;
    static int puntaje = 0;
    static int encontradas = 0;
    static BOOL modoDificil = TRUE;

    static HBITMAP imagenTapada;
    static HBITMAP imagenesCartas[12];

    switch (msg)
    {
    case WM_CREATE:
    {
        int x = 30, y = 30;
        int ancho = 40, alto = 40;
        int margen = 10;

        // Cargar imagen tapada
        imagenTapada = (HBITMAP)LoadImageW(NULL, L"tapada.bmp", IMAGE_BITMAP, ancho, alto, LR_LOADFROMFILE);

        // Cargar im�genes de las cartas
        for (int i = 0; i < 12; i++)
        {
            wchar_t ruta[64];
            swprintf(ruta, 64, L"imagen%i.bmp", (i + 1));
            imagenesCartas[i] = (HBITMAP)LoadImageW(NULL, ruta, IMAGE_BITMAP, ancho, alto, LR_LOADFROMFILE);
        }

        // Crear botones con imagen tapada
        for (int i = 0; i < TOTAL_CARTAS; i++)
        {
            botonesJuego[i] = CreateWindow("BUTTON", NULL,
                                           WS_VISIBLE | WS_CHILD | BS_BITMAP,
                                           x, y, ancho, alto, hwnd, (HMENU)(INT_PTR)(200 + i),
                                           ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            SendMessage(botonesJuego[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagenTapada);
            EnableWindow(botonesJuego[i], FALSE);

            x += ancho + margen;
            if ((i + 1) % 6 == 0)
            {
                x = 30;
                y += alto + margen;
            }
        }

        CreateWindow("STATIC", "Puntaje:", WS_VISIBLE | WS_CHILD,
                     400, 80, 60, 20, hwnd, NULL, NULL, NULL);
        txtPuntaje = CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | ES_READONLY,
                                  470, 80, 60, 20, hwnd, NULL, NULL, NULL);

        CreateWindow("BUTTON", "Regresar", WS_VISIBLE | WS_CHILD,
                     400, 20, 100, 30, hwnd, (HMENU)300, NULL, NULL);
        CreateWindow("BUTTON", "Iniciar", WS_VISIBLE | WS_CHILD,
                     400, 150, 100, 30, hwnd, (HMENU)4, NULL, NULL);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == 4)
        { // Iniciar
            if (!cargar(hwnd, tablero, &puntaje, &encontradas))
            {
                generarMatriz24(tablero);

                primerSeleccion = -1;
                segundoSeleccion = -1;
                contador = 0;
                puntaje = 0;
                encontradas = 0;

                SetWindowText(txtPuntaje, "0");

                for (int i = 0; i < TOTAL_CARTAS; i++)
                {
                    SendMessage(botonesJuego[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagenTapada);
                    EnableWindow(botonesJuego[i], TRUE);
                }

                HWND botonIniciar = GetDlgItem(hwnd, 4);
                EnableWindow(botonIniciar, FALSE);
            }
            else
            {
                printf("TOTAL ENCONTRADAS EN ARCHIVO: %d", encontradas);
            }

            primerSeleccion = -1;
            segundoSeleccion = -1;
            contador = 0;
        }

        else if (LOWORD(wParam) == 300)
        { // Regresar
            HWND botonIniciar = GetDlgItem(hwnd, 4);

            if (!IsWindowEnabled(botonIniciar))
            {
                guardarPartida(tablero, botonesJuego, puntaje);
                ReiniciarJuego(hwnd, tablero, &puntaje, &encontradas);
            }

            MostrarVentana(hwndFinal, hwnd);
        }

        else if (LOWORD(wParam) >= 200 && LOWORD(wParam) < 200 + TOTAL_CARTAS)
        {
            int indice = LOWORD(wParam) - 200;
            int fila = indice / 6;
            int columna = indice % 6;

            int valor = tablero[fila][columna];
            SendMessage(botonesJuego[indice], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagenesCartas[valor - 1]);

            if (contador == 0)
            {
                primerSeleccion = indice;
                contador = 1;

                if (modoDificil)
                {
                    SetTimer(hwnd, 2, 800, NULL);
                }
            }
            else if (contador == 1 && indice != primerSeleccion)
            {
                if (modoDificil)
                {
                    KillTimer(hwnd, 2);
                }

                segundoSeleccion = indice;
                contador = 2;
                SetTimer(hwnd, 1, 1000, NULL);
            }
        }
        break;

    case WM_TIMER:
        if (wParam == 1)
        {
            KillTimer(hwnd, 1);

            int fila1 = primerSeleccion / 6;
            int col1 = primerSeleccion % 6;
            int fila2 = segundoSeleccion / 6;
            int col2 = segundoSeleccion % 6;

            if (tablero[fila1][col1] == tablero[fila2][col2])
            {
                EnableWindow(botonesJuego[primerSeleccion], FALSE);
                EnableWindow(botonesJuego[segundoSeleccion], FALSE);

                puntaje += 10;
                encontradas++;

                char nuevoPuntaje[10];
                sprintf(nuevoPuntaje, "%d", puntaje);
                SetWindowText(txtPuntaje, nuevoPuntaje);

                if (encontradas == 12)
                {
                    borrarArchivo();
                    USUARIOACTUAL.puntajeMaximo += puntaje;
                    modificarPuntuacion();

                    MostrarVentana(hwndFinal, hwnd);
                    ReiniciarJuego(hwnd, tablero, &puntaje, &encontradas);

                    MessageBox(NULL, "Felicidades", "!Ganaste!", MB_OK | MB_ICONINFORMATION);
                    MostrarVentana(hwndFinal, hwnd);
                }
            }
            else
            {
                SendMessage(botonesJuego[primerSeleccion], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagenTapada);
                SendMessage(botonesJuego[segundoSeleccion], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagenTapada);
            }

            primerSeleccion = -1;
            segundoSeleccion = -1;
            contador = 0;
        }

        else if (wParam == 2)
        {
            KillTimer(hwnd, 2);

            if (contador == 1 && primerSeleccion != -1)
            {
                SendMessage(botonesJuego[primerSeleccion], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagenTapada);
                primerSeleccion = -1;
                contador = 0;
            }
        }
        break;

    case WM_CLOSE:
        guardarPartida(tablero, botonesJuego, puntaje);
        cerrarPrograma();
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
