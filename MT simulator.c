#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Versione con matrice dinamica*/
typedef struct contenutotransizione{
  char scritto;
  char mov;
  int finale;
} contenuti;

typedef struct stato {
	contenuti dati;
	struct stato *next;
} transizione;

typedef struct nastro_
{
	char* parte_destra;
	char* parte_sinistra;
	int size_destra;
	int celle_occupate_destra;
	int size_sinistra;
	int celle_occupate_sinistra;
  int contatore;
}nastro;

nastro* inizializza_nastro(char* str) //funzione che inizializza un nuovo nastro
{
	nastro* rtn = malloc(sizeof(nastro));
	int lunghezza_stringa = strlen(str);
    rtn->contatore=1;
	rtn->parte_destra = malloc(lunghezza_stringa);

	rtn->size_sinistra = rtn->celle_occupate_sinistra = 0;
	rtn->parte_sinistra = NULL;

	rtn->size_destra = rtn->celle_occupate_destra = lunghezza_stringa;
	memcpy(rtn->parte_destra, str, lunghezza_stringa);

	return rtn;
}

char leggi_nastro(nastro* nastr, int pos)
//funzione per leggere un carattere dal nastro
{
	if (pos >= 0) //Parte destra
	{
		if (pos < nastr->celle_occupate_destra)
			return nastr->parte_destra[pos];
		else
			return '_';
	}
	else
	{
		pos = -(pos + 1);
		if (pos < nastr->celle_occupate_sinistra)
			return nastr->parte_sinistra[pos];
		else
			return '_';
	}
}

void scrivi_nastro(nastro* nastr, int pos, char w)
{
	if (pos >= 0)
	{
		if (pos < nastr->celle_occupate_destra)
		{
			nastr->parte_destra[pos] = w;
			return;
		}
		else
		{
			if (pos >= nastr->size_destra)
			{
				nastr->size_destra*=2;
				nastr->parte_destra = realloc(nastr->parte_destra, nastr->size_destra);
			}
			nastr->parte_destra[pos] = w;
			nastr->celle_occupate_destra++;
		}
	}
	else
	{
		pos = -(pos+1);
		if (pos < nastr->celle_occupate_sinistra)
		{
			nastr->parte_sinistra[pos] = w;
			return;
		}
		else
		{
			if (pos >= nastr->size_sinistra)
			{
				if (nastr->size_sinistra == 0)
					nastr->size_sinistra = 512;
				else
					nastr->size_sinistra*=2;
				nastr->parte_sinistra = realloc(nastr->parte_sinistra, nastr->size_sinistra);
			}
			nastr->parte_sinistra[pos] = w;
			nastr->celle_occupate_sinistra++;
		}
	}
}

nastro* copia_nastro(nastro* a)
{
	nastro* rtn = malloc(sizeof(nastro));
	rtn->size_destra = a->size_destra;
	rtn->celle_occupate_destra = a->celle_occupate_destra;
	rtn->parte_destra = malloc(rtn->size_destra);
	memcpy(rtn->parte_destra, a->parte_destra, a->size_destra);

	rtn->size_sinistra = a->size_sinistra;
	rtn->celle_occupate_sinistra = a->celle_occupate_sinistra;
	if (rtn->size_sinistra != 0)
	{
		rtn->parte_sinistra = malloc(rtn->size_sinistra);
		memcpy(rtn->parte_sinistra, a->parte_sinistra, a->size_sinistra);
	}
	else
		rtn->parte_sinistra = NULL;
    rtn->contatore=1;
    /*Avendo appena copiato il nastro in teoria solo la macchina corrente guarderà questo duplicato, quindi
    il contatore va messo a 1*/
	return rtn;
}


typedef struct
{
	//punt punto;
	int posizione_testina;
	int stato_attuale;
	unsigned int conta_mosse;
	nastro* nastro_mt;
}macchina;

macchina nuova_macchina(nastro* n)
{
	macchina rtn;// = malloc(sizeof(macchina));
	rtn.posizione_testina = 0;
	rtn.stato_attuale = 0;
	rtn.nastro_mt = n;
	rtn.conta_mosse = 0;
	return rtn;
}

typedef struct nodo_
{
	macchina contenuto;
	struct nodo_* successivo;
	int free;
}nodo;

typedef struct lista_
{
	nodo* testa;
	nodo* coda;
}lista;

