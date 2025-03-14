# SYKOM Projekt – SoC z peryferiami emulowanymi przez QEMU

Repozytorium zawiera projekt realizowany w ramach przedmiotu SYKOM. Celem było stworzenie systemu na chipie (SoC) z peryferiami emulowanymi przez program QEMU oraz testowanie działania systemu przy użyciu dystrybucji Linux i odpowiednich sterowników.

## Opis projektu

Projekt obejmuje:
- **Moduł Verilog (`gpioemu.v`)** – Implementację automatu stanowego, który oblicza N-tą liczbę pierwszą.  
- **Moduł jądra systemu Linux** – Sterownik obsługujący komunikację z peryferiami poprzez interfejs `/proc`.  
- **Aplikację testującą w języku C** – Program weryfikujący poprawność działania modułów systemowych.
---

Więcej szczegółowych informacji, w tym opisy implementacji, wyniki testów oraz wnioski, znajdują się w sprawozdaniu dołączonym do projektu.
