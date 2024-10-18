# CFG-Parser

CFG-Parser is a C programme to parse strings based on a Context-Free Grammar. The programme outputs the acceptability of the input string by the CFG. It works using top-down parsing and uses DFS to explore all possible derivations from staring non-terminal

## Building

As of now the source code is not dependent on any POSIX library or headers. 

### For Windows like OS
```bash
gcc src\parser.c -o parser
```

### For Unix like OS
```bash
gcc src/parser.c -o parser
```

## Usage

Run the executable after building

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.


## Issues

A very limited number of CFGs have been tested till now. Hence, the programme is highly susceptible to crashing or reporting incorred result.

## Limitations

Terminals should be any lowercase character except 'e' as it is reserved for empty strings. Non-terminals should be any uppercase character.

## TODO

- [ ] Add threading functionality to speedup the programme
- [ ] Make source code more readable
- [ ] Add feature to print the derivation
- [ ] Add feature to accept a string as terminal or non-terminal