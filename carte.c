/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "carte.h"


void afiseaza_toate_cartile(TTrie T, FILE *fp)
{
    int i;
    if(T==NULL)
        return;
    for(i=0;i<alfabet;i++)
    {
        if(T->legaturi[i]!=NULL)
        {
        if(T->legaturi[i]->info!=NULL)
            {
                TCarte C=*(TCarte*)T->legaturi[i]->info;
                fprintf(fp,"%s\n",C.titlul_cartii);
            }
            afiseaza_toate_cartile(T->legaturi[i],fp);

        }
    }

}

void cauta_autor_si_carte(TTrie T, char *nume_autor, char *titlul_cartii, FILE *fp)
{
    void *y=NULL;
    TTrie aux=T;
    char s_aux[51];
    strcpy(s_aux,nume_autor);//salvăm numele autorului
    if(gaseste_copil(aux,nume_autor)==0)//dacă din rădăcină nu avem legătură către primul caracter, numele autorului nu există în arbore
        {
        fprintf(fp,"Autorul %s nu face parte din recomandarile tale.\n",s_aux);
        return;
        }
    while(gaseste_copil(aux,nume_autor)==1)//parcurgem arborele
    {
        char p=nume_autor[0];
        aux=aux->legaturi[returneaza_indice(p)];//ajungem la noul nod
        if(strcmp(aux->sir,nume_autor)==0)//am găsit că numele unui nod coincide cu bucata rămasă din nume_autor
            {
                cauta(aux,titlul_cartii,&y);//căutăm cartea
                if(y==NULL)
                    fprintf(fp,"Cartea %s nu exista in recomandarile tale.\n",titlul_cartii);
                else
                {
                TCarte C=*(TCarte*)y;
                fprintf(fp,"Informatii recomandare: %s, %s, %d, %d\n",C.titlul_cartii,C.nume_autor,C.rating,C.numar_pagini);
                }
                return;
            }
        int i=0;
        while((aux->sir[i]==nume_autor[i])&&(aux->sir[i]!='\0'))
            i++;
        nume_autor=nume_autor+i;
    }
    fprintf(fp,"Autorul %s nu face parte din recomandarile tale.\n",s_aux);


}

/*Principiu asemănător cu afișarea a maxim 3 autori*/
void afiseaza_un_numar_finit_de_carti(TTrie T, int *nr, FILE *fp)
{

    int i;
    for(i=0;i<alfabet;i++)
    {
        if(T->legaturi[i]!=NULL)
        {
            if((T->legaturi[i]->info!=NULL)&&(*nr<=3))
            {
                TCarte C=*(TCarte*)T->legaturi[i]->info;
                fprintf(fp,"%s\n",C.titlul_cartii);
                (*nr)++;
            }
            else
                afiseaza_un_numar_finit_de_carti(T->legaturi[i],nr,fp);

        }
    }
}

void cauta_autor_si_prefix_carte(TTrie T, char *nume_autor, char *prefix, FILE *fp)
{
    TTrie aux=T;
    char s_aux[51];
    strcpy(s_aux,nume_autor);
    if(gaseste_copil(aux,nume_autor)==0)
        {
        fprintf(fp,"Autorul %s nu face parte din recomandarile tale.\n",s_aux);
        return;
        }
    while(gaseste_copil(aux,nume_autor)==1)
    {
        char p=nume_autor[0];
        aux=aux->legaturi[returneaza_indice(p)];
        if(strcmp(aux->sir,nume_autor)==0)//dacă am găsit numele autorului, parcurgem arborele de căutare al cărților lui
            {
                cauta_carte_dupa_prefix(aux,prefix,fp);
                return;
            }
        int i=0;
        while((aux->sir[i]==nume_autor[i])&&(aux->sir[i]!='\0'))
            i++;
        nume_autor=nume_autor+i;
    }
    fprintf(fp,"Autorul %s nu face parte din recomandarile tale.\n",s_aux);

}

void sterge_carte_din_arborele_autorului(TTrie T, char *autor, char *carte, FILE *fp)
{
     TTrie aux=T;
     char s_aux[42];
     strcpy(s_aux,autor);

     while(gaseste_copil(aux,autor)==1)
    {
        aux=aux->legaturi[returneaza_indice(autor[0])];
        int j=0;
        while((aux->sir[j]==autor[j])&&(aux->sir[j]!='\0'))
                    j++;
        autor=autor+j;
    }

    sterge(aux,carte,carte,fp);
    if(este_frunza(aux)==1)
        sterge(T,s_aux,s_aux,fp);
}
