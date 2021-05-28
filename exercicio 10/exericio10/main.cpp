#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>

#define tamLinha 860
#define tamCampoId 30
#define tamCampoName 200
#define tamCampoKey 15
#define numArtistas 40

int qtd = 0;

typedef struct tipoData {
    int year;
    char *month;
    char *day;
} tipoData;

typedef struct tipoArtista {
    int numArtists;
    char **nameArtists;
} tipoArtista;

typedef struct tipoMusica {

    char* id;
    char* name;             // nome
    char* key;              // chave
    tipoArtista *artists;   // lista de artistas
    tipoData *release_date; // data de lançamento
    double acousticness;      // acústica
    double danceability;      // dança
    double energy;            // energia
    int duration_ms;         // duração, em milissegundos
    double instrumentalness;  // instrumentalidade
    double valence;           // valência
    int popularity;          // popularidade
    double time;              // tempo
    double liveness;          // vivacidade
    double loudness;          // volume
    double speechiness;       // discurso
    int year;                // ano
} tipoMusica;

typedef struct Celula* ApontadorCelula;

typedef struct Celula {
  tipoMusica *item;
  ApontadorCelula proximo;
} Celula;

typedef struct Fila {
  ApontadorCelula frente;
  ApontadorCelula tras;
} Fila;

char* getToken (char dados[], int tamCampo, int *i) {


    char *campo;
    int j = 0;

    campo = (char *) malloc(tamCampo * sizeof(char));

    while ((*i < strlen(dados) && (dados[*i] != ','))) {
        campo[j] = dados[*i];
        j++;
        (*i)++;
    }
    (*i)++;
    campo[j] = '\0';
    return campo;
}
tipoArtista* getArtistas (char dados[], int *i) {

    int j, k, index;
    char *campo;
    char **nomesArtistas;
    tipoArtista *artistas;
    int inicio = 0;
    int fim;

    campo = (char *) malloc(tamCampoName * numArtistas * sizeof(char));
    artistas = (tipoArtista *) malloc (sizeof (tipoArtista));

    while (dados[*i] != '[') // percorrendo a string até encontrar o início da lista de artistas.
        (*i)++;

    (*i)++;

    j = 0;
    while (dados[*i] != ']') { // percorrendo a string até encontrar o final da lista de artistas.
        campo[j] = dados[*i];
        (*i)++;
        j++;
    }

    campo[j] = '\0';

    while (dados[*i] != ',') // percorrendo a string até encontrar o separador para o próximo campo.
        (*i)++;

    (*i)++;

    nomesArtistas = (char **) malloc (numArtistas * sizeof(char *));
    for (k = 0; k < numArtistas; k++)
        nomesArtistas[k] = (char *) malloc (tamCampoName * sizeof(char));

    index = 0;
    for (j = 0, k = 0; j < strlen(campo)-1; j++) {
        if ((campo[j] == '\'') && (campo[j+1] == ',')) { // separador entre artistas: ',
            nomesArtistas[k][index] = '\0';
            k++;
            index = 0;
            j++;
        } else {
            nomesArtistas[k][index] = campo[j];
            index++;
        }
    }
    nomesArtistas[k][index] = campo[j];
    nomesArtistas[k][index + 1] = '\0';

    artistas->numArtists = k+1;
    artistas->nameArtists = (char **) malloc (artistas->numArtists * sizeof(char *));

    for (j = 0; j < artistas->numArtists; j++) {

        campo = (char *) malloc(tamCampoName * sizeof(char));
        inicio = 0;
        while ((nomesArtistas[j][inicio] == '\'') ||
               (nomesArtistas[j][inicio] == ' ')) // percorrendo até encontrar o início do nome do artista.
            inicio++;

        for (fim = strlen(nomesArtistas[j]) - 1; nomesArtistas[j][fim] == '\''; fim--);

        index = 0;
        while (inicio <= fim) {
            campo[index] = nomesArtistas[j][inicio];
            inicio++;
            index++;
        }

        campo[index] = '\0';
        artistas->nameArtists[j] = campo;

    }

    for (k = 0; k < numArtistas; k++)
        free(nomesArtistas[k]);
    free(nomesArtistas);

    return artistas;
}

