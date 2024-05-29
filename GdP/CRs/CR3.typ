#import "settings.typ": *
#let preset(body)={
  //Numéro et date du CR
  let CR = "&"
  let date = "27/03/2024"

  //membres présents et absents (si il n'y a qu'un seul nom, ajouter une virgule)
  let presents = ("Malo", "Aristide", "Baptiste", "Gauthier")
  let absents= ()

  show: init_cr.with( CR: CR, date: date, presents: presents, absents: absents)
  body
}
#show: preset.with()

//Remplir à partir d'ici


= Avancement
- Le moteur de jeu, ainsi que les déplacements, sont globalement terminés. Aristide explique les subtilités ajoutés aux déplacement:
  + le dash peut se faire dans plusieures directions, avec un cooldown
  + le slide permet su deplacemet rapide tout en etant baissé, et on saute plus haut apres yn slide
- Le systeme de texture de maps à été automatisé : au lieu de devoir placer soi meme les blocs, seuls quelques differentiation sont nécessaire (nuage/sol/mur pour wall jump) et les bons blocs sont passés automatiquement pour avoir une map ésthetique
  


= Discussion sur la génération des maps
La question se pose entre un algorithme de generation procédurale et un generation des maps par paterns; gauthier se voi mieux faire des paterns; on concatene des paterns en .txt dans un seul fichier .txt pour conserver les algos deja fourni. Pour l'insatnt, 6 paterns par map.

= To do 
- Malo :
  - textures : nuages, slide, dash
  - implementation d'un ecran d'acceuil et de fin 
- Gauthier et Baptiste: 
  - Maps et implementation dans le code actuel
Aristide : 
  - maps technique qui exploitent les deplacements
  - certains fix de deplacements
