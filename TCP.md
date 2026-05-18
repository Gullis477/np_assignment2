# TCP server

När en klient har kopplat upp sig har den 5 sekunder på sig, sen skickar servern ett "ERROR TO\n" till klienten. Socketen ska stängas och childprocessen ska termineras.

servern ska bara acceptera ett argument <string>:Port
antingen IPv4 eller DNS namn. DNS kan leda till både till IPv4 eller IPv6.

## getaddrinfo

Returnerar en int som berättar om det lyckades eller inte, 0 för framgång och allt annat är misslyckande.

### struct addrinfo

är en struct som används för att ge ledtrådar (input) till getaddrinfo och för att ta emot de resulterande nätverksadresserna.
För

```c
struct addrinfo hints;
struct addrinfo *result, *rp;
hints.ai_socktype = SOCK_STREAM // Detta betyder att det är en TCP server
hints.ai_family = AF_UNSPEC // Både IPv4 och IPv6
hints.ai_flags = AI_PASSIVE;     // Redo för bind()
```

result pekar på starten av listan av addrinfo som sparas för att kunna städa minnet i slutet medan rp (result pointer) pekar på den nuvaranda addressen för när man loopar igenom att testa för ipv4 och ipv6.

## setsockopt

Set Socket Options
används för att ändra inställningar och beteende för en specifik nätverkssocket.

### SO_REUSEADDR

gör att man kan återanvända adressen lättare under utveckling.

### SOL_SOCKET

Vilken "nivå" i nätverksstacken vi vill ändra på. SOL_SOCKET betyder att vi ändrar allmänna inställningar för själva socket-gränssnittet (oberoende av om det är IPv4, IPv6, TCP eller UDP).
