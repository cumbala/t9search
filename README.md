# t9search
Implementation of T9 contacts search algorithm for IZP course

## Building
`gcc` and `make` are the only requirements. `Linux` is preferred, but this should also build and run on `macOS`
1) Clone this repo
2) `cd t9search`
3) `make` 

## Running
- `./t9search < telbook.txt` - will output every contact in `seznam.txt` in formatted form
- `./t9search 264 < telbook.txt` - will perform search over `seznam.txt` with the given query
  
  Output:
  ```bash
  Bohuslav Krena, 420541141233
  Bohuzel Nedambody, 420541141233

  ```

- `./t9search 111 < telbook.txt` - will not find anything

  Output:
  ```bash
  Not found

  ```  

## Testing
Tested on both `merlin` and `eva` servers. Should also be compilable on `macOS` using it's built-in `clang`. Most work done in **CLion** connected to WSL Ubuntu 20.04 
