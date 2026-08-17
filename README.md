# Canonical Labelling and Graph Isomorphism: A C++ Reimplementation of Nauty

A C++ reimplementation of the Nauty algorithm (canonical labelling and automorphism group computation), developed as part of a master's thesis on graph isomorphism and canonical labelling.

## Requirements

- A C++ compiler supporting C++17 (e.g. `g++`)
- GNU Make

## Compilation

From the `code` directory, simply run:

```bash
make
```

This builds the executable at `bin/main`.

## Usage

```bash
./bin/main <mode> [arguments...]
```

Five modes are available: `iso`, `canonical`, `all`, `aut` and `file`.

### `iso` — Isomorphism test

```bash
./bin/main iso <graph6_1> <graph6_2> [invariant]
```

Computes the canonical form of both graphs (given in graph6 format) and prints `Isomorphes` or `Non isomorphes` depending on whether the canonical forms match.

### `canonical` — Canonical form

```bash
./bin/main canonical <graph6> [invariant]
```

Computes and prints (in graph6 format) the canonical form of the given graph.

### `all` — Canonical form + all permutations 

```bash
./bin/main all <graph6> [invariant]
```

Same as `canonical`, but additionally takes every permutation found for the original graph, applies each one to it, and reruns canonical labelling on the resulting graph. To verify that the same canonical form is obtained every time.

### `aut` — Automorphism group

```bash
./bin/main aut <graph6> [invariant]
```

Prints the canonical form, the generators of the automorphism group, then explicitly enumerates every permutation in the group.

### `file` — Batch processing

```bash
./bin/main file <input_file> <output_file> [invariant]
```

Reads a file containing one graph6 string per line, computes the canonical form of each graph, and writes the results (one canonical form per line) to the output file.

## Available invariants

The optional `invariant` argument, common to every mode, selects the vertex invariant used for partition refinement:

| Value      | Function                          |
|------------|-------------------------------------|
| `triangle` | `InvariantTriangleByCell` (default) |
| `paths`    | `InvariantTwoPathsByCell`           |
| `triple`   | `InvariantCellTriplesByCell`        |

If the argument is omitted or not recognized, `triangle` is used by default.

## Examples

```bash
# Test isomorphism between two graphs using the "paths" invariant
./bin/main iso G1 G2 paths

# Compute the canonical form of a graph using the default invariant (triangle)
./bin/main canonical G1

# Batch-process a file of graphs using the "triple" invariant
./bin/main file graphs_in.g6 canonical_out.g6 triple
```

(replace `G1`, `G2` with your own graph6 strings)
