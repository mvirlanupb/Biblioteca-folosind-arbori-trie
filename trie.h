/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define alfabet 68
#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

typedef struct nod
{
    struct nod *legaturi[alfabet];//vector de legături
    char sir[50];//reținem în nod un șir de maxim 50 de caractere
    void *info;//adresa unde pot fi stocate informații
}TNod,*TTrie;

TTrie creeaza_nod(char *cheie);//funcție care creează un nod ce va conține un șir de caractere dat ca parametru
int returneaza_indice(char c);//funcție care returnează locul unui caracter în vectorul de legături
/*Functia gaseste_copil verifică dacă un nod T al arborelui are legătura către indicele primului caracter
al șirului cheie nevidă*/
int gaseste_copil(TTrie T, char *cheie);
/*Funcția adauga inserează în arborele Trie Comprimat șirul cheie (sau doar o parte în caz în
care are prefix comun cu alte șiruri) și informația x generică de dimensiune d */
void adauga(TTrie T, char *cheie, void *x, size_t d);
int este_frunza(TTrie T);//verifică dacă nodul curent este frunză (are toate legăturile vide)

/*Funcția verifică în arborele de tip trie de-a lungul cheii dacă nodurile conțin întocmai
părți din șirul de caractere cheie. Dacă toate caracterele corespund, întoarce informația.
(Valabil doar pentru T1)*/
int cauta(TTrie T, char *cheie, void **x);
/*Funcție care adaugă autorul în arborele trie T2, iar la finalul cheii completează
arborele cu cărți corespunzător autorului. Arborele cu cărți se completează la fel ca T1.*/
void adauga_autor(TTrie T, char *cheie, char *titlu, void *x, size_t d);
/*Funcție care afișează un număr de maxim nr_afișări autori*/
void afiseaza_autori(TTrie T, int *nr_afisari, char *autor, FILE *fp);
void cauta_autor(TTrie T, char *cheie, FILE *fp);//căutare autor în T2
void cauta_autor_dupa_prefix(TTrie T, char *prefix, FILE *fp);
void cauta_carte_dupa_prefix(TTrie T, char *prefix, FILE *fp);
void sterge(TTrie T, char *cheie, char *copie, FILE *fp);
void distruge(TTrie T);
#endif // TRIE_H_INCLUDED
