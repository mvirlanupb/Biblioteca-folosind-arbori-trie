/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "trie.h"
#include "carte.h"

/*Fiecare nod al unui arbore Trie ”stochează” un șir de caractere și un pointer
către o zonă de memorie în care se pot pune alte informații.*/

TTrie creeaza_nod(char *cheie)
{
    TTrie T=malloc(sizeof(TNod));//inițiem nodul
    if(T==NULL)
        return NULL;
    strcpy(T->sir,cheie);//atașăm șirul de caractere de nod
    int i;
    for(i=0;i<alfabet;i++)
        T->legaturi[i]=NULL;//inițializăm legăturile cu NULL
    T->info=NULL;//inițial, zona de memorie asociată unei informații este NULL

    return T;
}


/*În continuare, am pregătit repartizarea fiecărui caracter, atribuind pentru fiecare, în mod unic
un indice pentru vectorul de legături*/

int returneaza_indice(char c)
{
    if((c>='a')&&(c<='z'))
        return c-'a';
    if((c>='A')&&(c<='Z'))
        return c-'A'+26;
    if((c>='0')&&(c<='9'))
        return c-'0'+52;
    if(c=='.')
        return 62;
    if(c=='-')
        return 63;
    if(c=='?')
        return 65;
    if(c=='!')
        return 66;
    if(c==' ')
        return 67;
    return 64;//64 este pentru apostrof

}

/*Verifică dacă indicele corespunzător primului caracter este ocupat (există legătură) sau nu.*/
int gaseste_copil(TTrie T, char *cheie)
{
    return (T->legaturi[returneaza_indice(cheie[0])]!=NULL);
}
/*Verifică dacă toate legăturile sunt vide*/
int este_frunza(TTrie T)
{
    int i;
    for(i=0;i<alfabet;i++)
    {
        if(T->legaturi[i]!=NULL)
            return 0;
    }
    return 1;

}

/*Funcția verifică dacă cheia există în arbore, prin parcurgere de noduri care conțin
bucăți din șir. Funcția primeșete un void **x pentru că doresc să întorc prin efect lateral informația,
dacă există. La apelul funcției x va fi deja NULL.*/
int cauta(TTrie T, char *cheie, void **x)
{
    TTrie aux=T;
    char s_aux[51];//pregătim un șir auxiliar în care stocăm cheia
    strcpy(s_aux,cheie);
    if(gaseste_copil(aux,cheie)==0)
        return 0;//dacă de la rădăcină nu avem legătură cu primul caracter al cheii, întorc 0.
    /*Cât timp există legătură cu primul caracter*/
    while(gaseste_copil(aux,cheie)==1)
    {
        char p=cheie[0];
        aux=aux->legaturi[returneaza_indice(p)];//avansăm
        /*Dacă șirul atașat unui nod conține deja o porțiune de cheie și informația
        există, este preluată*/
        if((strcmp(aux->sir,cheie)==0)&&(aux->info!=NULL))
            {
            *x=aux->info;
            return 1;
            }
        int i=0;
        while((aux->sir[i]==cheie[i])&&(aux->sir[i]!='\0'))
            i++;//vedem câte caractere comune au
        cheie=cheie+i;//actualizăm cheia
    }
    return 0;

}


