### Optkit
**Optkit** est une bibliothèque C déclarative, légère et orientée événements pour le parsing d'options en ligne de commande, construite au-dessus de getopt.

Contrairement aux approches traditionnelles qui imposent de longs blocs de structures rigides et une maintenance manuelle fastidieuse, Optkit utilise une syntaxe fluide basée sur des macros pour définir vos options (un DSL en C) et délègue le traitement à une fonction de rappel (callback) hautement configurable.

Sous le capot, la gestion des flux d'I/O et de l'aide textuelle s'appuie sur des hooks système via GNU fopencookie, offrant une flexibilité totale pour capturer, rediriger ou tester les sorties de votre CLI sans polluer le terminal.

##### Fonctionnalités Clés
Déclaration Fluide (DSL en C) : Définissez vos options de manière visuelle et structurée grâce aux macros optkit_begin, noarg, rearg, oparg et optkit_ends.

Architecture Orientée Événements : Un unique gestionnaire d'arguments (argument_handler) reçoit les options parsées à la volée.

Passage de Contexte Sécurisé : Injection d'un pointeur de données utilisateur (void * extra_data) dans le callback, idéal pour lier des structures de configuration complexes sans utiliser de variables globales.

Gestion Avancée des I/O (fopencookie) : L'aide automatique et les buffers de texte internes utilisent l'extension GNU de la glibc pour encapsuler proprement les flux de données.

Auto-Documentation intégrée : Génère automatiquement une documentation complète incluant un en-tête (header) et un pied de page (footer).

🛠️ Comment ça marche ? (Explication de l'exemple)
Voici l'analyse détaillée du fonctionnement d'Optkit basé sur le fichier d'exemple fourni :

```C
//SPDX-License-Identifier: GPL-3.0 
/*-- This is a test file And you can refer to this as example ---  
 * This file answer the Question how to use it ?? 
 * Copyright(c) 2026 , Umar Ba <jUmarB@protonmail.com> 
 */ 

#include <stdio.h> 
#include "optkit.h" 

// Le buffer d'aide global généré dynamiquement par Optkit
extern char * optkit_help; 

// 1. DÉCLARATION DES OPTIONS (Le bloc de configuration)
optkit_begin(options, "This program is a test\njust a illustration programm")  

noarg("help" ,  "show this help" , 'h') ,   // Option sans argument
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

##### Décryptage de la syntaxe :
optkit_begin(nom, description) : Initialise un tableau d'options nommé options et y injecte la description principale qui servira d'en-tête pour le --help.

Les types d'arguments :

```noarg(long, desc, short)``` : Équivalent à no_argument. Déclenche l'option via --long ou -short.

```rearg(long, desc, short)``` : Équivalent à required_argument. Nécessite une valeur associée (ex: -O valeur ou --output=valeur).

```oparg(long, desc)``` : Équivalent à optional_argument.

```optkit_ends(footer)``` : Ferme proprement la structure de données et y ajoute les métadonnées de fin (Auteur, Version, Copyright).

optkit_parse(...) : C'est le moteur. Il parcourt la ligne de commande, assemble dynamiquement la documentation d'aide en mémoire à l'aide de flux gérés par fopencookie, et appelle votre fonction argument_handler pour chaque argument intercepté.


L'avantage  technique est l'isolation I/O avec fopencookie. 
Plutôt que d'écrire l'aide ou les erreurs directement sur les flux standards (stdout/stderr) de manière brute, Optkit utilise fopencookie pour ouvrir un flux FILE * customisé.



> Zéro allocation de chaînes de caractères complexes : Optkit utilise les fonctions d'écriture standard (fprintf) sur son flux customisé. fopencookie intercepte ces écritures à bas niveau via des hooks mémoire pour alimenter la variable globale optkit_help.

> Testabilité unitaire : Vous pouvez simuler des entrées et capturer l'aide générée au sein de vos tests unitaires sans jamais interagir avec le vrai terminal système (pas de pollution de stdout).

### Intégration & Compilation
Comme fopencookie est une extension GNU, assurez-vous que la macro de test de fonctionnalité _GNU_SOURCE soit active lors de la compilation de votre projet.

``` Bash
 $ gcc -D_GNU_SOURCE main.c optkit.c -o mon_programme
```

#### Licence
Ce projet est distribué sous la licence GPL-3.0.

Copyright (c) 2026, Umar Ba jUmarB@protonmail.com
