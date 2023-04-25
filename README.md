# FISHHERD
Simulation de Boids en 2D utilisant [la bibliothèque P6 développée par Jules Fouchy](https://github.com/JulesFouchy/p6)

## Compilation
J'en parlerai un jour

## Simulation de boids
### Qu'est-ce que c'est ?
Une simulation de biods est un modèle informatique qui imite les processus biologiques ou les systèmes complexes trouvés dans la nature. Elle est utilisée dans divers domaines pour étudier, comprendre et modéliser des phénomènes biologiques, tels que les écosystèmes, les maladies infectieuses, la biophysique des cellules, la robotique inspirée de la biologie, et bien plus encore.
Lors de leurs déplacement les boids doivent respecter trois règles : 
- **Séparation** : Les boids évitent de se rapprocher trop près les uns des autres, ce qui simule l'évitement des collisions et maintient une distance de sécurité.
- **Alignement** : Les boids se dirigent dans la même direction que leurs voisins les plus proches, ce qui simule l'alignement des mouvements et crée un comportement de groupe cohérent.
- **Cohésion** : Les boids se dirigent vers le centre de masse de leurs voisins les plus proches, ce qui simule la cohésion du groupe et empêche sa dispersion.

### Paramètres de la simulation
- **Size** : Changement de la taille de la queue. *Valeur recommandée : 0.04.*
- **Speed** : Modification de la vitesse des boids. *Valeur recommandée : 0.01.*
- **Turn factor** : Capacité pour les boids à pouvoir tourner, une valeur plus élevée leur permet de tourner plus facilement et inversement. *Valeur recommandée : 1.*
- **Separation radius** : Distance à partir de laquelle les boids cherchent à se séparer les uns des autres. *Valeur recommandée : 0.25.*
- **Separation strength** : Force avec laquelle les boids vont se séparer les uns des autres. *Valeur recommandée : 0.0048.*
- **Alignment radius** : Distance à partir de laquelle les boids cherchent à s'aligner avec les autres. *Valeur recommandée : 0.5.*
- **Alignement strength** : Force avec laquelle les boids vont s'aligner avec les autres. *Valeur recommandée : 0.0024.*
- **Cohesion radius** : Distance à partir de laquelle les boids vont se cohésionner (?) entre eux. *Valeur recommandée : 0.75.*
- **Cohesion strenght** : Force avec laquelle les boids vont se cohésionner entre eux. *Valeur recommandée : 0.005.*
- **Walls radius** : Distance à partir de laquelle les boids vont prendre en compte les murs (les bordures de la fenêtre). *Valeur recommandée : 0.5.*
- **Walls strength** : Force avec laquelle les boids vont éviter les murs. *Valeur recommandée : 0.0032.*
- **Mouse strength** : Définit à quel point le pointeur de souris déstabilise les boids. . *Valeur recommandée : 0.0196.*
- **Show parameters** : Affiche clairement les paramètres *Separation radius*, *Alignment Radius* et *Cohesion radius*. Il est recommandé de mettre le paramètre *Size* au maximum avant d'activer ce paramètre. 

## FISHH3RD
Adaptation en 3D utilisant OpenGL 3 : [FISHH3RD](https://github.com/DANLCARTON/fishh3rd)