void adauga(TTrie T, char *cheie, void *x, size_t d)
{
    /*Dacă din prima nu există legătură cu primul caracter al cheii, aceasta este creată*/
    if(gaseste_copil(T,cheie)==0)
    {
        char p=cheie[0];
        T->legaturi[returneaza_indice(p)]=creeaza_nod(cheie);
        T->legaturi[returneaza_indice(p)]->info=malloc(d);//se pregătește informația
        if(T->legaturi[returneaza_indice(p)]->info==NULL)
            return;
        memcpy(T->legaturi[returneaza_indice(p)]->info,x,d);
    }
    /*Altfel, facem o parcurgere în arbore pentru a elimina caracterele comune din cheie cu șirurile
    asociate fiecărui nod și de a repartiza corespunzător informația.*/
    else
    {
        TTrie aux=T;
        size_t indice;//indicele reține ultima poziție în care au fost caractere comune
        while(gaseste_copil(aux,cheie)==1)//cât timp avem descendenți
        {
            char p=cheie[0];
            aux=aux->legaturi[returneaza_indice(p)];//se face avansul
            size_t i=0;
            while((aux->sir[i]==cheie[i])&&(aux->sir[i]!='\0'))
                i++;//numărăm câte caractere comune avem
            cheie=cheie+i;//scăpăm cheia de caracterele comune
            indice=i;
            /*Dacă numărul de caractere comune este mai mic decât lungimea șirului din nod
            oprim căuarea, în semn că am găsit o poziție de la care facem inserarea*/
            if(i<strlen(aux->sir))
                break;
        }
        /*În această etapă, vom ”rupe” nodul la care am ajuns în părți comune și necomune cu cheia.
        Dacă șirul din nod mai are încă caractere*/
        if(aux->sir[indice]!='\0')
        {
            /*Inițiem un nod care va stoca restul caracterelor din nod*/
            TTrie aux1=creeaza_nod(aux->sir+indice);
            aux1->info=aux->info;//transferăm informația
            aux->info=NULL;//anulăm informația din nodul vechi
            size_t j;
            for(j=0;j<alfabet;j++)
                {
                    aux1->legaturi[j]=aux->legaturi[j];
                    /*Legăturile noului nod se racordează la legăturile nodului în care suntem*/
                    aux->legaturi[j]=NULL;
                }
            /*Nodul care are subșir comun se racordează la cel nou*/
            aux->legaturi[returneaza_indice(aux->sir[indice])]=aux1;
            aux->sir[indice]='\0';//păstrăm partea comună

        }
        char p=cheie[0];
        if(p!='\0')//dacă informația este nouă (cheia mai are încă caractere)
        {
        aux->legaturi[returneaza_indice(p)]=creeaza_nod(cheie);//se inițiază o legătură nouă
        aux->legaturi[returneaza_indice(p)]->info=malloc(d);
        if(aux->legaturi[returneaza_indice(p)]->info==NULL)
            return;
        memcpy(aux->legaturi[returneaza_indice(p)]->info,x,d);
        }
        else//altfel, înseamnă că noua informație este un prefix al unei informații existente
        {
            aux->info=malloc(d);//inițiem informația
            if(aux->info==NULL)
                return;
            memcpy(aux->info,x,d);
        }
    }
}

/*În mare parte, funcția de inserare autor funcționează pe principii asemănătoare unei inserări normale,
singura excepție fiind adăugarea cărții în arborele tip T1 corespunzător*/
void adauga_autor(TTrie T, char *cheie, char *titlu, void *x, size_t d)
{
    if(gaseste_copil(T,cheie)==0)
    {
        char p=cheie[0];
        T->legaturi[returneaza_indice(p)]=creeaza_nod(cheie);
        TTrie aux=T;
        aux=aux->legaturi[returneaza_indice(p)];
        adauga(aux,titlu,x,d);
    }
    else
    {
        TTrie aux=T;
        char s_aux[51];
        strcpy(s_aux,cheie);
        size_t indice;
        while(gaseste_copil(aux,cheie)==1)
        {
            char p=cheie[0];
            aux=aux->legaturi[returneaza_indice(p)];
            size_t i=0;
            while((aux->sir[i]==cheie[i])&&(aux->sir[i]!='\0'))
                i++;
            cheie=cheie+i;
            indice=i;
            if(i<strlen(aux->sir))
                break;
        }
        if(aux->sir[indice]!='\0')
        {
            TTrie aux1=creeaza_nod(aux->sir+indice);
            aux1->info=aux->info;
            aux->info=NULL;
            size_t j;
            for(j=0;j<alfabet;j++)
                {
                    aux1->legaturi[j]=aux->legaturi[j];
                    aux->legaturi[j]=NULL;
                }
            aux->legaturi[returneaza_indice(aux->sir[indice])]=aux1;
            aux->sir[indice]='\0';

        }
        char p=cheie[0];
        if(p!='\0')
        {
        aux->legaturi[returneaza_indice(p)]=creeaza_nod(cheie);
        aux=aux->legaturi[returneaza_indice(p)];
        adauga(aux,titlu,x,d);
        }
        else
            adauga(aux,titlu,x,d);
    }

}


