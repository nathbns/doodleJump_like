# doodleJump_like

## BEN SOUSSAN Nathan p2300345

###### L'objectif général de l'application est de créer un jeux du type "doodleJump". Dans l'application nous jouons un joueur qui peut se déplacer à l'aide des flèche du clavier, notre joueur subis divers force dans le monde qu'il parcourt. 

# Commande de jeu :
Pour lancer une partie à partir de la page "PRESS START", appuyer sur **s** ou faire un **click droit**, pour ce déplacer on utilise **les flèches du clavier** (gauche et droite) et pour tuer les monstres vers un **click gauche court ou prolongé**
Ne pas hésité à modifier les constante dt (influe sur la vitesse de rebond du joueur) et vitesseJeu (toutes les vitesses qui influe sur les objects du jeu). Normalement, elle sont bien paramétré.

# Règle du jeu :
* Toucher le bas de la fenetre -> GAME OVER
* Toucher un monstre -> GAME OVER
* Sauter sur une platforme classique, qui bouge latéralement (+5), toutes les cassées (+10) et tuer un monstre (+20)
* Les boucliers, protège temporairement le joueur des monstres

# Explication des grandes lignes du code :

* Une particule avec les forces habituelles représente notre joueur, illustré ici avec une image.
* Les plateformes, positionné aléatoirement sur la fenetre, peuvent-etre de type classique, "broken" c'est à dire cassé (lorsque la particule monte dessus elle subit la gravité et tombent en bas de la fenetre), elle peut-etre de type "moving" (la plateforme fait des aller et retour dans la largeur de la fenetre) et elle peut-etre de type "jump" (la plateforme fais rebondir le joueur plus fortement). De plus, une plateforme peut-etre à la fois de type "moving" et "broken".
* L'effet de jeu à "l'infinie", est codé de cette manière: lorsqu'une plateforme sort de la fenetre de jeu, elle est immédiatement repositionné au plus haut de la fenetre de jeu.
* L'effet de défilement vers le haut lorsque le joueur monte sur les plateformes est codé de cette façon: j'ai fais en sorte que toutes les plateformes descendent vers le bas de la fenetre de manière constante mais très faiblement. Et en plus de cela, lorsque le joueur monte sur une plateforme, je fais descendre toutes les plateformes une nouvelle fois mais cette fois-ci d'un seul coup.
* Le joueur peut rebondir sur une plateforme que lorsque qu'il vient du haut de celle-ci. En effet, lorsque le joueur rebondissait sur des plateformes en venant du bas de celle-ci, cela donnait un rendu moins fluide.
* Des troues noirs qui lorsque l'on s'approche nous attire. Pour cela je calcule la distance entre le joueur et le troue noir et ensuite lorsqu'il est dans le rayon j'active les forces, s'il n'est plus assez proche alors je réinitialise les force que le joueur subissent afin que l'on puisse recommencer à jouer si nous n'avons pas perdue
* Pour mettre en place les projectiles qui peuvent tuer les monstres. J'ai crée une structure projectile, qui prend différent paramètre. J'ai également fait une interpolation de couleur pour les projectiles (les couleur évolue au fur et à mesure de la vie du projectile). Les projectiles sont des triangles qui effectue une rotation jusqu'à mourir ou tuer un monstre.
* Un bouclier tombent de la fenetre, lorsque le joueur le touche un cercle avec une certaines épaisseur apparait autour de notre joueur et le protège temporairement des monstres.
* Lorsque le joueur se situe ] 1/3 de la page, 3/4 de la page [ alors la vitesse de jeu augmente de 50%, au dessus de cela c'est 70% et sinon s'il est < 1/3 de la page alors c'est la vitesse prédéfinie dans les constantes

# Historique des évolutions :

* Sur cette première semaine de mini-projet, j'ai pu mettre en forme la base du code qui va me permettre d'implémenter de nouvelles forces et intéractions (mathématique) dans le jeu.
* Deuxième semaine : Mise en place des troues noirs, mise en place des projectiles qui tue les monstres, corrections des mouvements de toutes les plateformes (Au début les platformes cassées qui tombait après etre sauter dessus ne se générait par correctement). D'ici la semaine prochaine, je veux rajouter des sortes de trampoline sur lequel on peut sauter et ensuite propulser (+ d'autres idées à venir).
* 3ème semaine : -> Mise en place de nouvelle plateforme "jump" (verte), lorsque le joueur saute dessus, il rebondit plus haut que sur les autres plateformes. -> Modification des projectile, qui était une jet de particules, maintenant ce sont des triangle en rotation (1 projectile par projectile). -> Mise en place des bouclier qui lorsque le joueur le touche est protégé des monstre. -> Gestion de la vitesse au fonction de la position du joueur dans la fenetre (voir explication code)

# Références : 

* Wikipedia, github 
