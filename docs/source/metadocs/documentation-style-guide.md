# Documentation Style Guide
## Overview
- This document contains rules that are to be followed when creating or updating this documentation repository, as well as some related tips and syntax.

## File Locations
- All documentation files (generally Markdown) must be located within `/docs/source/` or a subfolder.
	- Most documentation category folders are located exactly at this level of the file structure.
- Static assets (essentially anything that is not a Markdown file) shall be placed in `/docs/source/_static`.
	- All image files shall be placed in `/docs/source/_static/images` or subfolders.
	- All other asset files (ex. PDF) shall be placed in `/docs/source/_static/files` for now.
- All static assets shall be placed in a subfolder reflecting their reference location in the main file tree.
	- For example, if the file `foo.png` is embedded within `/docs/source/metadocs/bar.md`, then `foo.png` shall be placed in `/docs/source/_static/images/metadocs`.
	- If there are many related images only used in a single note within that category, it is strongly preferred to create a folder sharing the title of the note to place those images in.
		- In the same example, `foo1.png` ... `foo20.png` embedded within `bar.md` would be placed in `/docs/source/_static/images/metadocs/bar`.

## Naming Conventions
- Do not use spaces in any folder or file name.
- Do not use capital lettering for any folder or file name without explicit approval from the project manager.
	- The reason for this is that links in the final product (on ReadTheDocs.io) are case-sensitive. Maintaining a consistent format helps reduce errors.
- Documentation category folder names (i.e., folders under `/docs/source/`) shall use lower kebab case (ex. `data-aq`).
- File names for Markdown files and assets (i.e., images) shall use lower kebab case (ex. `documentation-style-guide.md`).

## Writing Conventions
- Each Markdown file shall have only one H1 tag (`#`).
- Do not add spaces between consecutive headers, or between headers and their following text.
- Organize Markdown files intelligently.
	- For example, if you write a brief overview of what the file is at the top, create an H2 (`##`) titled `Overview`. Then put the rest of the content under other headers, such as `Instructions` and `Examples`.
- Text should generally be bulleted and short. 
	- Do not write long-form prose. Break it up into manageable chunks with bulleted or numbered lists.
- Any reference to a version number such as "V3" shall be formatted with an uppercase `V`.
	- For instance, `Quarterback V3` is formatted as such, not as `Quarterback v3`.
- As cursed as it is, "quarterback" is one word, and "running back" is two words. Use this and be consistent.

### Comments
- Line comments may be added using `%`. These will appear in the source Markdown file, but will not appear in the automatically generated HTML (and by extension the webpage).

## Linking and Embedding
- In order to link from one Markdown file to another, use the format below. 
	- For instance, linking to the [Sphinx](./sphinx) page in this directory would take the form `[Sphinx](./sphinx)`, where the `.` denotes the current directory.
	- However, linking to a file in another directory, such as [Bot Types](../high-level/bot-types.md), would look like this: `[Bot Types](../high-level/bot-types)`, where `..` denotes the parent of the current directory.
```
[Link Display Name](relative path with space escapes)
```
- In order to link to a heading in the same file, use the format below.
	- Linking to the heading in another file essentially follows the same rules as above. 
	- ReadTheDocs provides heading links for each note with forced lowercase and spaces formatted as dashes. For instance, linking to [this header](#linking-and-embedding) would look like this: `[this header](#linking-and-embedding)`
```
[Link Display Name](#lower-kebab-case-heading-name)
```
- To embed an image, prefix a link to the image with an exclamation point (`!`).
	- Some other URLs or assets can also be embedded this way, if Sphinx/ReadTheDocs supports them.
	- For example, including `/docs/source/_static/images/klondike-facepalm.png` from *this file* would look like this: `![Image Alt Text](../_static/images/klondike-facepalm.png)`
	- To change the size or alignment of an image, use `attrs_inline` syntax.
		- For example, to center align an image and set the width to 100 pixels: `![Image Alt Text](../_static/images/klondike-facepalm.png){w=100px align=center}`
		- See also: https://myst-parser.readthedocs.io/en/latest/syntax/images_and_figures.html
- Although it may be convenient to edit the documentation repository in Obsidian, regular Obsidian double-bracket links do **not** work.
	- However, this type of HTML *file* link does work in Obsidian. 
		- Header links with spaces do not work since RTD formats spaces as dashes.
			- Since Obsidian links are case-insensitive, that part of the formatting is irrelevant.