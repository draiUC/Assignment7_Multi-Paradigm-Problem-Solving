"""
stats.py - Mean, median, mode (plus extras) on a list of integers.
Paradigm: OBJECT-ORIENTED. A StatisticsCalculator object owns its data
(encapsulation), validates it once, and exposes one method per statistic.

Usage: python3 stats.py 4 8 15 16 23 42    (or no args for a sample list)
"""
import math
import sys


class StatisticsCalculator:
    """Calculates descriptive statistics for a list of integers."""

    def __init__(self, data):
        if not data:
            raise ValueError("data must contain at least one integer")
        if not all(isinstance(x, int) for x in data):
            raise TypeError("all values must be integers")
        self._data = list(data)            # private copy: callers can't mutate us
        self._sorted = sorted(self._data)  # cached; used by several methods

    # ----- core statistics, one method each ------------------------------
    def mean(self):
        return sum(self._data) / len(self._data)

    def median(self):
        n = len(self._sorted)
        mid = n // 2
        if n % 2 == 1:
            return float(self._sorted[mid])
        return (self._sorted[mid - 1] + self._sorted[mid]) / 2

    def frequencies(self):
        """Dictionary mapping each value to its number of occurrences."""
        counts = {}
        for value in self._data:
            counts[value] = counts.get(value, 0) + 1
        return counts

    def mode(self):
        """Return (list_of_modes, frequency); handles ties (multimodal data)."""
        counts = self.frequencies()
        best = max(counts.values())
        return sorted(v for v, c in counts.items() if c == best), best

    # ----- extras --------------------------------------------------------
    def minimum(self):
        return self._sorted[0]

    def maximum(self):
        return self._sorted[-1]

    def std_dev(self):
        mu = self.mean()
        return math.sqrt(sum((x - mu) ** 2 for x in self._data) / len(self._data))

    # ----- presentation --------------------------------------------------
    def report(self):
        modes, freq = self.mode()
        lines = [
            f"Input:    {self._data}",
            f"Sorted:   {self._sorted}",
            f"Count:    {len(self._data)}",
            f"Mean:     {self.mean():.2f}",
            f"Median:   {self.median():.2f}",
        ]
        if freq == 1:
            lines.append("Mode:     none (all values occur once)")
        else:
            lines.append(f"Mode:     {modes}")
            lines.append(f"          (each appears {freq} times)")
        lines.append(f"Min/Max:  {self.minimum()} / {self.maximum()}")
        lines.append(f"Std dev:  {self.std_dev():.2f}")
        return "\n".join(lines)


def main(argv):
    if argv:
        try:
            data = [int(a) for a in argv]
        except ValueError as err:
            sys.exit(f"Invalid integer: {err}")
    else:
        print("(no arguments given; using sample data)")
        data = [4, 8, 15, 16, 23, 42, 8, 15, 8, 23, 15]
    print(StatisticsCalculator(data).report())


if __name__ == "__main__":
    main(sys.argv[1:])
