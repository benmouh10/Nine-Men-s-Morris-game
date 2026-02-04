Jeu de Moulin (Nine Men's Morris) en Langage C
Ce projet est une implémentation numérique du Jeu de Moulin (également connu sous le nom de Nine Men's Morris), un jeu de stratégie classique pour deux joueurs. Développé en Langage C, il propose une interface en console structurée et plusieurs modes de jeu, allant du duel local à l'affrontement contre une intelligence artificielle avancée.

🎮 Présentation du Jeu
Le Jeu de Moulin se joue sur un plateau de 24 intersections reliées par des lignes.


Objectif : Former des "moulins" (alignements de trois pions) pour capturer les pièces de l'adversaire.


Victoire : La partie se termine lorsqu'un joueur est réduit à seulement deux pièces ou qu'il ne peut plus effectuer de mouvement valide.

🚀 Fonctionnalités
Le projet intègre trois modes de jeu principaux :


Joueur contre Joueur : Deux joueurs humains s'affrontent localement.


Joueur contre IA (Aléatoire) : Un mode simple où la machine joue des coups au hasard.


Joueur contre IA (Avancée) : Une machine intelligente utilisant des stratégies offensives (compléter des moulins) et défensives (bloquer l'adversaire).

🛠️ Architecture Technique
🖥️ Interface Utilisateur
L'affichage en console a été optimisé pour la lisibilité grâce à des fonctions manipulant l'interface Windows :


gotoxy(x, y) : Positionne le curseur pour un affichage structuré.


setColor / setColors : Utilise des couleurs distinctes pour différencier les pions des joueurs et les lignes du plateau.


Plateau : Représenté par un tableau 2D de caractères Board[43][17].

🧠 Intelligence Artificielle (IA)
L'IA avancée suit une hiérarchie de décision prioritaire :

Phase de Placement :

Tente de compléter un moulin pour capturer une pièce.

Bloque les moulins potentiels de l'adversaire.

Place des pièces à proximité de ses propres pions pour préparer de futurs moulins.


Phase de Déplacement : Utilise des fonctions comme movepiecemachine1 pour privilégier les cases stratégiques et les intersections clés.

📂 Structure du Code
Le projet est découpé en plusieurs fonctions modulaires :


Gestion du plateau : Display, showavailibleboxes.


Logique de jeu : millverifier (détection de moulins), TheMoveIsValid, takeout.


Phases de jeu : Player_Player, PlayerVsMachine0, PlayerVsMachine1.

⚙️ Installation et Exécution
Assurez-vous d'avoir un compilateur C (comme GCC) installé.

Compilez le fichier source :

Bash
gcc main.c -o jeu_de_moulin
Lancez le programme :

Bash
./jeu_de_moulin

Note : Ce projet utilise des bibliothèques spécifiques à Windows (windows.h) pour la gestion de la console (couleurs et positionnement du curseur).
