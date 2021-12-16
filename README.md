# Command-Parser

A command parser programming using C++ that allows a user to create a variety of shapes based on a dynamically created array. The shapes can include circles, rectangles, triangles , etc.

The sstream library is used to parse through the input, allowing for the opportunity to check the user has inputted the correct syntax. Note that no memory leaks exists 

Arguments which can be used include:
- name, type, loc, size, value, angle

Users can also create a new database, move/rotate/draw/delete shapes depending on their choosing.

The potential error messages that could pop up include:

1. Invalid Command -- first word in input does not match valid command
2. Invalid argument -- argument not correct type
4. Invalid Shape Name -- reserved word entered, as identified below
5. Shape Name exists -- Shape exists in database
6. Shape name not found -- Shape name not in database
7. Invalid shape type -- Shape type not covered by program
8. invalid value -- For example an angle, only allowed from 0 to 360
9. too many arguments -- input by user has too many input words
10. too few arguments -- not enough input for the desired argument
11. shape array is full

Reserved keyword list that can only be used by arguments:
{"all", "maxShapes", "create", "move", "rotate", "draw", "delete"}

An example run of the program is found below:

> maxShapes 6

New database: max shapes is 6
> create hello_circle circle 1 2 3 3


Created hello_circle: circle 1 2 3 3
> create hello


Error: too few arguments
> create triangle


Error: invalid shape name
> create triangle hello_triangle 2 3 4 5


Error: invalid shape name
> create hello_triangle 2 3 4 5


Error: invalid argument
Error: invalid shape type
> create hello_triangle triangle 2 3 4 5


Created hello_triangle: triangle 2 3 4 5
> draw


Error: too few arguments
> draw all


Drew all shapes
hello_circle: circle 1 2 3 3
hello_triangle: triangle 2 3 4 5
> delete hello_circle


Deleted shape hello_circle
> draw all


Drew all shapes
hello_triangle: triangle 2 3 4 5
> 
