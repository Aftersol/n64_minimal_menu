# N64 QOI Viewer
![Pixel art style title screen for a video game or software project featuring the Libdragon logo—a red, stylized dragon curved around the text. Below the logo is a menu with three options: Start Game, Options, and a humorous third line that reads Exit (jk, u can't exit lol). The background is a mosaic of geometric, stained-glass shapes in warm earthy tones fading from olive green at the top to vibrant orange at the bottom.](image.png)

An N64 program that showcases an example of a main menu.

## How to Build N64 Minimal Menu Example
This tutorial assumes you have your N64 Toolchain set up including GCC for MIPS.
Make sure you are on the preview branch of libdragon.

Clone this repository with `--recurse-submodules` or if you haven't run:

```bash

git submodule update --init
```
---
Initialize libdragon:
```bash
libdragon init
```
Then run make to build this project:

```bash
libdragon make
```

---

## Licenses

Everything in the src folder is licensed under MIT License. See [LICENSE page](https://github.com/Aftersol/n64_qoi_dec/blob/main/LICENSE) for more info.

In the assets folder the following assets are in the public domain:
- [miafan2010 - You Would Be Here](https://modarchive.org/index.php?request=view_by_moduleid&query=172936)
- [madameberry - background.png](https://opengameart.org/content/public-domain-backgrounds)
