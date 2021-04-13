The following readme-file is for the group Teemu_Miska_Eero_ja_Jaakko submission of the "weather power app".

We used Qt Creator version 5.15.0 on Windows (NOT THE REMOTE DESKTOP VERSION). Our compiler was MinGQ 8.1.0. and we used standard C++ and Qt libraries and modules (Qt Chart, Qt Core, Qt Widgets, Qt Network). No need to download any other libraries at this point.
User needs to have the openSSL toolkit installed in your QT in order to see the graphs. (see here: https://moodle.tuni.fi/mod/forum/discuss.php?d=282044)

For compiling the program TA has to compile the *.pro-file and run the main.cpp file (This executes the program). If TA only wants to fetch the data TA has to run the logic.cpp file (This fetches the data and draws graphs in the final application).

Using the application itself is very straight forward but still very work-in-progress. NOTE: THIS IS A PROTOTYPE!

User chooses a date or multiple.
Chooses what data they want/need.
Hits execute and the data is presented for the user.