char* getNome(char dados[], int *i) {

    char *campo;
    int index;
    bool terminou = false;

    campo = (char *) malloc (tamCampoName * sizeof(char));

    (*i)+=2; // percorrendo a string até encontrar o início do texto.

    index = 0;
    while (!terminou) {
        if ((dados[*i] == ']') && (dados[(*i) + 1] == '\"') && (dados[(*i) + 2] == ','))
            terminou = true;
        else {
            campo[index] = dados[*i];
            (*i)++;
            index++;
        }
    }

    (*i)+=3;
    campo[index] = '\0';

    return campo;
}

tipoData* getData(char *dados) {

    tipoData *data = (tipoData *) malloc (sizeof(tipoData));
    char *campo = (char *) malloc (4 * sizeof(char));
    int index = 0;
    int numCampos = 0;

    for (int j = 0; j < strlen(dados); j++) {
        if (dados[j] != '-') {
            campo[index] = dados[j];
            index++;
        } else {
            campo[index] = '\0';
            if (numCampos == 0)
                data->year = atoi(campo);
            else
                data->month = campo;

            campo = (char *) malloc (4 * sizeof(char));
            numCampos++;
            index = 0;
        }
    }

    campo[index] = '\0';

    if (numCampos == 0) {
        data->year = atoi(campo);
        campo = (char *) malloc (2 * sizeof(char));
        strcpy(campo, "01");
        data->month = campo;
        campo = (char *) malloc (2 * sizeof(char));
        strcpy(campo, "01");
        data->day = campo;
    } else
        data->day = campo;

    return data;
}

tipoMusica criarMusica(char dados[]) {

    tipoMusica novaMusica;
    int i;

    i = 0;

    novaMusica.valence = atof(getToken(dados, tamCampoId, &i));
    novaMusica.year = atoi(getToken(dados, 4, &i));
    novaMusica.acousticness = atof(getToken(dados, tamCampoKey, &i));
    novaMusica.artists = getArtistas(dados, &i);
    novaMusica.danceability = atof(getToken(dados, tamCampoKey, &i));
    novaMusica.duration_ms = atoi(getToken(dados, tamCampoKey, &i));
    novaMusica.energy = atof(getToken(dados, tamCampoKey, &i));

    getToken(dados, tamCampoKey, &i); // saltar campo da string original que não será utilizado

    novaMusica.id = getToken(dados, tamCampoId, &i);
    novaMusica.instrumentalness = atof(getToken(dados, tamCampoKey, &i));
    novaMusica.key = getToken(dados, tamCampoKey, &i);
    novaMusica.liveness = atof(getToken(dados, tamCampoKey, &i));
    novaMusica.loudness = atof(getToken(dados, tamCampoKey, &i));

    getToken(dados, tamCampoKey, &i); // saltar campo da string original que não será utilizado

    novaMusica.name = getNome(dados, &i);

    novaMusica.popularity = atoi(getToken(dados, tamCampoKey, &i));

    novaMusica.release_date = getData(getToken(dados, 10, &i));

    novaMusica.speechiness = atof(getToken(dados, tamCampoKey, &i));
    novaMusica.time = atof(getToken(dados, tamCampoKey, &i));

    return novaMusica;
}

