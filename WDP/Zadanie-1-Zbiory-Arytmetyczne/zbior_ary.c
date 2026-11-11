// Autor: Maciej Mokrzycki
// code review: Oskar Skoczylas
#include "zbior_ary.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

static int Q = -1;  // q nie zdefiniowany jeszcze

// reszta z dzielenia przez Q (tez dla liczb ujemnych)
static int reszta(int x) {
    int r = x % Q;
    if (r < 0)
        r += Q;
    return r;
}

zbior_ary ciag_arytmetyczny(int a, int q, int b) {
    if (Q == -1)
        Q = q;

    zbior_ary A;
    A.n = 0;
    A.pojemnosc = 1;
    A.komp = malloc(sizeof(komponent));

    if (A.komp == NULL)
        exit(EXIT_FAILURE);

    if (a > b)
        return A;  // pusty zbior

    A.komp[0].first = a;
    A.komp[0].last = b;
    A.komp[0].r = reszta(a);
    A.n = 1;

    return A;
}

zbior_ary singleton(int a) {
    return ciag_arytmetyczny(a, Q, a);
}

unsigned moc(zbior_ary A) {
    unsigned long long card = 0;

    for (size_t i = 0; i < A.n; i++)
        card += (unsigned long long)(
            ((long long)A.komp[i].last - A.komp[i].first) / Q + 1
        );

    if (card > UINT_MAX)
        exit(EXIT_FAILURE);

    return (unsigned)card;
}

bool nalezy(zbior_ary A, int x) {
    int r_x = reszta(x);

    // lzłożnosc logartmiczna
    // bin searchem znajduje ostatni komponent o reszcie r_x i first <= x
    size_t left = 0, right = A.n;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        komponent X = A.komp[mid];

        if (X.r < r_x || (X.r == r_x && X.first <= x))
            left = mid + 1;
        else
            right = mid;
    }

    if (left == 0)
        return false;

    // sprawdzam, czy x miesci sie w znalezionym komponencie
    komponent X = A.komp[left - 1];
    return X.r == r_x && x <= X.last;
}

unsigned ary(zbior_ary A) {
    return (unsigned)A.n;
}

static bool powieksz_pamiec(zbior_ary *A, size_t new_length) { // zwieksz dwukrotnie pamiec
    if (new_length > SIZE_MAX / sizeof(komponent))
        return false;

    komponent *tmp = realloc(A->komp, new_length * sizeof(komponent));

    if (tmp == NULL)  // nie udalo sie powiekszyc tablicy
        return false;

    A->komp = tmp;
    A->pojemnosc = new_length;

    return true;
}

static bool mozna_polaczyc(komponent X, komponent Y) {
    // return true iff maja ta sama reszte mod Q i mozna je polaczyc
    if (X.r != Y.r)
        return false;

    if ((long long)X.last + Q < Y.first)
        return false;

    if ((long long)Y.last + Q < X.first)
        return false;

    return true;
}

static komponent polacz_komp(komponent X, komponent Y) {
    // sluzy do mergowania dwoch komponentow o niepustym przekroju i tej samej reszcie
    komponent result;

    result.r = X.r;
    result.first = (X.first < Y.first) ? X.first : Y.first;
    result.last = (X.last > Y.last) ? X.last : Y.last;

    return result;
}

static void dodaj(zbior_ary *A, komponent new_komp) { // dodaje komponent na koniec
    if (A->n == A->pojemnosc) {
        if (A->pojemnosc > SIZE_MAX / 2)
            exit(EXIT_FAILURE);

        size_t new_pojemnosc = 2 * A->pojemnosc;

        if (!powieksz_pamiec(A, new_pojemnosc))
            exit(EXIT_FAILURE);
    }

    A->komp[A->n] = new_komp;
    A->n++;
}

static void dodaj_scalajac(zbior_ary *A, komponent X) {
    if (A->n == 0) {
        dodaj(A, X);
        return;
    }

    komponent *last = &A->komp[A->n - 1];

    if (mozna_polaczyc(*last, X)) {
        *last = polacz_komp(*last, X);
    } else {
        dodaj(A, X);
    }
}

