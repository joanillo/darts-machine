# darts-machine
Darts machine for automatic scoring

The idea is to build a cabinet with a dartboard (steel darts) and a scoreboard (15'' screen) integrated. Powered by a Raspberry Pi 3, 2 webcams and OpenCV library. The difficult part is a reliable detection of the darts.
<ul>
<li>wiki (Catalan): http://wiki.joanillo.org/index.php/Recreativa_dards._Discussi%C3%B3._Soluci%C3%B3_OpenCV_o_soluci%C3%B3_material_resistiu</li>
  <li>blog (Catalan & Spanish): http://www.joanillo.org/?cat=45&lang=es</li>
  <li>www.retroplaneta.com</li>
</ul>
This project is under construction and discussion. It is not functional at this moment.

<strong>v0.0.8</strong>: runs under Linux and console. No OpenCV and graphics (SDL) at this moment. This version implements all the logics of the games (301, 501, Cricket, Count Up, Round the Clock, Halve It), you can choose a game, number of users, and it is possible to see the correct  scoring until the end of a game. 

<strong>v0.0.11</strong>: Same as v0.0.8. Several improvemets, specially <strong>DELETE</strong> the last dart scoring. This is necessary because I'm already working on detecting darts with OpenCV, and I see that is very difficult to have 100% of accuracy in darts detection.

<strong>v0.0.12</strong>: This is the last version before merging with the subsytem of detection darts. In this version we fixed several bugs and added several improvements. When we delete the last score, a collection of the nearest scores are proposed. I'm sorry but at this moment variable names, user interface (console) and comments are in Catalan,

<img src="http://www.joanillo.org/wp-content/uploads/2018/02/darts-machine-00121.png" width="200" />
