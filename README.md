# c-tools

Kleine command-line tools in C (C11), geschreven om te oefenen met bestanden, buffers, pointers en string-verwerking zonder externe libraries.

| Tool | Wat het doet |
|------|--------------|
| [`minigrep`](minigrep/minigrep.c) | Zoekt een woord in een bestand, toont regelnummers en kleurt elke match rood |
| [`hexdump`](hexdump/hexdump.c) | Toont de inhoud van elk bestand als hex + ASCII, 16 bytes per regel |
| [`csv2json`](csv2json/csv2json.c) | Zet een CSV-bestand met kopregel om naar een JSON-array van objecten |

## Bouwen en testen

```sh
make        # bouwt alle tools met -Wall -Wextra -Wpedantic
make test   # draait de smoke tests op de voorbeeldbestanden
```

Op elke push bouwt GitHub Actions alles met **gcc en clang**, met warnings als errors.

## Voorbeelden

```console
$ ./minigrep/minigrep ERROR examples/server.log
2: ERROR: Kan database niet vinden
4: ERROR: Timeout opgetreden
Total found: 2

$ ./hexdump/hexdump examples/server.log
00000000 49 4E 46 4F 3A 20 53 65 72 76 65 72 20 67 65 73  |INFO: Server ges|
...
00000060 65 6E 0A                                         |en.|

$ ./csv2json/csv2json examples/stal.csv examples/stal.json
Converted 3 rows with 4 columns
```

```json
[
  {"afdeling": "Stal 1", "temperatuur": "22.5", "vochtigheid": "65", "ventilatie": "40"},
  ...
]
```

## Bekende beperkingen

- `minigrep`: regels langer dan 1023 tekens worden in stukken gelezen.
- `csv2json`: maximaal 32 kolommen en 128 tekens per veld; velden tussen aanhalingstekens met een komma erin (`"21,8"`) worden nog niet ondersteund. Alle waarden worden als string opgeslagen.
