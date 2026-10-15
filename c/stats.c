/*
 * stats.c - Mean, median, mode (plus extras) on a list of integers.
 * Paradigm: PROCEDURAL. State lives in a plain array; functions operate on it
 * step by step. Memory is managed manually with malloc/realloc/free.
 *
 * Build: gcc -Wall -Wextra -o stats stats.c -lm
 * Usage: ./stats 4 8 15 16 23 42      (or run with no args for a sample list)
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Sort in place with insertion sort (implemented by hand, per assignment). */
static void sort_array(int *a, size_t n) {
    for (size_t i = 1; i < n; i++) {
        int key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = key;
    }
}

static double mean(const int *a, size_t n) {
    long long sum = 0;
    for (size_t i = 0; i < n; i++) sum += a[i];
    return (double)sum / (double)n;
}

/* Median of a SORTED array. */
static double median(const int *sorted, size_t n) {
    if (n % 2 == 1) return sorted[n / 2];
    return ((double)sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
}

/*
 * Mode(s) of a SORTED array. Equal values are adjacent, so one pass counting
 * run lengths is enough. Returns a malloc'd array of modes (caller frees);
 * *n_modes receives its length and *freq the shared frequency.
 */
static int *modes(const int *sorted, size_t n, size_t *n_modes, size_t *freq) {
    int *result = malloc(n * sizeof *result);   /* worst case: every value unique */
    if (!result) return NULL;
    size_t best = 0, count = 0;
    *n_modes = 0;
    for (size_t i = 0; i < n; i++) {
        count = (i > 0 && sorted[i] == sorted[i - 1]) ? count + 1 : 1;
        /* Evaluate the run when it ends (end of array or value changes). */
        if (i == n - 1 || sorted[i + 1] != sorted[i]) {
            if (count > best) { best = count; *n_modes = 0; }
            if (count == best) result[(*n_modes)++] = sorted[i];
        }
    }
    *freq = best;
    return result;
}

static double std_dev(const int *a, size_t n, double mu) {
    double acc = 0.0;
    for (size_t i = 0; i < n; i++) acc += (a[i] - mu) * (a[i] - mu);
    return sqrt(acc / (double)n);               /* population std. deviation */
}

static void print_array(const char *label, const int *a, size_t n) {
    printf("%s[", label);
    for (size_t i = 0; i < n; i++) printf("%s%d", i ? ", " : "", a[i]);
    printf("]\n");
}

int main(int argc, char **argv) {
    size_t n = 0, cap = 8;
    int *data = malloc(cap * sizeof *data);
    if (!data) { fputs("out of memory\n", stderr); return 1; }

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            char *end;
            long v = strtol(argv[i], &end, 10);
            if (*end != '\0' || end == argv[i]) {
                fprintf(stderr, "Invalid integer: '%s'\n", argv[i]);
                free(data);
                return 1;
            }
            if (n == cap) {                      /* grow the array as needed */
                cap *= 2;
                int *tmp = realloc(data, cap * sizeof *data);
                if (!tmp) { free(data); fputs("out of memory\n", stderr); return 1; }
                data = tmp;
            }
            data[n++] = (int)v;
        }
    } else {
        static const int sample[] = {4, 8, 15, 16, 23, 42, 8, 15, 8, 23, 15};
        n = sizeof sample / sizeof sample[0];
        if (n > cap) {
            int *tmp = realloc(data, n * sizeof *data);
            if (!tmp) { free(data); fputs("out of memory\n", stderr); return 1; }
            data = tmp;
        }
        for (size_t i = 0; i < n; i++) data[i] = sample[i];
        puts("(no arguments given; using sample data)");
    }

    print_array("Input:    ", data, n);
    sort_array(data, n);
    print_array("Sorted:   ", data, n);

    double mu = mean(data, n);
    size_t n_modes, freq;
    int *m = modes(data, n, &n_modes, &freq);
    if (!m) { free(data); fputs("out of memory\n", stderr); return 1; }

    printf("Count:    %zu\n", n);
    printf("Mean:     %.2f\n", mu);
    printf("Median:   %.2f\n", median(data, n));
    if (freq == 1) {
        puts("Mode:     none (all values occur once)");
    } else {
        print_array("Mode:     ", m, n_modes);
        printf("          (each appears %zu times)\n", freq);
    }
    printf("Min/Max:  %d / %d\n", data[0], data[n - 1]);
    printf("Std dev:  %.2f\n", std_dev(data, n, mu));

    free(m);
    free(data);
    return 0;
}
