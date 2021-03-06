#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <unistd.h>
#include <vector>

#define SLEEP(milliseconds) usleep( (unsigned long) (milliseconds * 1000.0) )
#define GRASS     ' '
#define EMPTY     'n'
#define WATER     'x'
#define MOUNTAIN  'I'
#define PLAYER    'P'

#define GRASS_PAIR     1
#define EMPTY_PAIR     2
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4
using namespace std;
//setup, input, draw and logic
bool gameOver;
char movement;
bool checkn = false;
bool bulletBoolean = false;
char kills = '0' ;
char golds ='0';

class MapCharacter {
public:
    MapCharacter(char id, int h, int w);

    bool isWall; // x and n
    bool isGun;// bassically O
    bool isBullet;// bassically o
    bool isenemy;// basically P
    bool isPlayer;
    bool untouchable;
    bool isgold;
    char identity;// contains whatever the character is
    int height, width;

    void printChae() {
        cout << identity;
    }
};

class Items {
public:
    Items(char it, int x, int y) {
        character = it;
        xCordinate = xxx = x;
        yCordinate = yyy = y;
        direction = false;
    }

    char character;
    int xCordinate;
    int yCordinate;
    int xxx, yyy;
    bool direction;
};

MapCharacter::MapCharacter(char id, int h, int w) {
    {
        identity = id;
        width = w;
        height = h;
        isWall = false;
        isGun = false;
        isBullet = false;
        isPlayer = false;
        isenemy = false;
        untouchable = false;
        isgold = false;
    }
    if (id == 'x' || id == 'n') {
        isWall = true;
    }
    if (id == '_' || id == '/' || id == 92) {
        untouchable = true;
    }
    if(id == 'P' || id== 'I'){
        isenemy =true;
    }
    if(id == '!'||id == '#'|| id== '$'||id== '%'|| id == '&'){
        isgold = true;
    }

}

