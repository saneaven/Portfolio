# Computer Science

Coursework and self-contained projects focused on the fundamentals: algorithm design, relational databases, and image processing. The common thread across this category is implementing things from scratch: writing the core algorithms by hand, reasoning about correctness and complexity, and measuring real performance rather than reaching for a library. All of the work here is solo.

## Projects

| Project | Description | Key stack |
| --- | --- | --- |
| [Algorithms](Algorithm/README.md) | A collection of classic algorithm implementations, each built around a different design technique (brute force, divide and conquer, dynamic programming, greedy/best-first search, and branch-and-bound) and graded against reference outputs and timing targets. | C++ (C++11), GNU Make, Valgrind, LaTeX (`pgfplots`) |
| [Database (MySQL)](Database-MySQL/README.md) | Relational-database coursework progressing from comprehensive SQL on a rare-books schema, through Instagram-style ER design, to a Java/JDBC servlet app running social-graph analytics queries against MySQL. | MySQL, Java, JDBC (Connector/J), Java Servlets, drawio |
| [Image Operations](Image%20Operations/README.md) | A from-scratch image-processing toolkit with a Tkinter GUI and a command line, featuring hand-written point operations, spatial convolution, histogram equalization, and a three-way 2D Fourier transform suite (direct DFT, separable DFT, and FFT). | Python 3, Tkinter/ttk, NumPy, Pillow, Matplotlib, OpenCV (reference) |

## Common themes

- **From-scratch implementations.** Core algorithms (convex hull, Dijkstra variants, Kadane's, the DFT/FFT, point and convolution operations) are written by hand; libraries appear only as references for validation.
- **Correctness and performance.** The work emphasizes asymptotic complexity and measured runtime, from timing targets and Valgrind checks in the algorithm assignments to a dedicated transform-timing harness in Image Operations.
- **Data modeling and queries.** The database work layers schema design and referential integrity under SQL that ranges from roll-up reporting to multi-way self-joins for social-graph questions.
