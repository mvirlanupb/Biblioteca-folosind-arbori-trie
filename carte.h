/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "trie.h"
#ifndef CARTE_H_INCLUDED
#define CARTE_H_INCLUDED

typedef struct
{
    char titlul_cartii[51];
    char nume_autor[41];
    int rating;
    int numar_pagini;
}TCarte;

void afiseaza_toate_cartile(TTrie T, FILE *fp);
void cauta_autor_si_carte(TTrie T, char *nume_autor, char *titlul_cartii, FILE *fp);
void afiseaza_un_numar_finit_de_carti(TTrie T, int *nr, FILE *fp);
void cauta_autor_si_prefix_carte(TTrie T, char *nume_autor, char *prefix, FILE *fp);
void sterge_carte_din_arborele_autorului(TTrie T, char *autor, char *carte, FILE *fp);

#endif // CARTE_H_INCLUDED
