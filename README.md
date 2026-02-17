# scannerprt
Un scanner multithreadé en C++ qui scanne les 65535 ports en moins de 20 secondes, je travaille sur une version plus intelligente du scanner pour le moment, je mettrai une autre version future sur ce repos.

Ce scanner de ports en C++ pour Windows (WinSock2) est conçu pour la vitesse. Il peut analyser 65 535 ports en moins de 10 secondes grâce à une architecture multithreadée optimisée.

Le programme utilise un système de threads simultanés (1000 threads) pour tester plusieurs ports en même temps. Cela évite d'attendre la réponse d'un port avant de passer au suivant.

Pour garantir une vitesse maximale, les sockets sont configurés en mode non-bloquant avec FIONBIO. Cela permet de ne pas rester figé sur les ports qui ne répondent pas.

La gestion du temps est assurée par la fonction select. Elle permet de définir un timeout manuel très court (200ms) pour chaque tentative de connexion, ce qui accélère drastiquement le scan global.

L'affichage est sécurisé par un mutex. Cela empêche les différents threads de mélanger leurs textes dans la console, garantissant une lecture claire des ports ouverts.

Une version plus intelligente est en cours de développement pour intégrer le Banner Grabbing afin d'identifier les services derrière chaque port détecté.