tipoMusica *clone(tipoMusica musica) {

    tipoMusica *copia = (tipoMusica *) malloc (sizeof(tipoMusica));
    tipoArtista *artistas;
    tipoData *data;

    copia->id = (char *) malloc (tamCampoId * sizeof(char));
    strcpy(copia->id, musica.id);

    copia->name = (char *) malloc (tamCampoName * sizeof(char));
    strcpy(copia->name, musica.name);

    copia->key = (char *) malloc (tamCampoKey * sizeof(char));
    strcpy(copia->key, musica.key);

    artistas = (tipoArtista *) malloc (sizeof(tipoArtista));
    artistas->numArtists = musica.artists->numArtists;
    artistas->nameArtists = (char **) malloc (artistas->numArtists * sizeof (char *));
    for (int i = 0; i < artistas->numArtists; i++) {
        artistas->nameArtists[i] = (char *) malloc (tamCampoName * sizeof(char));
        strcpy(artistas->nameArtists[i], musica.artists->nameArtists[i]);
    }
    copia->artists = artistas;

    data = (tipoData *) malloc (sizeof(tipoData));
    data->year = musica.release_date->year;

    data->month = (char *) malloc (2 * sizeof(char));
    strcpy(data->month, musica.release_date->month);

    data->day = (char *) malloc (2 * sizeof(char));
    strcpy(data->day, musica.release_date->day);

    copia->release_date = data;

    copia->acousticness = musica.acousticness;
    copia->danceability = musica.danceability;
    copia->energy = musica.energy;
    copia->duration_ms = musica.duration_ms;
    copia->instrumentalness = musica.instrumentalness;
    copia->valence = musica.valence;
    copia->popularity = musica.popularity;
    copia->time = musica.time;
    copia->liveness = musica.liveness;
    copia->loudness = musica.loudness;
    copia->speechiness = musica.speechiness;
    copia->year = musica.year;

    return copia;
}

void imprimirMusica(tipoMusica musica) {
    printf("%s ## [", musica.id);
    for (int j = 0; j < musica.artists->numArtists - 1; j++) {
        printf("%s, ", musica.artists->nameArtists[j]);
    }
    printf("%s] ## %s ## ", musica.artists->nameArtists[musica.artists->numArtists - 1], musica.name);
    printf("%s/%s/%d", musica.release_date->day, musica.release_date->month, musica.release_date->year);
    printf(" ## %lg ## %lg ## %lg ## %lg ## %lg ## %lg ## %lg ## %d\n", musica.acousticness, musica.danceability, musica.instrumentalness, musica.liveness, musica.loudness, musica.speechiness, musica.energy, musica.duration_ms);
}

tipoMusica *buscarId(char **listaMusicas, int numMusicas, char *id) {

    int i;
    bool encontrou = false;
    tipoMusica *musica;

    for (i = 0; i < numMusicas && (!encontrou); i++)
        if (strstr(listaMusicas[i], id) != NULL)
            encontrou = true;

    if (encontrou) {
        musica = (tipoMusica *) malloc (sizeof(tipoMusica));

        *musica = criarMusica(listaMusicas[i-1]);
        return musica;
    }
    return NULL;
}

int contarTotalMusicas() {
    int numMusicas = 0;
    FILE *arquivoMusicas;
    char musica[tamLinha];

    // Abrindo o arquivo
    arquivoMusicas = fopen("/tmp/dataAEDs.csv", "r");

    if(arquivoMusicas == NULL){
        printf("Erro ao tentar abrir o arquivo!\n");
        return 0;
    }

    // Lendo uma linha inteira do arquivo: todos os dados de uma música
    fscanf(arquivoMusicas, " %[^\n]", musica);
    while (!(feof(arquivoMusicas))) {
        numMusicas++;
        // Lendo uma linha inteira do arquivo: todos os dados de uma música
        fscanf(arquivoMusicas, " %[^\n]", musica);
    }

    // Fechando o arquivo
    fclose(arquivoMusicas);

    return numMusicas;
}

void lerMusicas(char **listaMusicas) {

    FILE *arquivoMusicas;
    char dadosMusica[tamLinha];
    int i = 0;

    // Abrindo o arquivo
    arquivoMusicas = fopen("/tmp/dataAEDs.csv", "r");

    // Lendo uma linha inteira do arquivo: dados de cabeçalho
    fscanf(arquivoMusicas, " %[^\n]", dadosMusica);

    // Lendo uma linha inteira do arquivo: todos os dados de uma música
    fscanf(arquivoMusicas, " %[^\n]", dadosMusica);
    while (!(feof(arquivoMusicas))) {
        strcpy(listaMusicas[i], dadosMusica);
        i++;
        // Lendo uma linha inteira do arquivo: todos os dados de uma música
        fscanf(arquivoMusicas, " %[^\n]", dadosMusica);
    }

    // Fechando o arquivo
    fclose(arquivoMusicas);
}