zbior_ary suma(zbior_ary A, zbior_ary B) {
    zbior_ary SUM;   // to bedzie suma A i B
    SUM.n = 0;
    SUM.pojemnosc = 1;
    SUM.komp = malloc(sizeof(komponent)); // poczatkowa zaalokowana pamiec

    if (SUM.komp == NULL)
        exit(EXIT_FAILURE);

    size_t ind_A = 0;
    size_t ind_B = 0; // indeks aktualnie rozpatrywanego komponentu

    while (ind_A < A.n && ind_B < B.n) {
        // komponenty sa posortowane po 'r', a potem po 'first'
        komponent X = A.komp[ind_A];
        komponent Y = B.komp[ind_B];

        // Wybieramy wcześniejszy komponent
        // najpierw według r, potem według first
        if (X.r < Y.r || (X.r == Y.r && X.first <= Y.first)) {
            dodaj_scalajac(&SUM, X);
            ind_A++;
        } else {
            dodaj_scalajac(&SUM, Y);
            ind_B++;
        }
    }

    // Pozostałe komponenty z A
    while (ind_A < A.n) {
        dodaj_scalajac(&SUM, A.komp[ind_A]);
        ind_A++;
    }

    // Pozostałe komponenty z B
    while (ind_B < B.n) {
        dodaj_scalajac(&SUM, B.komp[ind_B]);
        ind_B++;
    }

    return SUM;
}

static bool roznica_komponentow(zbior_ary *ZBIOR, komponent *X, komponent Y) { // X - Y
    // Y dzieli X na dwie części - lewą i prawą (byc moze ktoras jest pusta)
    // lewą dodajemy do zbioru, prawą zostawiamy w X

    // Lewa część X, której Y nie usuwa.
    if (X->first < Y.first) {
        komponent L;

        L.r = X->r;
        L.first = X->first;
        L.last = (int)((long long)Y.first - Q);

        dodaj(ZBIOR, L);
    }

    long long new_first = (long long)Y.last + Q;

    if (new_first > X->last)
        return false;

    X->first = (int)new_first;
    return true;
}

zbior_ary roznica(zbior_ary A, zbior_ary B) {
    zbior_ary DIFF; // to bedzie roznica A i B
    DIFF.n = 0;
    DIFF.pojemnosc = 1;
    DIFF.komp = malloc(sizeof(komponent)); // poczatkowa zaalokowana pamiec

    if (DIFF.komp == NULL)
        exit(EXIT_FAILURE);

    size_t ind_A = 0;
    size_t ind_B = 0; // indeks aktualnie rozpatrywanego komponentu

    while (ind_A < A.n) {
        // aktualny komponent A, ktorey bedziemy zmniejszac gdy bedzie to mozliwe
        komponent tmp = A.komp[ind_A];

        // Pomijamy komponenty B, które są wcześniej
        // i nie mogą już przecinać bieżącego komponentu A.
        while (ind_B < B.n &&
               (B.komp[ind_B].r < tmp.r ||
                (B.komp[ind_B].r == tmp.r &&
                 B.komp[ind_B].last < tmp.first))) {
            ind_B++;
        }

        // bedziemy sprawdzac czy w A zostały jeszcze jakies elementy
        bool niepusty = true;

        // Odejmujemy kolejne komponenty B,
        // które przecinają bieżący komponent A.
        while (ind_B < B.n &&
               B.komp[ind_B].r == tmp.r &&
               B.komp[ind_B].first <= tmp.last) {

            niepusty = roznica_komponentow(&DIFF, &tmp, B.komp[ind_B]);

            // B usunęło całą pozostałą część tmp.
            if (!niepusty)
                break;

            // sprawdzamy nastepny komponent
            ind_B++;
        }

        // jeżeli zostało cos jeszcze to dodajemy to.
        if (niepusty)
            dodaj(&DIFF, tmp);

        ind_A++;
    }

    return DIFF;
}

zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
    // A przeciecie B = A - (A - B)
    zbior_ary tmp = roznica(A, B);
    zbior_ary wynik = roznica(A, tmp);

    free(tmp.komp); // zwalniam zeby uniknąć wycieku pamięci

    return wynik;
}
// Wynikiem funkcji jest część wspólna zbiorów A i B.
