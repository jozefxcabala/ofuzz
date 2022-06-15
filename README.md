# ofuzz

Fuzzer s názvom OFUZZ bol vyvinutý Jozefom Čabalom v rámci bakalárskej práce s názvom "Vyhľadávanie zraniteľností pomocou fuzzing techník"

# Popis

Mutation-based | covarge-based fuzzer, ktorý dokáže testovať aplikácie bez znalosti zdrojového kódu, s využitím inštrumentácie (vďaka <a href="https://github.com/GJDuck/e9patch" target="_blank">e9patch</a>)

# Spustenie

1. ```$ git clone ...```
2. v priečinku spusti príkaz ```$ make```
3. ```$ ./app.out "dir for input samples" "target application" "param" ```

 * maximalne jeden parameter

# Príklady

Spustenie aplikácie
```shell
$ ./app.out input_file/ binaries/fuzzgoat
```