int lerIds(char **listaIds) {

    char id[tamCampoId];
    int i;

    scanf(" %[^\n]", id);
    i = 0;

    while (strcmp(id, "FIM")){
	strcpy(listaIds[i], id);
    	scanf(" %[^\n]", id);
	i++;
    }

    return i;
}

int main() {

    Fila *fila;
    int media;
    int numOps;
    char * opId;

    tipoMusica *desenfileirado;

    int numTotalMusicas = contarTotalMusicas() - 1;
    char **listaTodasMusicas;
    char **listaIds;
    tipoMusica *listaMusicas;
    int i;
    int numIds;
    tipoMusica *pesquisado;

    fila = criarFila();

    listaTodasMusicas = (char **) malloc (numTotalMusicas * sizeof (char *));
    for (i = 0; i < numTotalMusicas; i++)
        listaTodasMusicas[i] = (char *) malloc (tamLinha * sizeof(char));

    lerMusicas(listaTodasMusicas);

    listaIds = (char **) malloc (numTotalMusicas * sizeof (char *));
    for (i = 0; i < numTotalMusicas; i++)
        listaIds[i] = (char *) malloc (tamCampoId * sizeof(char));

    numIds = lerIds(listaIds);

    listaMusicas = (tipoMusica *) malloc (numIds * sizeof(tipoMusica));

    for(i = 0; strcmp(listaIds[i],"FIM")==0; i++){
        pesquisado = buscarId(listaTodasMusicas, numTotalMusicas, listaIds[i]);
        enfileira(fila,pesquisado);
        media = obtemMediaDuration(fila);
        printf("%d\n",media);
    }

    i++;

    numOps = atoi(listaIds[i]);

    i++;

    for(int contador = 0; contador < numOps; contador++){
        opId = strtok(listaIds[i]," ");

        if(strcmp(opId,"I")==0){
            opId = strtok(NULL," ");
            pesquisado = buscarId(listaTodasMusicas, numTotalMusicas, opId);
            enfileira(fila,pesquisado);
            media = obtemMediaDuration(fila);
            printf("%d\n",media);
        }
        else {
            desenfileirado = desenfileira(fila);
            printf("(R) %s", &desenfileirado->name);
        }

    }

    return 0;
}

int obtemMediaDuration(Fila* minhaFila){
    ApontadorCelula end = minhaFila->frente;
    int media = 0;
    int tam = 0;
    while (end != NULL){
        media += end->item->duration_ms;
        tam++;
        end = end->proximo;
    }

    return (media/tam);
}

ApontadorCelula criarCelula(tipoMusica *item) {

  ApontadorCelula novaCelula;

  novaCelula = (ApontadorCelula) malloc (sizeof(Celula));
  novaCelula->item = item;
  novaCelula->proximo = NULL;

  return (novaCelula);
}

Fila* criarFila() {

    Fila *minhaFila;

    minhaFila->frente = NULL;
    minhaFila->tras = NULL;

  return (minhaFila);
}

bool filaVazia(Fila* minhaFila) {

  return (minhaFila->frente == minhaFila->tras);
}

void enfileira(Fila* minhaFila, tipoMusica *item) {

  ApontadorCelula nova;
  tipoMusica* temp;

  if(qtd == 5){
    temp = desenfileira(minhaFila);
  }

  nova = criarCelula(item);
  nova->proximo = minhaFila->tras;
  minhaFila->tras = nova;
  qtd++;
}

tipoMusica* desenfileira(Fila* minhaFila) {

  tipoMusica *retirado;
  ApontadorCelula aux;

  if (! filaVazia(minhaFila)) {
      aux = minhaFila->frente;
      retirado = minhaFila->frente->item;

      minhaFila->frente = minhaFila->frente->proximo;

      aux->proximo = NULL;

      free(aux);
      qtd--;

  } else
    retirado = NULL;

  return (retirado);
}
