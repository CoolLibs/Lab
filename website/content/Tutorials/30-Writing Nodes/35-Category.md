
A category is defined by simply creating a folder with the desired name in the "Nodes" folder. All nodes belonging to that category will have to be put in that folder.
The number at the beginning of the folder name won't be displayed but is used to order the categories.

## Consistency between nodes of the same category

:::info
Since nodes of the same category can be swapped with each other, it is important to make sure they will be compatible. This is achieved by using the exact same parameter names when they share common parameters. For example both "Square Grid" and "Hexagonal Grid" have a "Grid Size" parameter. By giving it the exact same name in both nodes we make sure that its value / links will be preserved when switching between a square and hexagonal grid in the dropdown.
:::

## Category config

By right-clicking on a category in the nodes menu you can configure its kind, which will determine its color, and its number of main input pins, which will determine the look of the first N pins. It is very important to set this up when you create a new category.

![](./img/category-config.png)