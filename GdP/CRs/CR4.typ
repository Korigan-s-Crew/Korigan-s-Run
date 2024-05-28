#import "settings.typ": *
#let preset(body)={
  //Numéro et date du CR
  let CR = "4"
  let date = "07/05/2024"

  //membres présents et absents (si il n'y a qu'un seul nom, ajouter une virgule)
  let presents = ("Malo", "Aristide", "Baptiste", "Gauthier")
  let absents= ()

  show: init_cr.with( CR: CR, date: date, presents: presents, absents: absents)
  body
}
#show: preset.with()

//Remplir à partir d'ici

= Fait
Les maps sont en cours d'implementation dans l'ancien code;\
Textures de nuages, ecran d'acceuil et niveau tutoriel
  


= To do
  - Finir l'implementation des maps
  - Dernieres textures manquantes (dash)
  -timer et ecran de "fin" pour donner un but au jeu
  - peut etre des entités


