# Homework.
Егор Шастин Олегович, gruppa b81-mm
Contact: st129457@student.spbu.ru,egor.shast0.0@gmail.com

# The Shannon Encoder and Decoder

## Overview

The Shannon encoder and decoder is a lossless data compression method. It assigns short binary codes to frequent characters and long ones to rare ones based on their probability. The encoder builds a table of codes and encodes the message. The decoder uses this table to reconstruct the source text. The method saves information and reduces the size of the data.

## Table of Contents
- [Installation and Build](#installation-and-build)
- [Tests](#tests)

## Installation and Build

### Building the Project

```sh
make
```

### Running the Programm
To start the programm, run the compiled executable:

```sh
./bin/Start # Run general programm
```

## Tests

#### Continuous Integration (CI)
This project uses GitHub Actions for automated builds and testing. All tests are run in the CI pipeline on every commit.

