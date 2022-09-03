typedef struct Poly {
    float a;
    float b;
    float c;

    float delta;

    float sx; //sommet x
    float sy; //sommet y

    float x1;
    float x2;
    float x0;
} Poly;

enum STATUS {
    OK,
    FAIL
};

enum STATUS Calcul_Delta(Poly* poly);
enum STATUS Calcul_x1_x2(Poly* poly);
enum STATUS Calcul_x0(Poly* poly);
enum STATUS Calcul_Sommet(Poly* poly);
enum STATUS Calcul_Fonction(Poly* poly, float x, float *out);
enum STATUS Calcul_Derivee(Poly* poly, float x, float *out);

int LONGUEUR_X_MAX = 20;
int LONGUEUR_Y_MAX = 20;
int LONGUEUR_Y_MIN = -20;
int LONGUEUR_X_MIN = -20;
int LONGUEUR_X_TAB = 20;
int LONGUEUR_Y_TAB = 20;
void Dessiner_Separateur();
int use_colors = false;
void Dessiner_Actualiser(Poly* poly);
enum STATUS Dessiner_Graph(Poly* poly);
int main();