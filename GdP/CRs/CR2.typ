#import "settings.typ": *
#let preset(body)={
  //Numéro et date du CR
  let CR = "2"
  let date = "07/03/2024"

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
  [*Ce qui a été fait*], [],
  [*Repartition des tâches*], []
)


= Ce qui à été fait
- Baptista à développé un premier moteur graphique, avec boucle de jeu dans lequel on peut se déplacer
- Malo à commencé à dessiner des premieres textures de sol
- on à toujours pas envoyé le mail pour déclarer nitre groupe/sujet
  


= Repartition des tâches
- Baptiste s'occuppera de tout ce qui est moteur de jeu au sen général : il à déja commencé
- Malo de la partie "visuelle" du jeu : gestion des sprites,du terrain, DA : attrait pour le dessin et le visuels, grosse partie de gestion dans le code à faire
- Aristide s'occuperra de la partie déplacements ainsi que des entités
- gauthier s'occuperra de la création des maps


= TO DO 
- tout le monde : se renseigner sur l'etat actuel avec le code de Baptiste
- malo : 
  - développement des premiers sprites et integration dans le code
  - envoi du mail de présentation du groupe
- gauthier : renseignement sur comment fonctionne la génération procédurale
- Aristide : déplacements