//function that takes in the object MapCharacter
class EveryFunction {
public:
    EveryFunction(char **map, int height, int width) {
        h = height;
        w = width;
        playMap = new MapCharacter **[height];
        for (int i = 0; i < height; i++) {
            playMap[i] = new MapCharacter *[width];
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (map[i][j] == 'I') {
                    Ibox.push_back(new Items(map[i][j], i, j));

                }
                if (map[i][j] == 'P') {
                    Pbox.push_back(new Items(map[i][j], i, j));
                }
                if (map[i][j] == 'T') {
                    TBox.push_back(new Items(map[i][j], i, j));
                }
                if (map[i][j] == '$' || map[i][j] == '%' || map[i][j] == '#' || map[i][j] == '!' || map[i][j] == '&') {
                    Openbox.push_back(new Items(map[i][j], i, j));
                }
                if (map[i][j] == 'O') {
                    Weapon.push_back((new Items(map[i][j], i, j)));
                }
                playMap[i][j] = new MapCharacter(map[i][j], i, j);
            }
        }
        playerSetup();
    }

    void playerSetup() {
        int playerX = homiex = 3;
        int playerY = homeiy = 60;
        char player = '@';
        playMap[playerX][playerY]->identity = player;
        playMap[playerX][playerY]->height = playerX;
        playMap[playerX][playerY]->width = playerY;
        playMap[playerX][playerY]->isPlayer = true;
    }

    /*this function allows the player to move according to the user input
     * if w then up and so on
     * */

    void movementofPlayer(char move) {
        if (checkn == true) {
            int xaxis = homiex;
            int yaxis = homeiy;
            if (move == 'w') {
                //movement is upwards
                //to move upwards we will shift the player arr[--][]
                direc = 'U';
                if (playMap[--xaxis][yaxis]->isWall == true) {
                    checkn = false;
                    return;
                }
                if(playMap[xaxis][yaxis]->isenemy == true){
                    gameOver = true;
                }
                if(playMap[xaxis][yaxis]->isgold == true){
                    Openbox.pop_back();
                    golds++;
                }
                playMap[homiex][homeiy]->identity = ' ';
                int xx = --homiex;
                int yy = homeiy;
                playMap[homiex][homeiy]->identity = '@';
                playMap[homiex][homeiy]->isPlayer = true;
                checkn = false;
                return;
            }
            if (move == 's') {
                direc = 'D';
                if (playMap[++xaxis][yaxis]->isWall == true) {
                    checkn = false;
                    return;
                }
                if(playMap[xaxis][yaxis]->isenemy == true){
                    gameOver = true;
                }
                if(playMap[xaxis][yaxis]->isgold == true){
                    Openbox.pop_back();
                    golds++;
                }
                playMap[homiex][homeiy]->identity = ' ';
                int xx = ++homiex;
                int yy = homeiy;
                playMap[homiex][homeiy]->identity = '@';
                playMap[homiex][homeiy]->isPlayer = true;
                checkn = false;
                return;
            }
            if (move == 'd') {
                direc = 'R';
                if (playMap[xaxis][++yaxis]->isWall == true) {
                    checkn = false;
                    return;
                }
                if(playMap[xaxis][yaxis]->isenemy == true){
                    gameOver = true;
                }
                if(playMap[xaxis][yaxis]->isgold == true){
                    Openbox.pop_back();
                    golds++;
                }
                playMap[homiex][homeiy]->identity = ' ';
                int xx = homiex;
                int yy = ++homeiy;
                playMap[homiex][homeiy]->identity = '@';
                playMap[homiex][homeiy]->isPlayer = true;
                checkn = false;
                return;
            }
            if (move == 'a') {
                direc = 'L';
                if (playMap[xaxis][--yaxis]->isWall == true) {
                    checkn = false;
                    return;
                }
                if(playMap[xaxis][yaxis]->isenemy == true){
                    gameOver = true;
                }
                if(playMap[xaxis][yaxis]->isgold == true){
                    Openbox.pop_back();
                    golds++;
                }
                playMap[homiex][homeiy]->identity = ' ';
                int xx = homiex;
                int yy = --homeiy;
                playMap[homiex][homeiy]->identity = '@';
                playMap[homiex][homeiy]->isPlayer = true;
                checkn = false;
                return;
            }
        }
        return;
    }
    void gameO(){
        if(Openbox.size()==0){
            gameOver = true;
        }
    }
    void bullet() {
        if (checkn == true) {
            bulletx = homiex;
            bullety = homeiy;
        }
        int xx = bulletx;
        int yy = bullety;
        if (direc == 'D') {
            if (playMap[bulletx + 1][homeiy]->identity == ' ') {
                playMap[++bulletx][homeiy]->identity = 'o';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
            } else if (playMap[bulletx + 1][homeiy]->identity == 'P') {
                playMap[++bulletx][homeiy]->identity = ' ';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                kills++;
                Pbox.pop_back();
            }
            else if (playMap[bulletx + 1][homeiy]->identity == 'x' || playMap[bulletx + 1][homeiy]->identity == 'n') {
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                bulletBoolean = false;
                return;
            }
        }
        if (direc == 'U') {
            if (playMap[bulletx - 1][homeiy]->identity == ' ') {
                playMap[--bulletx][homeiy]->identity = 'o';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
            } else if (playMap[bulletx - 1][homeiy]->identity == 'P') {
                playMap[--bulletx][homeiy]->identity = ' ';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                kills++;
                Pbox.pop_back();
            } else if (playMap[bulletx - 1][bullety]->identity == 'x' ||
                       playMap[bulletx - 1][bullety]->identity == 'n') {
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                bulletBoolean = false;
                return;
            }
        }
        if (direc == 'R') {
            if (playMap[bulletx][bullety + 1]->identity == ' ') {
                playMap[bulletx][++bullety]->identity = 'o';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
            } else if (playMap[bulletx][bullety + 1]->identity == 'P') {
                playMap[bulletx][++bullety]->identity = ' ';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                kills++;
                Pbox.pop_back();
            } else if (playMap[bulletx][bullety + 1]->identity == 'T') {
                TBox.pop_back();
                int count =0;
                while(playMap[bulletx][bullety + count]->identity !=' '){
                    playMap[bulletx][bullety + count]->identity = ' ';
                    playMap[bulletx][bullety + count]->isWall = false;
                    count++;

                }
                bulletBoolean = false;
                return;
            }
            else if(playMap[bulletx][bullety + 1]->identity == 'I'){
                Ibox.pop_back();
                int count =0;
                playMap[bulletx][bullety + 1]->isenemy = false;
                while(playMap[bulletx][bullety + count]->identity !=' '){
                    playMap[bulletx][bullety + count]->identity = ' ';
                    playMap[bulletx][bullety + count]->isWall = false;
                    count++;

                }
                bulletBoolean = false;
                return;
            }
            else if (playMap[bulletx][bullety + 1]->identity == 'x' ||
                       playMap[bulletx][bullety + 1]->identity == 'n') {
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                bulletBoolean = false;
                return;
            }
        }
        if (direc == 'L') {
            if (playMap[bulletx][bullety - 1]->identity == ' ') {
                playMap[bulletx][--bullety]->identity = 'o';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
            } else if (playMap[bulletx][bullety - 1]->identity == 'P') {
                playMap[bulletx][--bullety]->identity = ' ';
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                kills++;
                Pbox.pop_back();
            } else if (playMap[bulletx][bullety - 1]->identity == 'x' ||
                       playMap[bulletx][bullety - 1]->identity == 'n') {
                if (playMap[xx][yy]->identity != '@') {
                    playMap[xx][yy]->identity = ' ';
                }
                bulletBoolean = false;
                return;
            }
        }
        return;
    }

    void printMap() {
        printw("KILLS: ");
        addch(kills);
        printw("  ||  ");
        printw("GOLDS COLLECTED: ");
        printw("%c", golds);
        printw("\n");
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if(playMap[i][j]->identity=='@'){
                    attron(COLOR_PAIR(PLAYER_PAIR));
                    addch(playMap[i][j]->identity);
                }
                else if(playMap[i][j]->identity== 'x'||playMap[i][j]->identity=='n'){
                    attron(COLOR_PAIR(GRASS_PAIR));
                    addch(playMap[i][j]->identity);
                }
                else {
                    attron(COLOR_PAIR(MOUNTAIN_PAIR));
                    addch(playMap[i][j]->identity);
                }

            }
            printw("\n");
        }
    }

    void printlocalsPlayer() {
        for (int i = 0; i < Ibox.size(); i++) {
            cout << Ibox[i]->character << endl;
        }

        for (int i = 0; i < Pbox.size(); i++) {
            cout << Pbox[i]->character << endl;
        }
        for (int i = 0; i < TBox.size(); i++) {
            cout << TBox[i]->character << endl;
        }
        for (int i = 0; i < Openbox.size(); i++) {
            cout << Openbox[i]->character << endl;
        }
        for (int i = 0; i < Weapon.size(); i++) {
            cout << Weapon[i]->character << endl;
        }

    }

