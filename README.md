The following readme-file is for the group Teemu_Miska_Eero_ja_Jaakko submission of the "weather power app".

General idea of the application is to monitor electricity consumption and compare it to the given weather forecast. This is done by fetching power and weather data from an API, presenting it to the user in an interface. User can choose to compare different data from multiple to a specific date – this is completely up-to the end-user. 

We used Qt Creator version 5.15.0 on Windows (NOT THE REMOTE DESKTOP VERSION). Our compiler was MinGQ 8.1.0. and we used standard C++ and Qt libraries and modules (Qt Chart, Qt Core, Qt Widgets, Qt Network). No need to download any other libraries at this point.
User needs to have the openSSL toolkit installed in your QT in order to see the graphs. (see here: https://moodle.tuni.fi/mod/forum/discuss.php?d=282044)

For compiling the program TA has to compile the *.pro-file and run the main.cpp file (This executes the program). If TA only wants to fetch the data TA has to run the logic.cpp file (This fetches the data and draws graphs in the final application). Using the application itself is very straight forward.

Documentation and instructions of the application are located in weather_power_app folder.
