# ANI-3D

AUTHORS :
- chloé.magnier
- adam.ismaili

# Introduction

Dans le cadre de notre projet d'Animation 3D, nous avons repris le TP 6 concernant la simulation de l'eau qui nous avait beaucoup plu.

Nous nous sommes dits que ce TP pouvait être amélioré, tant au niveau de ses performances que des options offertes par son interface.

Nous avons alors décidé de l'optimiser spacialement, en plus d'ajouter certaines fonctionnalités interessantes à la simulation.

# Ajout d'une grille pour l'optimisation spaciale

Nous avons remarqué que les particules étaient toutes traitées à chaque update à l'aide de boucles intriquées.

Pour éviter cela, nous avons choisi de remplacer ce fonctionnement par une grille (provenant de la classe "grid") stockant les particules dans des cellules.

Cela nous permet d'éviter de retraiter chaque particule, mais juste celles dont les paramètres changent ou se trouvant dans le rayon d'influence de la particule traitée, de la taille d'une cellule.

# Contrôle du nombre, de la taille et de la distance d'influence d'une particule

En termes d'options que nous avons jugé interessantes à ajouter, nous avons implémenté un slider permettant de changer le rayon d'influence d'une particule.

Son curseur est positionné par défaut à 2, et peut se mouvoir de 1 à 3.

Plus le rayon diminue, plus la place disponible pour stocker des particules augmente, plus le nombre de particules augmente au prochain reset. Et inversement.

# Contrôle de la direction de déplacement des particules (prédeterminée ou aléatoire)

Nous avons aussi jugé interessant d'ajouter deux boutons pour reset la position des particules.

Le premier se nommant "Reset simulation" sert à reset la position des particules avec des directions prédeterminée de déplacement.

Le second se nommant "Random simulation" sert à reset la position des particules avec des directions aléatoires de déplacement.

Ces options nous servent à vérifier que la simulation reste vraisemblable avec des conditions initiales différentes.

# Contrôle du mouvement des particules à l'aide de touches de clavier