/*this  function contains the algorithm for moving the bullets, if the booolean
 * direction is false that means the bullets are going in the right direction and if it is
 * true they are moving towards the left. this function also makes sure that it moves the bullets back and forth
 * comming out of the O.
 * */
    void moveasterist(Items *list) {
        if (list->direction == false) {
            int y = list->yCordinate;
            //if the next character is a wall that means we will be changing directions so in that case we will change
            // change the direction boolean to true,  and the current spot will become empty
            if (playMap[list->xCordinate][y + 1]->identity == 'x' ||
                playMap[list->xCordinate][y + 1]->identity == 'n') {
                list->direction = true;
                playMap[list->xCordinate][list->yCordinate]->identity = ' ';

            } else if (playMap[list->xCordinate][y]->identity == '@') {
                gameOver = true;
            }
                //if we do not change direction than we will print * for the next spot and if the previos spot is not x it will be
                // blanked.
            else {
                playMap[list->xCordinate][list->yCordinate++]->identity = '*';
                if (playMap[list->xCordinate][y - 1]->identity == 'x' ||
                    playMap[list->xCordinate][y - 1]->identity == 'n') {
                    playMap[list->xCordinate][y - 1]->identity = playMap[list->xCordinate][y - 1]->identity;
                } else {
                    playMap[list->xCordinate][y - 1]->identity = ' ';
                }


            }
        } else {
            int y = list->yCordinate;
            if (playMap[list->xCordinate][y - 1]->identity == 'x' ||
                playMap[list->xCordinate][y - 1]->identity == 'n') {
                list->direction = false;
                playMap[list->xCordinate][list->yCordinate]->identity = ' ';
            } else if (playMap[list->xCordinate][y]->identity == '@') {
                gameOver = true;
            } else {
                playMap[list->xCordinate][list->yCordinate--]->identity = '*';
                if (playMap[list->xCordinate][y + 1]->identity == 'x' ||
                    playMap[list->xCordinate][y + 1]->identity == 'n') {
                    playMap[list->xCordinate][y + 1]->identity = playMap[list->xCordinate][y + 1]->identity;
                } else {
                    playMap[list->xCordinate][y + 1]->identity = ' ';
                }
            }
        }


    }

