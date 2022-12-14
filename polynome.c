#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <stdbool.h>

#include "polynome.h"

enum STATUS Calcul_Delta(Poly* poly) {
    if (poly == NULL) {
        return FAIL;
    }

    poly->delta = (poly->b * poly->b) - (4 * poly->a * poly->c);
    return OK;
}

enum STATUS Calcul_x1_x2(Poly* poly) {
    if (poly == NULL) {
        return FAIL;
    }

    poly->x1 = (-poly->b - sqrt(poly->delta)) / (2 * poly->a);
    poly->x2 = (-poly->b + sqrt(poly->delta)) / (2 * poly->a);

    return OK;
}

enum STATUS Calcul_x0(Poly* poly) {
    if (poly == NULL) {
        return FAIL;
    }

    poly->x0 = -poly->b / (2 * poly->a);

    return OK;
}

enum STATUS Calcul_Sommet(Poly* poly) {
    if (poly == NULL) {
        return FAIL;
    }

    poly->sx = -poly->b / (2 * poly->a);

    float result;
    if (Calcul_Fonction(poly, poly->sx, &result) == FAIL) {
        return FAIL;
    }

    poly->sy = result;

    return OK;
}

enum STATUS Calcul_Fonction(Poly* poly, float x, float *out) {
    if (poly == NULL || out == NULL) {
        return FAIL;
    }

    *out = poly->a * x * x + poly->b * x + poly->c;
    return OK;
}

enum STATUS Calcul_Derivee(Poly* poly, float x, float *out) {
    //f'(x)=2ax+b

    if (poly == NULL || out == NULL) {
        return FAIL;
    }

    *out = (2 * poly->a * x) + poly->b;
    return OK;
}

void Dessiner_Separateur() {
    //1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21
    //1 2 3 4 5 6
    //xmin = -5
    //xmax = 15
    printf("+-----+");
    for (int i = 0; i <= LONGUEUR_X_TAB*2; i++) {
        if (i == LONGUEUR_X_TAB*2 - LONGUEUR_X_MAX) {
            printf("--+-");
        }
        else {
            printf("----");
        }
    }
    printf("-+\n");
}

void Dessiner_Actualiser(Poly* poly) {
    system("cls");
    Dessiner_Graph(poly);
    printf("\nPress [ESC] to exit.");
}

enum STATUS Dessiner_Graph(Poly* poly) {
    if (poly == NULL) {
        return FAIL;
    }

    printf("\n");
    Dessiner_Separateur();
    for (int j = LONGUEUR_Y_MAX; j >= LONGUEUR_Y_MIN; j--) {

        if (j < -9) {
            printf("| %d | ",j);
        }
        else if (j > 9) {
            printf("| 0%d | ",j);
        }
        else if (j < 0 && j > -10) {
            printf("| -0%d | ",abs(j));
        }
        else if (j == 0) {
            printf("| 00%d |-",j);
        }
        else {
            printf("| 00%d | ",j);
        }

        for (int i = LONGUEUR_X_MIN; i <= LONGUEUR_X_MAX+1; i++) {
            if (i > LONGUEUR_X_MAX) {
                printf("|");
                continue;
            }

            float y;
            if (Calcul_Fonction(poly, i, &y) == FAIL) {
                return FAIL;
            }

            float y2;
            if (Calcul_Derivee(poly, i, &y2) == FAIL) {
                return FAIL;
            }

            //wow, much colors ! https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
            if (roundf(y) == j) {
                if (j == 0) {
                    if (use_colors) {
                        printf("-\033[36m\033[1mo\033[0m--");
                    }
                    else {
                        printf("-o--");
                    }
                }
                else {
                    if (use_colors) {
                        printf("\033[36m\033[1m o  \033[0m");
                    }
                    else {
                        printf(" o  ");
                    }
                }
                continue;
            }
            if (roundf(y2) == j) {
                if (j == 0) {
                    if (use_colors) {
                        printf("-\033[92m\033[1mu\033[0m--");
                    }
                    else {
                        printf("-u--");
                    }
                }
                else {
                    if (use_colors) {
                        printf("\033[92m\033[1m u  \033[0m");
                    }
                    else {
                        printf(" u  ");
                    }
                }
                continue;
            }

            if (i == 0) {
                if (j == 0) {
                    printf("-+--");
                }
                else {
                    printf(" :  ");
                }
            }
            else if (roundf(y) != j && j != 0) {
                printf("    ");
            }
            else {
                printf("----");
            }
        }
        printf("\n");
    }

