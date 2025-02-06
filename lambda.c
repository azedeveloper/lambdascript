#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

void InterpretBinary(wchar_t *input); 
void LambdaToBinary(wchar_t *input);

void LambdaToBinary(wchar_t *input) {
    size_t len = wcslen(input);
    for (size_t i = 0; i < len; i++) {
        if (input[i] == L'Λ') { 
            input[i] = L'1';
        } else if (input[i] == L'λ') { 
            input[i] = L'0';
        }
    }
    InterpretBinary(input); 
}

void InterpretBinary(wchar_t *input) {
    wchar_t binaryChunk[9]; 
    size_t len = wcslen(input);
    size_t index = 0;

    for (size_t i = 0; i < len; i++) {
        if (!iswspace(input[i])) { 
            binaryChunk[index++] = input[i];
            if (index == 8) {
                binaryChunk[8] = L'\0'; 
                int asciiValue = wcstol(binaryChunk, NULL, 2); 
                wprintf(L"%c", (char)asciiValue); 
                index = 0; 
            }
        }
    }
    wprintf(L"\n");
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    size_t bufferSize = 1024;
    wchar_t *binaryString = malloc(bufferSize * sizeof(wchar_t));
    if (binaryString == NULL) {
        perror("Memory allocation failed");
        fclose(fptr);
        return 1;
    }

    size_t length = 0;
    wint_t ch;
    while ((ch = fgetwc(fptr)) != WEOF) {
        if (length + 1 >= bufferSize) {
            bufferSize *= 2;
            wchar_t *newBuffer = realloc(binaryString, bufferSize * sizeof(wchar_t));
            if (newBuffer == NULL) {
                perror("Memory reallocation failed");
                free(binaryString);
                fclose(fptr);
                return 1;
            }
            binaryString = newBuffer;
        }
        binaryString[length++] = (wchar_t)ch;
    }
    binaryString[length] = L'\0';
    fclose(fptr);

    LambdaToBinary(binaryString);

    free(binaryString);
    return 0;
}
