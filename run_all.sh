#!/bin/sh
set -e
cd "$(dirname "$0")"
echo "===== C (procedural) =====";        (cd c && gcc -Wall -Wextra -o stats stats.c -lm && ./stats "$@")
echo; echo "===== OCaml (functional) ====="; (cd ocaml && ocamlopt stats.ml -o stats && ./stats "$@")
echo; echo "===== Python (OOP) =====";      python3 python/stats.py "$@"
