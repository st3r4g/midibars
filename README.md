# midibars
Play selected bars from a midi file. Useful for a quick audio feedback when
editing lilypond scores.

## build

```
git submodule init
git submodule update
make
```

## usage

Invoke the program from your text editor. Vim example:
```
command! -nargs=* Play !/path/to/midibars <args> %:h.midi
```
