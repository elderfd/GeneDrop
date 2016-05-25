# GeneDrop
## What is this?
GeneDrop is a tool for simulating the breeding of diploid organisms with known linkage maps.

## How do I use it?
You can either build from source or use one of the prebuilt binaries.

### Input files
GeneDrop takes three input files, each of which is formatted as CSV. Each file should have a header, with columns described as follows.

#### Loci files
This describes the loci to be tracked in the simulations.

Column | Meaning
--- | ---
Locus ID | An identifier for a given locus
Distance (cM) | The genetic distance in centimorgans from one end of the chromosome
Chromosome | Which chromosome the locus sits on

An example loci file might look like,

Locus ID | Distance (cM) | Chromosome
--- | --- | ---
L1 | 0 | A
L2 | 0.3 | A

The above examples describes two loci which both sit on chromosome A. L1 sits on the end of the chromosome and L2 sits 0.3 centimorgans from L1.

#### Founder files
This describes the genotypes of the founding individuals in the pedigree.

Column | Meaning
--- | ---
Founder | The name of the founding individual, can take an arbitrary value.
... | Columns named after the loci described in the loci file. These give the genotype of the founding individual.

Arbitrary allele names can be used in the genotype columns, and slashes are used to indicate heterozygosity. An example founder file might look like,

Founder | L1 | L2
--- | --- | ---
A | 0 | 1
B | 1/0 | 0

The above examples describes founders A and B in terms of their genotype at loci called L1 and L2. Founder A carries the 0 allele at L1 and the 1 allele at L2. Founder B is heterozygous at the L1 locus and carries the 0 allele at L2.

#### Pedigree files
This describes the pedigree to be simulated. Each row describes an individual in the pedigree.

Column | Meaning
--- | ---
Generation | The name of the generation the individual belongs to, can take an arbitrary value.
ID | A specifier for the individual. Arbitrary but must be unique within a generation.
P1 | Specifier for first parent of this individual.
P2 | Specifier for second parent of this individual.

If IDs are unique they can be used as a specifier. If IDs are not unique, then one can use the format `<generation name>::<ID>`. One can also refer to founder individuals using the IDs specified in the founders file. An example pedigree file might look like,

Generation | ID | P1 | P2
--- | --- | --- | ---
F1 | C | A | B
F1 | D | A | B
F2 | E | C | D

The above example describes a F2 cross where  A and B are founder individals described in the founders file.

### Building from source
The project is set up to use cmake to allow cross-platform compilation. First ensure you have [cmake](https://cmake.org/), [Boost](http://www.boost.org/) and your C++ compiler of choice installed on your system.

1. If you plan to build the Qt front-end then change the `CMAKE_PREFIX_PATH` variable in the top-level CMakeLists.txt to point to your install directory for Qt.
2. Run `cmake build` to generate a build folder with files appropriate for your system.
3. Compile from the files generated in the build folder.

# Project structure
Project Name | Function
------------ | --------------
GeneDropBackEnd | Does all simulations, compiles to static library
GeneDropCommandLineFrontEnd | Command line interface for using the back end
GeneDropTests | A test suite ensuring the program works as intended (uses [Catch ](https://github.com/philsquared/Catch))
GeneDropGraphicalFrontEnd | Graphical interface for using the back end (uses [Qt ](https://www.qt.io/))
