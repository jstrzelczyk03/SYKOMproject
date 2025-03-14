#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024
#define REG_A "/proc/sykom/rejstrjakA"  // Sciezka do rejestru A
#define REG_W "/proc/sykom/rejstrjakW"  // Sciezka do rejestru W
#define REG_S "/proc/sykom/rejstrjakS"  // Sciezka do rejestru S

// Funkcja do odczytu wartosci z pliku
unsigned int read_from_file(const char *);
int write_to_file(const char *, unsigned int);
void test_module();
unsigned int calculate_prime(unsigned int a);

int main(void)
{
    test_module();
    return 0;
}

// Funkcja do odczytu wartosci z pliku
unsigned int read_from_file(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error opening file: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    
    char buffer[MAX_BUFFER];
    size_t n = fread(buffer, 1, MAX_BUFFER - 1, file);
    if (n == 0) {
        if (feof(file)) {
            printf("End of file reached.\n");
        } else {
            printf("Error reading from file\n");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
    buffer[n] = '\0';
    fclose(file);
    
    unsigned int result;
    if (sscanf(buffer, "%o", &result) != 1) {  // %o dla formatu oktalnego
        printf("Error parsing file content\n");
        exit(EXIT_FAILURE);
    }
    
    return result;
}

// Funkcja do zapisu wartosci do pliku
int write_to_file(const char *filePath, unsigned int input) {
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error opening file: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (fprintf(file, "%o", input) < 0) {  // %o dla formatu oktalnego
        printf("Error writing to file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return 0;
}

// Funkcja do obliczenia liczby pierwszej na podstawie wartosci a
unsigned int calculate_prime(unsigned int a) {
    write_to_file(REG_A, a);  // Zapisujemy wartosc a do rejestru A
    unsigned int status;
    unsigned int result;
    
    // Petla oczekujaca na zakonczenie obliczen
    while (1) {
        status = read_from_file(REG_S);  // Odczyt statusu z rejestru S
        if (status == 2) {  // Jesli status wskazuje zakonczenie obliczen
            result = read_from_file(REG_W);  // Odczyt wyniku z rejestru W
            return result;
        }
        usleep(10000); // Opoznienie 10ms
    }
    return 0;
}

// Funkcja testujaca modul
void test_module() {
    unsigned int a[] = {1, 2, 7, 10, 28, 100, 255, 500, 750, 850, 1000};  // Testowane wartosci a
    unsigned int results[] = {02, 03, 021, 035, 0153, 01035, 03135, 07043, 013501, 015435, 017537};  // Oczekiwane wyniki w formacie oktalnym
    unsigned int wynik;
    
    // Petla wykonujaca testy dla kazdej wartosci a
    for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
        wynik = calculate_prime(a[i]);  // Obliczenie liczby pierwszej dla danej wartosci a
        if (wynik == results[i]) {
            printf("TEST PASSED for N=%u: expected=%o, got=%o\n", a[i], results[i], wynik);  // Sukces testu
        } else {
            printf("TEST FAILED for N=%u: expected=%o, got=%o\n", a[i], results[i], wynik);  // Niepowodzenie testu
        }
    }
    printf("Tests completed.\n");
}
