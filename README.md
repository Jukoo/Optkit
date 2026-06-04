### Optkit
**Optkit** est une bibliothèque C déclarative, légère et orientée événements pour le parsing d'options en ligne de commande, construite au-dessus de getopt.

Contrairement aux approches traditionnelles qui imposent de longs blocs de structures rigides et une maintenance manuelle fastidieuse, Optkit utilise une syntaxe fluide basée sur des macros pour définir vos options (un DSL en C) et délègue le traitement à une fonction de rappel (callback) hautement configurable.

La gestion des flux d'I/O et de l'aide textuelle s'appuie sur des hooks système via ```GNU fopencookie```, offrant une flexibilité totale pour capturer, rediriger ou tester les sorties de votre CLI sans polluer le terminal.

##### Fonctionnalités
- Déclaration Fluide (DSL en C) : Définissez vos options de manière visuelle et structurée grâce aux macros ```optkit_begin```, ```noarg```, ```rearg```, ```oparg``` et ```optkit_ends```.

- Architecture Orientée Événements : Un unique gestionnaire d'arguments ```(argument_handler)``` reçoit les options parsées à la volée.

- Passage de Contexte Sécurisé : Injection d'un pointeur de données utilisateur ```(void * extra_data)``` dans le callback, idéal pour lier des structures de configuration complexes sans utiliser de variables globales.

- Gestion Avancée des I/O avec ```(fopencookie)``` : L'aide automatique et les buffers de texte internes utilisent l'extension GNU de la glibc pour encapsuler proprement les flux de données.

- **Auto-Documentation intégrée : Génère automatiquement une documentation complète incluant un en-tête (header) et un pied de page (footer).**

Comment ça marche ? (Explication de l'exemple)
Voici l'analyse détaillée du fonctionnement d'Optkit basé sur le fichier d'exemple fourni :

```source :  [test.c](test.c)```
```C
//SPDX-License-Identifier: GPL-3.0 
#include <stdio.h> 
#include "optkit.h" 

// Le buffer d'aide global généré dynamiquement par Optkit
extern char * optkit_help; 

// 1. DÉCLARATION DES OPTIONS (Le bloc de configuration)
optkit_begin(options, "This program is a test\njust a illustration programm")  

noarg("help" ,  "show this help" , 'h') ,    // Option sans argument
rearg("output" ,"redirect output",  'O') ,   // Option avec argument REQUIS
oparg("input" , "redirect input")  ,         // Option avec argument OPTIONNEL

optkit_ends("The end footer should content the copyright\n,author and the version of the program") 

// 2. LE CALLBACK (Gestionnaire d'arguments)
void argument_handler(int * option , void * your_extra_data) 
{
    if(your_extra_data) {
        puts("has something inside"); 
    }

    switch(*option) 
    {
        case 'O': 
            puts("the big O notation"); 
            break; 
        case 'i':  
            // 'optarg' de la glibc reste accessible pour récupérer la valeur associée
            printf("Input data   %s \n" , optarg); 
            break; 
        default:
            // En cas d'option inconnue ou d'aide demandée, on affiche le menu généré
            printf("%s" , optkit_help); 
            break; 
    }
}

// 3. LE POINT D'ENTRÉE
int main(int ac , char * const *av) 
{
    int your_extra_data = 10; 
  
    // optkit_parse prend :
    // - Les arguments du terminal (av)
    // - Le bloc d'options déclaré plus haut (options)
    // - Le callback de traitement (argument_handler)
    // - Un pointeur vers vos données personnalisées (&your_extra_data)
    if(optkit_parse(av , options , argument_handler, (void *) &your_extra_data))  
        fprintf(stderr , "%s" , optkit_help);
 
    return 0; 
}
```  
En sortie:  
```bash

Usage : test_optkit :[OPTION]... [ARGS]...

This program is  a test
just a illustration programm

  -h, --help 		show this help
  -O, --output 		redirect output
  -i, --input 		redirect input

The end footer should content the copyright
,author and the version of the program

```
##### Décryptage de la syntaxe  :

```optkit_begin(nom, description)``` : Initialise un tableau d'options nommé options et y injecte la description principale qui servira d'en-tête pour le --help.

*Le [short] est optionel. Si on le Specifie pas il prendra par  defaut le premiere lettre de  --long*
  
Les types d'arguments :

```noarg(long, desc, [short])``` : Équivalent à no_argument. Déclenche l'option via --long ou -short.

```rearg(long, desc, [short])``` : Équivalent à required_argument. Nécessite une valeur associée (ex: -O valeur ou --output=valeur).

```oparg(long, desc, [short])``` : Équivalent à optional_argument.

```optkit_ends(footer)``` : Ferme proprement la structure de données et y ajoute les métadonnées de fin (Auteur, Version, Copyright).

optkit_parse(...) : C'est le moteur. Il parcourt la ligne de commande, assemble dynamiquement la documentation d'aide en mémoire à l'aide de flux gérés par fopencookie, et appelle votre fonction argument_handler pour chaque argument intercepté.


Ici l'avantage  technique est l'isolation I/O avec fopencookie. 
Plutôt que d'écrire l'aide ou les erreurs directement sur les flux standards (stdout/stderr) de manière brute, Optkit utilise fopencookie pour ouvrir un flux FILE * customisé.

Zéro allocation de chaînes de caractères complexes : Optkit utilise les fonctions d'écriture standard (fprintf) sur son flux customisé. fopencookie intercepte ces écritures via des hooks mémoire pour alimenter la variable globale optkit_help.

Testabilité unitaire : Vous pouvez simuler des entrées et capturer l'aide générée au sein de vos tests unitaires sans jamais interagir avec le vrai terminal système (pas de pollution de stdout).

### Intégration & Compilation

Le projet utilise le système de build moderne Meson. Pour compiler la bibliothèque et l'exemple fournis, exécutez simplement les commandes suivantes :

######  1. Configurer le répertoire de build

```meson setup build```
######  2. Compiler le projet
```meson compile -C build```

Note : Comme fopencookie est une extension GNU spécifique à la glibc, le fichier meson.build configure automatiquement les drapeaux de compilation nécessaires (-D_GNU_SOURCE ou std type gnu11).


#### Licence
Ce projet est distribué sous la licence GPL-3.0.

Copyright (c) 2026, Umar Ba jUmarB@protonmail.com
