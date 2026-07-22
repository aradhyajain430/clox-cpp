mkdir -p t
printf '(-1 + 2) * 3 - -4'      > t/ok.lox        # the known-good case
printf ''                        > t/empty.lox     # zero-length source
printf '   \n\t  '               > t/blank.lox     # whitespace only, EOF in skipWhitespace
printf '"abc'                    > t/unterm.lox    # stringToken walking off the end
printf '@'                       > t/stray.lox     # errorToken path
printf '(((('                    > t/parens.lox    # nested prefix rules, no closers
printf '1'                       > t/lastdigit.lox # numberToken peek at exactly size()
printf '1.'                      > t/dot.lox       # peekNext at the boundary
printf 'print'                   > t/kw.lox        # checkKeyword substr at end of buffer
printf '// comment no newline'   > t/comment.lox   # skipWhitespace comment loop hits EOF