/*this function is resposible for dealing with the bullets comming out of the weapon
 * O, basically it shoots these bullets by changing the positions of the bullets which goes back and forth
 * */
    void weaponry() {
        for (int i = 0; i < Weapon.size(); i++) {
            int xx = Weapon[i]->xxx;
            int yy = Weapon[i]->yyy;
            moveasterist(Weapon[i]);
            playMap[xx][yy]->identity = 'O';
        }
    }
void printsixe(){
        cout << TBox.size() << endl;
        cout << Pbox.size() << endl;
        cout << Openbox.size() << endl;
    }

private:
    MapCharacter ***playMap;//contains all the characters of the map and their positions
    vector<Items *> Ibox;//contains I's
    vector<Items *> Pbox;//contains P's
    vector<Items *> TBox;//contains T's
    vector<Items *> Openbox;//contains's &, %, #, $, !
    vector<Items *> Weapon;
    char direc;
    int h;//height of the map
    int w;//width of the map
    char homiex, homeiy; //position of player
    int bulletx, bullety;
};

int kbhit(void) {
    int ch = getch();

    if (ch != ERR) {
        checkn = true;
        movement = ch;
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char **argv) {
    // a loop which keeps going until gameOver is true
    //getting information from the function.
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    printw("Welcome - Press Q to Exit\n");
    int height;
    int width;
    ifstream inf;
    inf.open("map.txt");
    inf >> height;
    inf >> width;
    char **map;
    map = new char *[height];
    for (int i = 0; i < height; i++) {
        map[i] = new char[width];
        for (int j = 0; j < width; j++) {
            inf >> map[i][j] >> noskipws;
            //  addch(map[i][j]);
        }
        inf >> std::skipws;
    }
    inf.close();
    EveryFunction firstMap(map, height, width);
    char decideChar;
    int count = 0;
    gameOver = false;
    start_color();
    init_pair(GRASS_PAIR, COLOR_YELLOW, COLOR_GREEN);
    init_pair(WATER_PAIR, COLOR_BLUE, COLOR_BLUE);
    init_pair(MOUNTAIN_PAIR, COLOR_CYAN, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_CYAN, COLOR_MAGENTA);
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    while (!gameOver) {
        //updating the locations of everyone
        if (kbhit()) {
            //here I will do the change in direction of the player part
            if (movement == 'q') {
                break;
            }
            if (movement != ' ') {
                firstMap.movementofPlayer(movement);
            } else {
                firstMap.bullet();
                bulletBoolean = true;
                checkn = false;
            }
        }
        if (bulletBoolean == true) {
            firstMap.bullet();
        }
        firstMap.gameO();
        firstMap.weaponry();
        clear();
        firstMap.printMap();
        refresh();
        SLEEP(50);
        count++;

    }
    refresh();
    getch(); //Make user press any key to close
    endwin();
    cout << count << endl;
    firstMap.printsixe();
    firstMap.printlocalsPlayer();
    return 0;
}
