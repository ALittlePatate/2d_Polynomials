#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
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
    if (poly == NULL) {
        return FAIL;
    }

    *out = poly->a * x * x + poly->b * x + poly->c;
    return OK;
}

enum STATUS Dessiner_Graph(Poly* poly) {
    if (poly == NULL) {
        return FAIL;
    }

    printf("\n");
    printf("+-----+---------------------------------------------+\n");
    for (int j = LONGUEUR_Y; j >= -LONGUEUR_Y; j--) {

        if (j == -10) {
            printf("| %d |  ",j);
        }
        else if (j == 10 || j < 0) {
            printf("| %d  |  ",j);
        }
        else if (j == 0) {
            printf("| %d   |--",j);
        }
        else {
            printf("| %d   |  ",j);
        }

        for (int i = -LONGUEUR_X; i <= LONGUEUR_X+1; i++) {
            if (i > LONGUEUR_X) {
                if (j == 0) {
                    printf("-|");
                }
                else {
                    printf(" |");
                }
                continue;
            }

            float y;
            if (Calcul_Fonction(poly, i, &y) == FAIL) {
                return FAIL;
            }

            if (roundf(y) == j) {
                printf("o ");
            }
            else if (i == 0) {
                if (j == 0) {
                    printf("+-");
                }
                else {
                    printf(": ");
                }
            }
            else if (roundf(y) != j && j != 0) {
                printf("  ");
            }
            else {
                printf("--");
            }
        }
        printf("\n");
    }

    //            |  10 9 8 7 6 5 4 3 2 1 0 1 2 3 4 5 6 7 8 9 10
    printf("+-----+---------------------------------------------+\n");
    printf("|     | ");
    for (int k = -LONGUEUR_Y; k <= LONGUEUR_Y; k++) {
        printf("%d ",abs(k));
    }
    printf("|\n");
    printf("+-----+---------------------------------------------+\n");

    return OK;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "%s", "[-] usage : poly.exe a b c\n");
        return 1;
    }

    float a = atof(argv[1]);
    float b = atof(argv[2]);
    float c = atof(argv[3]);

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

    free(poly);
    return 0;
}