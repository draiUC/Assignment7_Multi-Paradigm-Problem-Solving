# Assignment 7: Multi-Paradigm Problem Solving

Mean, median, and mode (plus count, min/max, std. deviation) of a list of integers.

| Language | Paradigm          | File             |
|----------|-------------------|------------------|
| C        | Procedural        | `c/stats.c`      |
| OCaml    | Functional        | `ocaml/stats.ml` |
| Python   | Object-oriented   | `python/stats.py`|

## Build and run
Each program takes integers as arguments; with no arguments it uses a built-in sample list.

    cd c      && gcc -Wall -Wextra -o stats stats.c -lm && ./stats 3 1 2 2 3 10
    cd ocaml  && ocamlopt stats.ml -o stats             && ./stats 3 1 2 2 3 10
    cd python && python3 stats.py 3 1 2 2 3 10

Or run all three: `./run_all.sh 3 1 2 2 3 10`

## Behavior
- Multimodal data: every tied value is reported (e.g. `[2, 3]`).
- If all values are unique, the mode is reported as "none".
- Even-length lists: the median is the average of the two middle values.
- Invalid input prints an error and exits with status 1.
