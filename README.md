# PET-cat_c
The repository is PET-project, that implements standard "cat" function in C language

## Start the program
After cloning this repository, use "make" command to create executive file.
```
make
```
If you want to rebuild executive file, you can use command "make rebuild"
```
make rebuild
```
Exevutive file will appear at the "src" directory

```
Usage:

s21_cat [-benst] [file ...] 

  -b, --number-nonblank    number nonempty output lines, overrides -n \
  -e,                      show nonprinting and add $ at end of each line, equivalent to -vE\
  -E, --show-ends          display $ at end of each line\
  -n, --number             number all output lines\
  -s, --squeeze-blank      suppress repeated empty output lines\
  -t                       equivalent to -vT\
  -T, --show-tabs          display TAB characters as ^I\
  -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB
```