/*
Observație: la apelul acestei funcții, se consideră că am aflat de existența unor autori ai căror nume încep cu
un prefix dat. Această funcție va ajunge, de fapt la arborele de
căutare de tip T1 al cărților unui autor și, dacă a găsit o zonă cu informație, facem cast la tipul TCarte și verificăm dacă numele
a fost deja folosit la afișare.
Această funcție afișează un număr dat de autori. Pentru că am vrut să o fac în mod recursiv, am
inclus un pointer ca parametru în care rețin adresa numărului de afișări. Șirul de caractere autor va păstra
numele unui autor, pentru a ne asigura că acesta nu se repetă.*/
void afiseaza_autori(TTrie T, int *nr_afisari, char *autor, FILE *fp)
{
    int i;
    if(T==NULL)
        return;
    for(i=0;i<alfabet;i++)
    {
        if(T->legaturi[i]!=NULL)//am găsit o legătură
        {
        if((T->legaturi[i]->info!=NULL)&&(*nr_afisari<=3))//am găsit informație și respectăm numărul de afișări
            {
                TCarte C=*(TCarte*)T->legaturi[i]->info;//cast la structura de tip Carte
                if(strcmp(C.nume_autor,autor)==0)//dacă se repetă numele unui autor, mergem mai departe
                    continue;
                else
                {
                fprintf(fp,"%s\n",C.nume_autor);//altfel, afișăm
                strcpy(autor,C.nume_autor);//actualizăm autorul referință
                (*nr_afisari)++;//actualizez valoarea de la adresa lui nr_afisari
                }
            }
        else
            afiseaza_autori(T->legaturi[i],nr_afisari,autor,fp);

        }
    }
}

/*Funcție care caută numele unui autor în T2*/
void cauta_autor(TTrie T, char *cheie, FILE *fp)
{
    TTrie aux=T;
    char s_aux[51];
    strcpy(s_aux,cheie);//salvez numele autorului într-un șir auxiliar
    if(gaseste_copil(aux,cheie)==0)//dacă din start nu avem legătură către primul caracter, numele autorului nu există în arbore
        {
        fprintf(fp,"Autorul %s nu face parte din recomandarile tale.\n",s_aux);
        return;
        }
    /*Cât timp avem descendenți, începem parcurgerea arborelui. Pe parcurs, cheia
    se actualizează, scăpând de caracterele comune ale cheii cu șirurile din noduri*/
    while(gaseste_copil(aux,cheie)==1)
    {
        char p=cheie[0];
        aux=aux->legaturi[returneaza_indice(p)];
        if(strcmp(aux->sir,cheie)==0)//dacă șirul dintr-un nod coincide cu bucata rămasă din cheie
            {
                afiseaza_toate_cartile(aux,fp);//se apelează funcție de afișare a tuturor cărților
                return;
            }
        int i=0;
        while((aux->sir[i]==cheie[i])&&(aux->sir[i]!='\0'))
            i++;//numărăm caracterele comune și actualizăm cheia
        cheie=cheie+i;
    }
    fprintf(fp,"Autorul %s nu face parte din recomandarile tale.\n",s_aux);

}

