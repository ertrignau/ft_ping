# ft_ping 🛰️ — ICMP network diagnostic tool

Premier projet post tronc commun de l’école 42.

ft_ping est une implémentation en C de la commande ping, permettant de tester la connectivité réseau entre une machine locale et une cible distante à l’aide du protocole ICMP (Internet Control Message Protocol).

# 🎯 Objectifs du projet

Ce projet vise à comprendre :
- le fonctionnement bas niveau du réseau
- l’utilisation des raw sockets
- la construction et l’analyse de paquets ICMP
- la gestion du temps (RTT) et des signaux

# 🌐 Qu’est-ce que ICMP ?

ICMP (Internet Control Message Protocol) est un protocole de la couche réseau utilisé pour le diagnostic et le contrôle des erreurs.
Le programme ping repose sur l’échange de messages :
- ICMP Echo Request
- ICMP Echo Reply
afin de mesurer la latence et la disponibilité d’une machine distante.

# 🛠️ Compilation & Utilisation
make
- sudo ./ft_ping [options] <destination>
- sudo ./ft_ping -v google.com

 ℹ️ Les raw sockets nécessitent les droits root.

 # ✨ Fonctionnalités

- Envoi de requêtes ICMP Echo Request
- Réception et analyse des Echo Reply
- Calcul du RTT (Round Trip Time)
- Gestion des pertes de paquets
- Affichage type ping standard (min/avg/max/mdev)
- Support des options :
  - `-v` mode verbose

# ➡️ Schema 

<img width="1593" height="2026" alt="mermaid-diagram" src="https://github.com/user-attachments/assets/5a28b95e-0b7d-49b3-ac95-baf23c7beb12" />

# 🧪 Scripts de test

Les scripts sont dans le dossier `script/` et doivent etre lances depuis la racine du projet.

```bash
./script/valgrind_parser.sh 127.0.0.1
./script/compare_ping_timestamp.sh 127.0.0.1
./script/performance.sh 127.0.0.1
```

- `valgrind_parser.sh` lance `ft_ping` sous Valgrind en ne gardant que les fuites definitives et indirectes.
- `compare_ping_timestamp.sh` compare les RTT de `ft_ping` avec `ping` systeme et echoue si l'ecart depasse 30 ms par paquet.
- `performance.sh` mesure le temps total et le temps moyen par paquet.

# 📁 Arborescence du projet
```
├── inc
│   ├── icmp.h     # Définition des structures ICMP et fonctions associées
│   ├── ping.h     # Structure principale t_ping et prototypes globaux
│   └── utils.h    # Fonctions utilitaires
├── srcs
│   ├── icmp.c     # Construction et traitement des paquets ICMP
│   ├── init.c     # Initialisation du programme et du socket
│   ├── main.c     # Point d’entrée du programme
│   ├── parsing.c  # Parsing des arguments et gestion des flags
│   ├── ping.c     # Boucle principale du ping
│   └── utils.c    # Fonctions utilitaires (temps, affichage, erreurs…)
├── Makefile
```
# 👥 Contributeurs
@ertrigna
