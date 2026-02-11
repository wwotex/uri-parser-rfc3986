# URI Parser (compliant with RFC3986)
A simple URI parser implemented according to the [RFC3986](https://datatracker.ietf.org/doc/html/rfc3986). I believe it fully implements the [collected ABNF](https://datatracker.ietf.org/doc/html/rfc3986#appendix-A) from Appendix A. Otherwise, it also decodes the PCT encoded strings.

### Building and running:

```bash
cd uri-parser-rfc3986
cmake -S . -B build -G Ninja

# For main app
cmake --build build --target main
./build/main "https://sample.uri%20com/this/is/the/path?x=1%21#meg%40cool"


# For automated tests
cmake --build build --target test_uri
./build/test_uri
```

### Running in a container:

```bash
# run uri parser with arbitrary input
docker compose run --rm uri_parser "https://google.com/p%40th/to/something?xxx#a%40aa"

# run automated tests for uri parser
docker compose run --rm tests_uri_parser
```
