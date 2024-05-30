# PP2I2 - Korigan's run

Un jeu plateformer fonctionant avec SDL

# Prérequis

```bash
sudo apt install -y libsdl2-dev clang libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

# Comment lancer le programme ?

```bash
make run
```

# Modes de jeu
Deux modes sont proposés : 
- Le tutoriel, accessible en appuyant sur t où en cliquant sur le bouton "tuto" depuis le menu d'acceuil.
- le mode de jeu principal, accessible en appuyant sur e où en cliquant sur le bouton "start" depuis le menu d'acceuil.
Le but est de finir le plsu vite possible trois maps, en utilisant les differentes mechaniques de déplacement.

# Contrôles
## En jeu
| touche |    action    |
| ------ | ------------ |
|   q    |    droite    |
|   d    |    gauche    |
| espace |     saut     |
|   s    |     bas      |
| shift  |     dash     |
|  F11   | mode fenêtre |
|   m    |   mode dev   |
|   k    |   lancer/arrêter la musique    |
|  tab   | passe les contrôles en qwerty |
## Mode développeur
| touche |    action    |
| ------ | ------------ |
|   p    |  accélerer   |
|   o    |   ralentir   |
|   l    | dash infini  |
## Ecrans d'acceuil et de fin
| touche |    action    |
| ------ | ------------ |
|   e    |    start     |
|   t    |   tutoriel   |
| escape |   quitter    |

# déplacement
## Descente
- Sur le sol, descendre permet de s'accroupir.
- si le personnage tombe en se déplaçant, descendre déclenche un slide.
- sur un nuage, decendre permet de passer à travers.

## Murs spéciaux
On peut s'attacher aux murs spéciaux (bleu et orange)
- maintenir juste la touche de déplacement latéral permet de decendre lentement.
- maintenir en plus espace permet de rester fixe sur le mur.
- relacher le coté en maintenant espace déclenche un saut latéral dans l'autre sens.

## slide
Déclencher un slide permet de se déplacer rapidement en faisant seulement 1 bloc de haut et de sauter de 1 bloc de plus en hauteur.

## dash
Il peut s'effectuer dans 8 direction : droite, gauche, haut et bas et les intermédiares. Il y a un cooldown visible sur le ponpon du personnage (jaune quand on peut faire un dash, bleu en cooldown).
