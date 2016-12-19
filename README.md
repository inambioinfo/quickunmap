# quickunmap

This tool returns unmapped reads in a sorted bam file quickly. 
It locates a later chromosome to speed up the search of unmapped reads in a sorted bam file.
It works for species with a known "late" chromosome. For human and mouse, the default is "chrM", but
can be others depending on the sequencing library.

This tool assumes the unmapped reads do not contain chromosome name and appended to the end of the bam. Otherwise, user may want to specify the chromosome name. In this case, it is quick to retrieve unmapped anyway.

## Install

```
$ wget [lastest release]
$ make
```

Note: install from git clone should also clone the samtools submodule.
```
$ git clone --recursive git://github.com/zwdzwd/quickunmap.git
```
or
```
$ git clone git://github.com/zwdzwd/quickunmap.git
$ cd quickunmap
$ git submodule update --init --recursive
```

## Usage
```
$ quickunmap -i in.bam
```
