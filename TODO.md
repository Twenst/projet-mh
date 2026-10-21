## Heuristique que l'on pourrait implémenter:
parcours du graphe en largeur ou profondeur, à chaque fois qu'on voit un sommet on lui donne la première couleur disponible, on peut dépasser le nombre maximal de couleurs comme ça mais pg.

---
## Idées de voisinages:
*   Changement de couleur pour un sommet
*   Échange de couleurs entre 2 sommets voisins

---
## Idées de MH à implémenter:
*   Recherche tabou: descente locale mais bannissement des solutions que l'on a exploré récemment (ou des assignations d'un sommet à une couleur)
*   Recuit simulé: le score étant le nombre de couleurs utilisées, on part de 1 couleur par sommet.