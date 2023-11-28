#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char const * argv[])
{
    if(argc < 2){
        cout << "Insert Error : a.txt [ Bitmap Image File ]" << endl;

        exit(-1);
    }

    // const char filename[20] = argv[1];
    // char pathname[20] = "./";
    // strcat(pathname, argv[1]);

    cout << argv[1] << endl;

    int fd = open(argv[1], O_RDONLY);

    if(fd < 0){
        cout << "Image open error!" << endl;

        exit(-2);
    }

    //WINDOW * window;
    //window = newwin(16, 32, 0, 0);

    // int image_width = 0;
    // int image_height = 0;
    // int * image_width_ = &image_width;
    // int * image_height_ = &image_height;

    int image_width = 0;
    int image_height = 0;

    lseek(fd, 18, SEEK_SET);        // Image 의 가로, 세로 픽셀 수는 처음에서 18 번째 Byte 에서부터 있다.
    read(fd, &image_width, sizeof(int));    // Bit 단위로 다루어야 하지만, 파일 입출력으로는
    read(fd, &image_height, sizeof(int));   // Byte 단위로밖에 다룰 수 없다.

    bool image[image_width][image_height];
    
    lseek(fd, 0, SEEK_END);        // 이미지 파일의 마지막으로 이동.

    unsigned char temp = 0;

    for(int i = 0; i < image_height; i++){  // 단색 비트맵 파일에는 원본 이미지를 아래로 한 번 뒤집어진 상태로 저장됨을 고려.
        lseek(fd, -(image_width / 8), SEEK_CUR);    // 읽으려는 이미지의 row 의 첫 번째 픽셀로 이동.

        for(int j = 0; j < image_width; j = j + 8){ // 읽으면 해당 row 의 마지막 번째 픽셀로 이동.
            read(fd, &temp, sizeof(unsigned char)); 
            
            int offset = 0;
            unsigned char temp_and = 128;

            while(offset < 8){
                unsigned char comp = temp & temp_and;

                if(comp == 0)
                    image[i][j + offset] = false;
                else
                    image[i][j + offset] = true;

                offset++;
                temp_and /= 2;
            }
        }

        lseek(fd, -(image_width / 8), SEEK_CUR);    // 읽었던 이미지의 첫 번째 픽셀로 이동.
    }

    close(fd);
    
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

    window = newwin(image_height / 4, image_width / 2, 0, 0);

    wbkgd(window, COLOR_PAIR(1));

    /*
        --- Braille Unicode ---

        Start from '/u2800'.
            ( 1   )  ( 8   )
            ( 2   )  ( 16  )
            ( 4   )  ( 32  )
            ( 64  )  ( 128 )

        Need to think about 2 * 4 matrix and adding the numbers that matches.
    */

    wprintw(window, "Hello!");

    wrefresh(window);

    getch();

    endwin();

    // for(int i = 0; i < image_height; i++){
    //     for(int j = 0; j < image_width; j++){
    //         if(image[i][j] == true)
    //             cout << " " << " ";
    //         else
    //             cout << "*" << " ";
    //     }

    //     cout << endl;
    // }

    return 0;
}