# Things to rebuild this library

Must be compile-time, so the library will be:

- header only part to produce binaries
- .so file that reads and loads binaries
- the header-only may embed the binary data inside a c source
- will be sequential
- will support subcommand contexts (composition)
- rewrite the god damn readme
- will have a way to deal with anonymous arguments

I might delete this project if I don't consider it useful enough


## Improvements that may happen

- hash table
- trie structure
- memory configuration
