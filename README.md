# Prova Finale di Algoritmi e Principi dell'Informatica


Politecnico di Milano A.A. 2022-2023

Valutazione 30L/30

### Obiettivo del progetto

Implementazione delle strutture dati e dell'algoritmo per gestire le auto, le autostrate e i percorsi a tapper minime come dettagliatamente descritto nelle [specifiche](https://github.com/sara-leka/Algorithms-and-Data-Structures-Project-API-2022-2023/edit/main/requirements-ita.pdf)  

L'implementazione in linguaggio C è ottimizzata sia nei tempi di esecuzione sia ne consumo di memoria. 
A tale fine sono stati usati alberi binari Red&Black per organizzare le stazioni nell'autostrada e un VLA per le auto nelle stazioni. 
A livello di efficienza temporale il collo di bottiglia è la ricerca del percorso, per cui l'algoritmo implementato ha complessità $O(n)$.

### Criteri di valutazione

In fase di sviluppo sono stati offerti a supporto dei test case aperti. Dopo la consegna il codice è stato testato su ulteriori test case per coprire tutti i casi limite ed è stato vlautato in base alle performance come descritto in tabella.


| Voto | Time Limit [s] | Memory Limit [MiB] | 
| -------- | -------- | -------- |  
| 18 | 19,00 | 128,00 | 
| 21 | 15,00 | 118,00 | 
| 24 | 10,00 | 108,00 | 
| 27 | 6,00 | 98,00 | 
| 30 | 4,00 | 88,00 |
| 30L | 1,00 | 78,00 |

#### I risultati di questo codice sono stati:

Execution Time 0.337 s

Used Memory 40.6 MiB
