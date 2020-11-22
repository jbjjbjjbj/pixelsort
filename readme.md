# Pixelsort

Small pixelsorter written for fun.

Tried to make it fast enough to be used with i3lock.

## Depencies

- FreeImage for leading and saving images

## Usage

To use defaults run:

```
./pixelsort input_image.jpg
```

All the FreeImage supported image formats can be used.

## Features

Here i the small list of features.

- The sorting area is determined from image brightness.

  The limits can be set with the `-u` and `-l`.
- Both horizontal and vertical sorting is supported.

  This is set with the `-d` flag, with `h` for horizontal and `v` for vertical.
  Multiple values is valid to, for example `hv` for first horizontal and then vertical.
