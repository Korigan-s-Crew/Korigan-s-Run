#import "settings.typ": *
#let preset(body)={
  //Numéro et date du CR
  let CR = "&"
  let date = "JJ/MM/YYYY"

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
  [*ODJ1*], [précision 1],
  [*ODJ2*], [Précision 2]
)
#pagebreak()

= Concept du jeu
  


= Point 2
  == Sous-point 2.1
    #lorem(30)

  == Sous-point 2.2
    #lorem(30)

  #important(titre: "Décision", "il se passe des trucs ")


