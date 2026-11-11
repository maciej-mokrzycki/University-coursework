**Arithmetic sets · C17**

Set representation, union, intersection, and difference.

This assignment implements the API in [`zbior_ary.h`](./zbior_ary.h). The supplied [`ocen.c`](./ocen.c) runs example operations and checks their results with assertions.

## Build and run

Requires GCC with C17 support.

Run the following commands from the assignment directory containing `zbior_ary.c`, `zbior_ary.h`, `ocen.c`, and `opcje`:

```bash
gcc @opcje -std=c17 ocen.c zbior_ary.c -o ary -lm
./ary
```

The `@opcje` argument loads compiler options from the supplied file. The `-std=c17` flag selects the C17 standard, and `-lm` links the math library.

## Task description

### Zbiory arytmetyczne

W tym zadaniu powinieneś zaimplementować w języku C podstawowe operacje teoriomnogościowe na zbiorach liczb całkowitych. Zbiory w zadaniu konstruujemy za pomocą specyficznych ciągów arytmetycznych.

Zbiory będziemy reprezentować za pomocą typu `struct zbior_ary`.

Do dyspozycji są dwa konstruktory zbiorów:

- `zbior_ary ciag_arytmetyczny(int a, int q, int b);`  
  Tworzy zbiór będący skończonym ciągiem arytmetycznym o różnicy `q > 0`, o zadanym elemencie początkowym `a` i końcowym `b`. Mamy `a mod q = b mod q` i `a <= b`. **Różnica `q` będzie taka sama we wszystkich wywołaniach konstruktora `ciag_arytmetyczny` w danym teście.**

- `zbior_ary singleton(int a);`  
  Tworzy zbiór złożony z tylko jednej liczby całkowitej `a`.

Operacje na zbiorach, jakie należy zaimplementować, to:

- `zbior_ary suma(zbior_ary A, zbior_ary B);`  
  Wynikiem funkcji jest suma teoriomnogościowa zbiorów `A` i `B`.

- `zbior_ary iloczyn(zbior_ary A, zbior_ary B);`  
  Wynikiem funkcji jest część wspólna zbiorów `A` i `B`.

- `zbior_ary roznica(zbior_ary A, zbior_ary B);`  
  Wynikiem funkcji jest różnica zbiorów `A` i `B`.

Ponadto należy zaimplementować następujące funkcje:

- `bool nalezy(zbior_ary A, int x);`  
  Funkcja sprawdza, czy liczba `x` należy do zbioru `A`.

- `unsigned moc(zbior_ary A);`  
  Funkcja daje w wyniku liczbę elementów zbioru `A`. Możesz założyć, że wynik funkcji zawsze zmieści się w typie.

Dla danego zbioru A oraz liczby q, przez `Ary_q(A)` oznaczamy minimalną liczbę parami rozłącznych ciągów arytmetycznych o różnicy q, których sumą jest zbiór A.

Na przykład `Ary_5({1, 2, 4, 7, 9, 12, 19}) = 4`, ponieważ zbiór ten jest sumą czterech ciągów arytmetycznych o różnicy 5:

- `{1}`,
- `{2, 7, 12}`,
- `{4, 9}`,
- `{19}`.

Powinieneś zaimplementować jeszcze jedną funkcję:

- `unsigned ary(zbior_ary A);`  
  Funkcja dla danego zbioru A daje w wyniku liczbę `Ary_q(A)`, dla zadanego w teście parametru q.

W Twojej implementacji:

- Złożoność czasowa funkcji `suma(A, B)`, `iloczyn(A, B)` i `roznica(A, B)` powinna być proporcjonalna do `Ary_q(A) + Ary_q(B)`.
- Złożoność funkcji `nalezy(A, x)` powinna być `O(log(Ary_q(A)))`.
- Złożoność funkcji `moc(A)` i `ary(A)` powinna być w najgorszym razie `O(Ary_q(A))`.
- Do przechowywania zbioru A należy użyć `O(Ary_q(A))` pamięci.

Rozwiązania nieoptymalne czasowo lub pamięciowo, ale poprawne, mogą liczyć na punkty częściowe.

Argumentami powyższych funkcji będą liczby typu `int`. Parametr `q` będzie liczbą dodatnią i we wszystkich wywołaniach funkcji `ciag_arytmetyczny` w danym teście będzie taki sam. Ponadto funkcja `ciag_arytmetyczny` zostanie wywołana jako pierwsza, dzięki czemu Twoja biblioteka będzie mogła od razu poznać parametr `q`.

Wyżej opisane funkcje znajdują się w pliku [`zbior_ary.h`](./zbior_ary.h). Twoim zadaniem jest zaproponować reprezentację zbiorów jako `struct zbior_ary`, która umożliwia efektywne wykonywanie operacji zgodnie z powyższymi wymaganiami.

Twoje rozwiązanie ma być umieszczone w pliku o nazwie `zbior_ary.c` i pasować do specyfikacji interfejsu `zbior_ary.h`. Należy również załączyć plik `zbior_ary.h` uzupełniony o definicję typu `struct zbior_ary`.

Kilka przykładowych operacji na zbiorach możesz znaleźć w pliku [`ocen.c`](./ocen.c).

Komenda kompilacji podana w treści zadania:

```bash
gcc @opcje ocen.c zbior_ary.c -o ary -lm
```

Pliki `zbior_ary.c`, `zbior_ary.h`, `ocen.c` oraz `opcje` muszą znajdować się w bieżącym katalogu.

Opcja `-lm` dołącza bibliotekę matematyczną. Powinna znajdować się na końcu komendy kompilacji. W rozwiązaniu nie musisz używać funkcji z `math.h`, ale możesz.

W tym zadaniu nie musisz przejmować się końcowym zwalnianiem pamięci.

## Authors

- Implementation — Maciej Mokrzycki.
- Code review — Oskar Skoczylas.
