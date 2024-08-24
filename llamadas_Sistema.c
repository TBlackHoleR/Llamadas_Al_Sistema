#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hFile;
    DWORD dwBytesWritten = 0;
    DWORD dwBytesRead = 0;
    char writeData[] = "Hola, mundo desde C!";
    char readData[30] = {0};

    // 1. Crear un archivo y escribir datos en él
    hFile = CreateFile(
        "archivo.txt",               // Nombre del archivo
        GENERIC_WRITE,               // Permisos de escritura
        0,                           // No compartir
        NULL,                        // Seguridad predeterminada
        CREATE_ALWAYS,               // Crear nuevo archivo o sobrescribir si existe
        FILE_ATTRIBUTE_NORMAL,       // Atributos del archivo
        NULL);                       // Sin plantilla de archivo

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("No se pudo crear el archivo.\n");
        return 1;
    }

    if (!WriteFile(hFile, writeData, sizeof(writeData), &dwBytesWritten, NULL)) {
        printf("Error al escribir en el archivo.\n");
        CloseHandle(hFile);
        return 1;
    }

    CloseHandle(hFile);

    // 2. Leer datos de un archivo
    hFile = CreateFile(
        "archivo.txt",               // Nombre del archivo
        GENERIC_READ,                // Permisos de lectura
        0,                           // No compartir
        NULL,                        // Seguridad predeterminada
        OPEN_EXISTING,               // Abrir el archivo existente
        FILE_ATTRIBUTE_NORMAL,       // Atributos del archivo
        NULL);                       // Sin plantilla de archivo

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("No se pudo abrir el archivo para lectura.\n");
        return 1;
    }

    if (!ReadFile(hFile, readData, sizeof(readData) - 1, &dwBytesRead, NULL)) {
        printf("Error al leer el archivo.\n");
        CloseHandle(hFile);
        return 1;
    }

    printf("Contenido leído del archivo: %s\n", readData);

    CloseHandle(hFile);

    // 3. Crear un proceso hijo
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Aquí se crea el proceso hijo. Por ejemplo, lanzamos el Bloc de notas
    if (!CreateProcess(
        NULL,                // Nombre de la aplicación
        "notepad.exe",       // Línea de comandos
        NULL,                // Atributos de seguridad para el proceso
        NULL,                // Atributos de seguridad para el hilo
        FALSE,               // No heredar handles
        0,                   // Banderas de creación
        NULL,                // Usar ambiente del proceso padre
        NULL,                // Usar directorio actual del proceso padre
        &si,                 // Información de inicio
        &pi))                // Información del proceso
    {
        printf("Fallo en la creación del proceso hijo.\n");
        return 1;
    }

    // 4. Esperar a que el proceso hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);

    printf("El proceso hijo ha terminado.\n");

    // Cerrar handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
