# Questions

## What's `stdint.h`?

`stdint.h` is a header file that defines various integer types with specified sizes.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

By specifying precisely how many bits we need for each piece of data, we can
keep the file size to the minimum required.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

- `BYTE` is `uint8_t`, so 8 bits, or 1 byte
- `DWORD` is `unint32_t`, so 32 bits, or 4 bytes
- `LONG` is `int32_t`, so 32 bits, or 4 bytes
- `WORD` is `uint16_t`, so 16 bits, or 2 bytes

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first two bytes will be the `bfType` member of `BITMAPFILEHEADER`, which
is a 2-byte `WORD`. According to MSDN documentation, those bytes must be
`B` and `M`.

## What's the difference between `bfSize` and `biSize`?

`bfSize` is the total size of the file, whereas `biSize` is the total size
of the actual bitmap structure.

## What does it mean if `biHeight` is negative?

If `biHeight` is negative, the bitmap is a top-down representation, where
the first row in the bitmap structure corresponds to the top row of the
image.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`

## Why might `fopen` return `NULL` in `copy.c`?

There is some problem with the filename specified. It could be that the system
does not allow filenames like the one specified (such as including newlines),
but you may also not have permissions to read or write the file in question.

## Why is the third argument to `fread` always `1` in our code?

We are reading one `struct` at a time, not one byte or some other piece of
data. We know that we want `size` to be `sizeof(BITMAPFILEHEADER)`, for
instance, and `quantity` should only be `1` because we just want the data that
should be included in that particular struct. Because we have predefined these
data structures, we can just deal with them individually.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

```c
int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
padding = (4 - (3 * sizeof(RGBTRIPLE)) % 4) % 4;
padding = (4 - (3 * 3) % 4) % 4;
padding = (4 - 9 % 4) % 4;
padding = (4 - 1) % 4;
padding = 3 % 4;
padding = 3;
```

## What does `fseek` do?

`fseek()` moves the file pointer ahead by `offset` bytes. By using `SEEK_CUR`
as the `whence` argument, we are telling `fseek()` to move `offset` bytes from
the current file position indicator.

## What is `SEEK_CUR`?

`SEEK_CUR` is the current position in the file.
