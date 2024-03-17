# Parsing.md : High-level documentation describing parsing protocols and error handling

Hi there :) enjoy your stay
## Parsing protocol



## Error recovery

In principle, when we are parsing a certain AST via a certain Parse* method, even when we hit an error, we don't return 
want to return a nullptr to indicate that we've hit an error. We would want to instead still create the intended AST, 
and set the error flag to be true instead.

The general error recovery mode for parsing is parsing until we hit the
SemiColon token, then we consume that token and start a new.