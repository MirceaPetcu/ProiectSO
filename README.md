Contribuitori:

  -Lem-Rau Dumitru
  
  -Petcu Mircea
  
  Proiectul consta in sincronizare sistemului local de fisiere cu contul de dropbox. O data ce un fisier/director din sistemul local de fisiere este sincronizat cu
contul de dropbox, toate actiunile valabile in mediul dbxcli vor fi efectuate si pe sistemul local. In plus, toate comenzile ce nu se regasesc in dropbox, dar se pot 
folosi in mediul Linux, vor efectuate atunci cand se apeleaza. Avantajul principal al proiectului este ca se poate incarca un director in dropbox cu tot cu 
continutul de pe primul nivel al acestuia. Fisierele/Directoarele nesincronizate cu dropbox vor putea fi in continuare operate, cu ajutorul comenzilor Linux, pe 
sistemul local.

  Pentru rularea acestui proiect va fi nevoie de crearea unui cont de dropbox (https://www.dropbox.com/home) si instalarea fisierului dbxcli specific sistemului personal de calcul din sursa:
https://github.com/dropbox/dbxcli/releases.

  Se va genera un token cu care se va putea conecta sistemul local de fisiere cu mediul dropbox.
