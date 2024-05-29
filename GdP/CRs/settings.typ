#import "@preview/showybox:2.0.1":showybox

//Parametres généraux
#let colorTheme = rgb("#6a487e")
#let club = "PP2I 2"
#let year = "2024"
#let logo = "data/Logo.jpg"

//Membres réccurents et raccourcis

//Définition des membres du club

//Fonctions pour que ce soit joli
#let project(club: club, year: "", CR: "", date: "", body) = {
  set page(
    paper: "a4",
    numbering: "1",
    header: [#table(
        columns: 100%,
        stroke: none,
        showybox(
          frame: (body-color: colorTheme, border-color: white, radius: 3pt),
          [ #align(center)[#text(white, 14pt)[#club #year -- Compte Rendu #CR -- #date]] 
          ]
        ),
      )
    ],
    footer: [#table(columns: (92%, 8%), stroke: none, [], showybox(
        frame: (body-color: colorTheme, border-color: white, radius: 3pt),
        [#align(center)[
            #text(white, 15pt)[
              #locate(loc => {
                counter(page).display("1")
              })
            ]
        ]],
    ))],
  )
  [#table(
      columns: 100%,
      stroke: none,
      [])]
  body
}

#let maketitle(CR: "", date: "", year: "") = {
  let title = [\ #club #year \ Compte rendu #CR \ #date \ \ ]
  [#table(
      columns: 100%,
      stroke: none,
      [#showybox([#text(colorTheme)[
            #text(30pt)[
              #align(center)[
                #title
              ]
            ]
          ]
        ])
      ]
      ,[],[],[],
      [#align(center)[#image(logo, width: 90%)]],
      
    )
  ]
}

#let membres(presents: "", absents: "") = {
  grid(
  columns: (1fr, 1fr),
  rows: (auto, auto),

  [*#underline()[#text(15pt)[Membres présents :]]*
    #{for p in presents {
    [- #p]
   } }
  ],

  [*#underline()[#text(15pt)[Membres absent :]]*
    #{for a in absents {
    [- #a]
   } }
  ],
  )
}

#let init_cr(CR: "", date: "", presents: "", absents: "", body) = {
  show: project.with(year: year, CR: CR, date: date)
  show heading.where(level: 1): set text(25pt)
  show heading.where(level: 2): set text(20pt)
  show heading.where(level: 2): it => [
  #h(1cm) #counter(heading).display() #it.body \ ]
  show heading.where(level: 3): set text(15pt)
  show heading.where(level: 3): it => [
  #h(2cm) #counter(heading).display() #it.body \ ]
  maketitle(CR: CR, date: date, year: year)
  pagebreak()
  

  outline(title: "Table des matières", indent: auto)

  heading("Membres")
  membres(presents: presents, absents: absents)
  //listMembrereu1()

  set heading(numbering: "I/1/i/")
  body
}

#let important(titre: "", body) = {
  showybox(
  frame: (
    border-color: colorTheme.darken(60%),
    title-color: colorTheme.lighten(40%),
    body-color: colorTheme.lighten(80%)
  ),
  title-style: (
    color: black,
    weight: "regular",
    boxed-style: (anchor: (x: left) )

  ),
  title: titre,
  body
  )
}