int lista_vuota(lista* list)
{
	if (list->testa == NULL)
		return 1;
	else
		return 0;
}

void inserisci_coda(lista* list, macchina* macch)
{
	nodo* n;// = malloc(sizeof(nodo));

	if (list->coda == NULL)
	{
        n = malloc(sizeof(nodo));
		list->testa = list->coda = n;
		n->successivo = list->testa;
	}
	else if(list->coda->successivo->free==1)
	{
		n=list->coda->successivo;
		list->coda = n;

	}
	else{
         n = malloc(sizeof(nodo));
         list->coda->successivo = n;
         list->coda=n;
         n->successivo = list->testa;
	}
	n->contenuto = *macch;
	n->free=0;
}

macchina* rimuovi_coda(lista* list)
{
	macchina*rtn;
	//if (list->testa == NULL)
	//	abort();
	if (list->testa->successivo == NULL) //1 elemento
	{
		rtn = &(list->testa->contenuto); //Memory leak
		//
		list->testa->free=1;
		//free(list->testa);
		//list->testa = list->coda = NULL;
	}
	else
	{
		rtn = &(list->testa->contenuto);
		list->testa->free=1;
		//free(list->testa);
		list->testa = list->testa->successivo;
	}
	return rtn;
}

#define MAX_ACC 80