    //            |  10 9 8 7 6 5 4 3 2 1 0 1 2 3 4 5 6 7 8 9 10
    Dessiner_Separateur();
    printf("|     | ");

    for (int k = LONGUEUR_X_MIN; k <= LONGUEUR_X_MAX; k++) {
        if (k < 0) {
            printf("-");
        }
        else {
            printf("0");
        }

        if (k < 10 && k > -10) {
            printf("0%d ",abs(k));
        }
        else if (k < 100 && k > -100) {
            printf("%d ",abs(k));
        }
    }
    printf("|\n");
    Dessiner_Separateur();

    return OK;
}

int main(int argc, char* argv[]) {
    system("cls");

    if (argc < 4) {
        fprintf(stderr, "%s", "[-] usage : poly.exe a b c [--size 20] [--colors]\n");
        return 1;
    }

    float a = atof(argv[1]);
    float b = atof(argv[2]);
    float c = atof(argv[3]);

    if (argc > 4) {
        if (strcmp(argv[4], "--size") == 0) {
            if (argc >= 5) {
                LONGUEUR_X_MAX = strtol(argv[5], NULL, 10);
                LONGUEUR_Y_MAX = LONGUEUR_X_MAX;
                LONGUEUR_Y_MIN = -LONGUEUR_X_MAX;
                LONGUEUR_X_MIN = -LONGUEUR_X_MAX;
                LONGUEUR_X_TAB = LONGUEUR_X_MAX;
                LONGUEUR_Y_TAB = LONGUEUR_X_MAX;

                if (LONGUEUR_X_MAX >= 100) {
                    fprintf(stderr, "%s", "[-] Size can't be greater than 99.\n");
                    exit(1);
                }
            }
        }
        
        if (strcmp(argv[4], "--colors") == 0) {
            use_colors = true;
        }
        if (argc >= 7) {
            if (strcmp(argv[6], "--colors") == 0) {
                use_colors = true;
            }
        }
    }

    printf("[+] Processing %.3fx^2%.3fx%.3f\n", a, b, c);

    Poly* poly = malloc(sizeof(Poly));
    poly->a = a;
    poly->b = b;
    poly->c = c;

    if (Calcul_Delta(poly) == FAIL) {
        fprintf(stderr, "%s", "[-] Error while calculating delta.\n");
    }
    else {
        printf("[+] Delta : %.3f\n",poly->delta);
    }

    if (poly->delta > 0) {
        printf("[+] The polynomial admits 2 solutions x1 and x2.\n");
        
        if (Calcul_x1_x2(poly) == OK) {
            printf("[+] x1 : %.4f\n",poly->x1);
            printf("[+] x2 : %.4f\n",poly->x2);
        }
        else {
            fprintf(stderr, "%s", "[-] Error while calculating x1 or x2.\n");
        }
    }
    else if (poly->delta == 0) {
        printf("[+] The polynomial admits 1 solution x0\n");

        if (Calcul_x0(poly) == OK) {
            printf("[+] x0 : %.4f\n",poly->x0);
        }
        else {
            fprintf(stderr, "%s", "[-] Error while calculating x0.\n");
        }
    }
    else {
        printf("[+] The polynomial has no solution\n");
    }

    if (Calcul_Sommet(poly) == FAIL) {
        fprintf(stderr, "%s", "[-] Error while calculating vertex coordinates.\n");
    }
    else {
        printf("[+] Vertex coordinates : S = (%.4f,%.4f)\n",poly->sx,poly->sy);
    }

    printf("[+] Drawing the graph...\n");
    if (Dessiner_Graph(poly) == FAIL) {
        fprintf(stderr, "%s", "[-] Error while drawing the graph.\n");
    }

    printf("\nPress [ESC] to exit.");
    while (1) {
        Sleep(50);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            LONGUEUR_X_MAX += 5;
            LONGUEUR_X_MIN = LONGUEUR_X_MAX - LONGUEUR_X_TAB*2;
            Dessiner_Actualiser(poly);
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            LONGUEUR_X_MAX -= 5;
            LONGUEUR_X_MIN = LONGUEUR_X_MAX - LONGUEUR_X_TAB*2;
            Dessiner_Actualiser(poly);
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            LONGUEUR_Y_MAX += 5;
            LONGUEUR_Y_MIN = LONGUEUR_Y_MAX - LONGUEUR_Y_TAB*2;
            Dessiner_Actualiser(poly);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            LONGUEUR_Y_MAX -= 5;
            LONGUEUR_Y_MIN = LONGUEUR_Y_MAX - LONGUEUR_Y_TAB*2;
            Dessiner_Actualiser(poly);
        }
    }

    free(poly);
    return 0;
}