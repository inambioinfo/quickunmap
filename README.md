# quickunmap

This tool returns unmapped reads in a sorted bam file quickly. 
It locates a later chromosome to speed up the search of unmapped reads in a sorted bam file.
It works for species with a known "late" chromosome. For human and mouse, the default is "chrM", but
can be others depending on the sequencing library.

This tool assumes the unmapped reads do not contain chromosome name and appended to the end of the bam. Otherwise, user may want to specify the chromosome name. In this case, it is quick to retrieve unmapped anyway.

## Install

```
$ wget https://github.com/zwdzwd/quickunmap/archive/master.zip
$ make
```

## Usage
```
$ quickunmap -i in.bam
```