int main (){
	//freopen("inputDontLost.txt", "r", stdin);  //apertura del file di input in lettura
  transizione** array[75];
  int accetti[MAX_ACC];
  int prossimo_acc;
  unsigned int max;
  int numero_stati;
  int i, f; //stato iniziale e stato finale della tipotransizione
	char letto, scritto, mov; //carattere letto nella transizione
	numero_stati=512;
    int stato_max;
    int old_numero_stati;
	transizione* list;

	int j, k;
	for(j=0;j<75;j++) array[j]=(transizione**)calloc(numero_stati, sizeof(transizione*));

	scanf("%*s"); // oppure while (getchar() != '\n');

	while (scanf("%d %c %c %c %d\n", &i, &letto, &scritto, &mov, &f)==5)
	{
		if(i>=numero_stati || f>=numero_stati)
		{
          if(i>f)
            stato_max = i;
          else
            stato_max = f;
          old_numero_stati = numero_stati;
		  numero_stati=stato_max+128;
		  for(j=0;j<75;j++)
		  {
			array[j]=(transizione**)realloc(array[j], sizeof(transizione*)*numero_stati);
			for(k=old_numero_stati;k<numero_stati;k++) array[j][k] = 0;
		  }
		}

		int carattere_letto = letto;

		list = (transizione*)malloc(sizeof(transizione));
		list->dati.finale = f;
		list->dati.mov = mov;
		list->dati.scritto = scritto;
		if (array[carattere_letto-48][i] == 0)
		{
			list->next = NULL;
			array[carattere_letto-48][i] = list;
		}
		else
		{
			list->next = array[carattere_letto-48][i];
			array[carattere_letto-48][i] = list;
		}
    }

	scanf("%*s");

	int acc;
	while(scanf("%d ", &acc) == 1)
	{
		//operazione per memorizzazione degli stati di accettazione
		accetti[prossimo_acc] = acc;
		prossimo_acc++;
		//printf("%d\n", acc);
	}

	scanf("%*s");
	scanf("%u", &max);
	scanf("%*s");

	char* stringa;

    lista* coda_att = calloc(1, sizeof(lista));

	while ( scanf("%ms", &stringa) == 1 )
	{
		nastro* n = inizializza_nastro(stringa);
		macchina m = nuova_macchina(n);

		inserisci_coda(coda_att, &m);
		int flag;

		{
			flag = 0;
			while (coda_att->testa->free==0 && flag!=1)
			{
				m = *(rimuovi_coda(coda_att));
				m.nastro_mt->contatore--;
				char carattere_letto = leggi_nastro(m.nastro_mt, m.posizione_testina);
        //leggo il carattere dal nastro e prelevo lo stato attuale dalla macchina
				int stato_attuale = m.stato_attuale;
				transizione* trans = array[carattere_letto-48][stato_attuale];
        //prelevo dalla struttura dati la transizione
				if (trans != NULL)
        //se ci sono transizioni definite, ovvero se non sono in uno stato pozzo, scorro ogni ramo delle transizioni
                {
                      /*controllo per stringhe tipo 2 a a S 2. Per la condizione ho usato
                      l'intersezione delle condizioni. Non si salta la transizione se sono
                      nel ramo di else.*/
                      if((carattere_letto==trans->dati.scritto && trans->dati.mov == 'S' && stato_attuale==trans->dati.finale) || (carattere_letto=='_' && trans->dati.mov == 'R' && stato_attuale==trans->dati.finale && m.posizione_testina == m.nastro_mt->size_destra -1) || (carattere_letto=='_' && trans->dati.mov == 'L' && stato_attuale==trans->dati.finale && -(m.posizione_testina+1) == m.nastro_mt->size_sinistra -1) )
                      {
                            flag=-1;
                            trans = trans->next;
                      }
                      else
                      {
                        //trans = trans->next;
                        //indice_array=curr;
                        while (trans != NULL)
                        {
                            m.conta_mosse++;
                            //if(numero_mt_in_sharing>=1)
                            {
                                //char write = trans->dati.scritto;
                                //se il carattere letto e quello da scrivere coincidono
                                if(carattere_letto==trans->dati.scritto)
                                {
                                    if(m.conta_mosse<=max)
                                    {
                                        m.nastro_mt->contatore++;
                                        //macchina* temp = copia_macchina_senza_nastro(m);
                                        //macchina*temp=malloc(sizeof(macchina));
                                        //*temp=*m;
                                        inserisci_coda(coda_att, &m);
                                        macchina*temp = &(coda_att->coda->contenuto);
                                        if (trans->dati.mov == 'R')
                                            temp->posizione_testina++;
                                        else if (trans->dati.mov == 'L')
                                            temp->posizione_testina--;
                                        temp->stato_attuale = trans->dati.finale;
                                    }
                                    else
                                        flag=-1;
                                }
                                /*Il contatore del nastro della macchina corrente sarà incrementato di 1, perché una macchina
                                in più guarda lo stesso nastro. La posizione dipende dal movimento della testina. Lo stato attuale sarà dato da quello di destinazione della
                                transizione alla posizione curr. La posizione curr parte a scandire l'array dall'inizio, e
                                ogni volta che una mt non viene duplicata perché guarda allo stesso nastro si incrementa di 1
                                in modo da partire alla fase successiva alla prima posizione libera.*/
                                else
                                {
                                    if(m.conta_mosse<=max)
                                    {
                                        char write = trans->dati.scritto;

                                        inserisci_coda(coda_att, &m);
                                        macchina*temp = &(coda_att->coda->contenuto);
                                        temp->nastro_mt = copia_nastro(m.nastro_mt);
                                        scrivi_nastro(temp->nastro_mt, temp->posizione_testina, write);
                                        if (trans->dati.mov == 'R')
                                            temp->posizione_testina++;
                                        else if (trans->dati.mov == 'L')
                                            temp->posizione_testina--;
                                        temp->stato_attuale = trans->dati.finale;
                                    }
                                    else
                                        flag=-1;
                                }
                                /*Altrimenti devo duplicare la macchina e inserirla nella coda delle computazioni successive con il
                                nastro aggiornato. Il contatore del nastro da duplicare viene decrementato perché una macchina in
                                meno lo sta "guardando"*/
                                trans = trans->next;
                            }
                        }

                    }
                }
                else
                {
                    for(i=0; i<prossimo_acc; i++)
                    {
                        if(accetti[i] == m.stato_attuale)
                        {
                            printf("1\n");
                            flag=1;
                            break;
                        }
                    }
                }
                if(m.nastro_mt->contatore==0)
                {   free(m.nastro_mt->parte_destra);
                    free(m.nastro_mt->parte_sinistra);
                    free(m.nastro_mt);
                }
                //free(m);
			}
			while(coda_att->testa->free==0)
			{
                m = *(rimuovi_coda(coda_att));
                m.nastro_mt->contatore--;
                if(m.nastro_mt->contatore==0)
                {
                    free(m.nastro_mt->parte_destra);
                    free(m.nastro_mt->parte_sinistra);
                    free(m.nastro_mt);
                }
                //free(m);
			}

		}

		if (flag == 0)
			printf("0\n");
        else if(flag == -1)
            printf("U\n");
  //stringa=malloc(20000);
	}

	return 0;

}
