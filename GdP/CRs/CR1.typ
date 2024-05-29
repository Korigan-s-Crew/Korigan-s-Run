#import "settings.typ": *
#let preset(body)={
  //Numéro et date du CR
  let CR = "N"
  let date = "20/02/2024"

  //membres présents et absents (si il n'y a qu'un seul nom, ajouter une virgule)
  let presents = ("Malo", "Aristide", "Baptiste", "Gauthier")
  let absents= ()

  show: init_cr.with( CR: CR, date: date, presents: presents, absents: absents)
  body
}
#show: preset.with()

//Remplir à partir d'ici
= ODJ
#table(
  columns: (5cm, 1fr),
  stroke: none,
  [*Concept du jeu*], [],
  [*premiere reflexion sur l'aspect technique*], []
)


= Concept du jeu
  Jeu platformeur 2D : a chaque nouvelle partie un monde d'une longueur finie est géneré. \
  Objectif: arriver le plus vite possible a la fin des niveaux.\
  generation procedurale pour créer les niveaux

  la 2D revient dans toutes les idees \
  suggeré: mario qui part dans tous les sens (peut reculler, avancer ...)\
#important(titre: "iddée retenue", 
"plateformer 2D
  Salles prédefinies (type boss ou challenge) et chemins entre les salles générées aleatoires (generation procédurale)
  les salles sont choisi dans un pool de salles par niveaux
  style pixel art ")

= Aspect techniques
  
  c, affichage sdl, géneration procedurale de terrain, ia pour les boss\
  Gestion des terrains pour la structure de données