/*Această funcție prezintă asemănări cu precedenta */
void cauta_autor_dupa_prefix(TTrie T, char *prefix, FILE *fp)
{
    TTrie aux=T;
    /*Dacă nu avem legături cu primul caracter din prefix, nu există în arbore astfel de autori*/
    if(gaseste_copil(T,prefix)==0)
        {
        fprintf(fp,"Niciun autor gasit.\n");
        return;
        }
    while(gaseste_copil(aux,prefix)==1)//inițiem o parcurgere, actualizând cheia
    {
        aux=aux->legaturi[returneaza_indice(prefix[0])];
        int i=0;
        while((aux->sir[i]==prefix[i])&&(aux->sir[i]!='\0'))
            i++;
        prefix=prefix+i;//actualizăm prefixul
    }
    /*Dacă au mai rămas caractere în prefix, nu avem astfel de autori*/
    if(prefix[0]!='\0')
        fprintf(fp,"Niciun autor gasit.\n");
    else//altfel (am golit prefixul)
        {
        int nr=1;//inițiem numărul de afișări
        /*inițiem șirul care va păstra numele autorilor, pentru a ne asigrua că nu se repetă numele unuia*/
        char *autor=(char*)calloc(42,sizeof(char));
        afiseaza_autori(aux,&nr,autor,fp);
        free(autor);
        }

}

void cauta_carte_dupa_prefix(TTrie T, char *prefix, FILE *fp)
{
    TTrie aux=T;
    if(gaseste_copil(T,prefix)==0)
        {
        fprintf(fp,"Nicio carte gasita.\n");
        return;
        }
    while(gaseste_copil(aux,prefix)==1)
    {
        aux=aux->legaturi[returneaza_indice(prefix[0])];
        int i=0;
        while((aux->sir[i]==prefix[i])&&(aux->sir[i]!='\0'))
            i++;

        prefix=prefix+i;
    }
    if(prefix[0]!='\0')
        fprintf(fp,"Nicio carte gasita.\n");
    else
        {
        int nr=1;
        /*Caz special în care nodul curent conține informație*/
        if(aux->info!=NULL)
            {
                TCarte C=*(TCarte*)aux->info;
                fprintf(fp,"%s\n",C.titlul_cartii);
            }
        /*Această funcție verifică dacă printre descendenții nodului se mai află și alte cărți*/
        afiseaza_un_numar_finit_de_carti(aux,&nr,fp);
        }

}
/*copie este numele original al cheii, la primul apel al funcției*/
void sterge(TTrie T, char *cheie, char *copie, FILE *fp)
{
    char s_aux[51];
    TTrie aux=T,ant;
    strcpy(s_aux,cheie);//copiem cheia
    if(gaseste_copil(T,cheie)==0)
        return;
    while(gaseste_copil(aux,cheie)==1)
    {
        ant=aux;//vrem să pătrăm nodul precedent celui pe care vrem să-l eliminăm
        aux=aux->legaturi[returneaza_indice(cheie[0])];
        int j=0;
        while((aux->sir[j]==cheie[j])&&(aux->sir[j]!='\0'))
                    j++;
        cheie=cheie+j;
    }
    /*Am verificat mai întâi dacă cartea pe care vrem să o ștergem există în arbore*/
    if(cheie[0]!='\0')
        fprintf(fp,"Cartea %s nu exista in recomandarile tale.\n",s_aux);
    else
    {
        if(este_frunza(aux)==1)//eliminăm numai frunzele
            {
            ant->legaturi[returneaza_indice(aux->sir[0])]=NULL;//rupem legătura cu nodul curent
            if(aux->info!=NULL)
            {
            TCarte C=*(TCarte*)aux->info;
            /*Pentru a nu altera alte cărți care pot fi prefixul altora, verificăm în plus dacă titlul cărții pe care
            îl ștergem coincide cu ce se află în cheia salvată în s_aux*/
            if(strcmp(C.titlul_cartii,copie)!=0)
                return;
            }
            /*Din cheia copiată, scoatem subșirul din nodul curent*/
            s_aux[strlen(s_aux)-strlen(aux->sir)]='\0';
            free(aux->info);//eliminăm informația
            free(aux);//distrugem nodul
            sterge(T,s_aux,copie,fp);//apelăm funcția cu șirul rămas
            }
    }
}

void distruge(TTrie T)
{
    if(T==NULL)
        return;
    int i;
    for(i=0;i<alfabet;i++)
    {
        distruge(T->legaturi[i]);
    }
    for(i=0;i<alfabet;i++)
        free(T->legaturi[i]);
    free(T->info);

}
