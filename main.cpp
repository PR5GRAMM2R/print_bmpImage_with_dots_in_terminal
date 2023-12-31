#include <ncursesw/curses.h>        // "g++ main.cpp screen.cpp original.cpp braille.cpp -o main.out -lncursesw" 로 터미널 컴파일 필수.
// #include <ncurses.h>             // 2020203011 배주환
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <locale>
#include <codecvt>
#include "braille.hpp"
#include "original.hpp"
#include "screen.hpp"

using namespace std;

int main(int argc, char const * argv[])
{
    std::locale::global(std::locale(""));   // Set program's locale to default for using Unicode.
    std::wcout.imbue(std::locale());

    if(argc < 2){
        std::cout << "Insert Error : a.txt [ Bitmap Image File ]" << endl;

        exit(-1);
    }

    std::cout << argv[1] << endl;
    
    Original ori;

    ori.loadImage(argv[1]);

    Screen mainScreen(256, 128);

    mainScreen.updateScreen(0, 0, ori.getImage());

    WINDOW * window;

    initscr();

    if(has_colors == FALSE){
        puts("Terminal does not support colors!");
        endwin();
        return 1;
    }
    else{
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
    }

    refresh();

    window = newwin(mainScreen.getHeight() / 4, mainScreen.getWidth() / 2, 0, 0);

    wbkgd(window, COLOR_PAIR(1));

    for(int i = 0; i < mainScreen.getHeight(); i++){
        for(int j = 0; j < mainScreen.getWidth(); j++){
            mvwaddwstr(window, i, j, &(mainScreen.getScreenBraille())[i][j]);
        }
        waddch(window, '\n');
    }
   
    wrefresh(window);

    getch();

    endwin();

    // for(int i = 0; i < screenHeight; i++){
    //     for(int j = 0; j < screenWidth; j++){
    //         if(screenImage[i][j] == true){
    //             cout << "*";
    //         }
    //         else{
    //             cout << " ";
    //         }
    //     }

    //     cout << endl;
    // }

    return 0;
}
