# Parsing.md : High-level documentation describing parsing protocols and error handling

Hi there :) enjoy your stay
## Parsing protocol



## Error recovery

In principle, we say that if a Parse*() result is nullptr, then we haven't
committed to calling that Parse*() function yet. 

If a Parse*() result is non-null, then it can either parse successfully, or failed since its
commitment to parsing via that Parse*(). The error message is sent directly to a vector of error messages in parser.

If a Parse*() commits and at a certain time cannot match against a TokenType,
it will suppose that it has matched successfully against that token and report an error.

There are two kind of Parse* that can be recoverable:
- Statement parsing.
- Declaration parsing.