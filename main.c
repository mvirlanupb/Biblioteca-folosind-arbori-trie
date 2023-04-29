/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "trie.h"
#include "carte.h"

int exista_tilda(char *s)
{
    size_t i;
    for(i=0;i<strlen(s);i++)
    {
        if(s[i]=='~')
            return 1;
    }
    return 0;

}

int exista_doua_puncte(char *s)
{
    size_t i;
    for(i=0;i<strlen(s);i++)
    {
        if(s[i]==':')
            return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    FILE *fp,*fp1;
    fp=fopen(argv[1],"r");
    if(fp==NULL)
        {
            printf("Eroare la deschiderea fisierului de intrare.\n");
            return -1;
        }
    fp1=fopen(argv[2],"w");
    if(fp1==NULL)
        {
            printf("Eroare la pregatirea fisierului de scriere.\n");
            return -1;
        }
    char *linie_fisier=NULL,titlu_carte[51],numele_autorului[41],rating_string[5],nr_pagini_string[5];
    size_t lungime=0;
    TTrie T1=creeaza_nod("\0");//inițiem arborii
    TTrie T2=creeaza_nod("\0");
    void *x=NULL;
    while(getline(&linie_fisier,&lungime,fp)!=-1)
    {
        linie_fisier[strlen(linie_fisier)-1]='\0';
        if(strstr(linie_fisier,"add_book"))
        {
            char *p;
            p=strtok(linie_fisier,":");
            p=p+strlen("add_book ");
            strcpy(titlu_carte,p);
            TCarte C;
            strcpy(C.titlul_cartii,titlu_carte);
            p=strtok(NULL,":");
            strcpy(numele_autorului,p);
            strcpy(C.nume_autor,numele_autorului);
            p=strtok(NULL,":");
            strcpy(rating_string,p);
            C.rating=atoi(rating_string);
            p=strtok(NULL,":");
            strcpy(nr_pagini_string,p);
            C.numar_pagini=atoi(nr_pagini_string);
            void *y;
            if(cauta(T1,titlu_carte,&y)==0)
            {
                adauga(T1,titlu_carte,&C,sizeof(TCarte));
                adauga_autor(T2,numele_autorului,titlu_carte,&C,sizeof(TCarte));
            }
        }
        if(strstr(linie_fisier,"search_book")&&(exista_tilda(linie_fisier)==0))
        {
            char *p;
            p=linie_fisier;
            p=p+strlen("search_book ");
            if(cauta(T1,p,&x)==0)
                fprintf(fp1,"Cartea %s nu exista in recomandarile tale.\n",p);
            else
                {
                    TCarte C=*(TCarte*)x;
                    fprintf(fp1,"Informatii recomandare: %s, %s, %d, %d\n",C.titlul_cartii,C.nume_autor,C.rating,C.numar_pagini);
                }
            x=NULL;
        }
        if(strstr(linie_fisier,"list_author")&&(exista_tilda(linie_fisier)==0))
        {
            char *p;
            p=linie_fisier;
            p=p+strlen("list_author ");
            cauta_autor(T2,p,fp1);

        }
        if(strstr(linie_fisier,"search_by_author")&&(exista_tilda(linie_fisier)==0))
        {
            char *p;
            p=linie_fisier+strlen("search_by_author ");
            char *q;
            q=strtok(p,":");
            strcpy(numele_autorului,q);
            q=strtok(NULL,":");
            strcpy(titlu_carte,q);

            cauta_autor_si_carte(T2,numele_autorului,titlu_carte,fp1);


        }
        if(strstr(linie_fisier,"list_author")&&(exista_tilda(linie_fisier)==1))
        {
            char *p;
            p=linie_fisier+strlen("list_autor  ");
            p[strlen(p)-1]='\0';
            cauta_autor_dupa_prefix(T2,p,fp1);

        }
        if(strstr(linie_fisier,"search_book")&&(exista_tilda(linie_fisier)==1))
        {
            char *p;
            p=linie_fisier+strlen("search_book ");
            p[strlen(p)-1]='\0';
            cauta_carte_dupa_prefix(T1,p,fp1);

        }
        if(strstr(linie_fisier,"search_by_author")&&(exista_tilda(linie_fisier)==1)&&(exista_doua_puncte(linie_fisier)==0))
        {
            char *p;
            p=linie_fisier+strlen("search_by_author ");
            p[strlen(p)-1]='\0';
            cauta_autor_dupa_prefix(T2,p,fp1);

        }
        if(strstr(linie_fisier,"search_by_author")&&(exista_tilda(linie_fisier)==1)&&(exista_doua_puncte(linie_fisier)==1))
        {
            char *p;
            p=linie_fisier+strlen("search_by_author ");
            p[strlen(p)-1]='\0';
            char *q;
            q=strtok(p,":");
            strcpy(numele_autorului,q);
            q=strtok(NULL,":");
            cauta_autor_si_prefix_carte(T2,numele_autorului,q,fp1);

        }
        if(strstr(linie_fisier,"delete_book"))
        {
            char *p;
            p=linie_fisier+strlen("delete_book ");
            cauta(T1,p,&x);
            if(x!=NULL)
            {
            TCarte C=*(TCarte*)x;
            strcpy(numele_autorului,C.nume_autor);
            sterge(T1,p,p,fp1);
            sterge_carte_din_arborele_autorului(T2,numele_autorului,p,fp1);
            }
            else
                fprintf(fp1,"Cartea %s nu exista in recomandarile tale.\n",p);
            x=NULL;
        }

    }
    distruge(T1);
    distruge(T2);
    free(T1);
    free(T2);
    free(linie_fisier);
    fclose(fp);
    fclose(fp1);
    return 0;
}
