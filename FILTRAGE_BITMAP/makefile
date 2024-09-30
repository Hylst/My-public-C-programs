CC = gcc                  # Variable représentant le nom du compilateur.
CFLAGS = -Wall -I include # Variable représentant les options de compilation.
LDFLAGS = -L lib -lZDS    # Variable représentant les options d’édition de liens. 


Programme : filtrage_bitmap.O        # Programme dépend de `main.o`.
    $(CC) filtrage_bitmap.o -o Programme $(LDFLAGS)

filtrage_bitmap.o : src/filtrage_bitmap.c       # `main.o` dépend de `src/main.c`.
    $(CC) $(CFLAGS) -c src/filtrage_bitmap.c -o filtrage_bitmap.o

# La syntaxe se comprend en remplaçant les variables par leur valeur : $(CC) → gcc.
