#include "../toybox.h"
#include <set>
#include <utility>
#include <random>
#include <string>

enum Direction { STILL, UP, DOWN, LEFT, RIGHT };
Direction dir = STILL;
bool fruit_mode, K_mode, flag = true;
const int MYFPS = 26;
int score, enemy_num, fpscnt, FPSCNT = 2*MYFPS;
std::random_device seed;  // 硬件生成随机数种子
std::ranlux48 engine(seed()); // 利用种子生成随机数引擎

std::set<std::pair<int,int>> Kmode_bullet;
std::set<std::pair<int,int>> Kmode_bullet2; // 分裂弹
std::set<std::pair<int,int>> normal_bullet;
std::set<std::pair<int,int>> targets; // 敌人坐标

void keypress(int key) {
    switch (key) {
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'k': {
            fruit_mode = false;
            K_mode = !K_mode;
            break;
        }
        case 'f': {
            K_mode = false;
            fruit_mode = !fruit_mode;
            break;
        }
    }
}

int fly_x, fly_y;
// --x--------
// |     /\
// y    /  \
// |    ----
// |
void drawBody(int w, int h, draw_function draw) {
    switch (dir) {
        case STILL: break;
        case UP:
            if(fly_y-3 >= 0) fly_y--;
            break;
        case DOWN:
            if(fly_y+1 < h) fly_y++;
            break;
        case LEFT:
            if(fly_x-1 >= 0) fly_x--;
            break;
        case RIGHT:
            if(fly_x+4 < w) fly_x++;
            break;
    }
    dir = STILL;

    draw(fly_x+1, fly_y-2, '/');
    draw(fly_x+2, fly_y-2, '\\');
    draw(fly_x, fly_y-1, '/');
    draw(fly_x+3, fly_y-1, '\\');
    for(int i = 0; i < 4; i++)
        draw(fly_x+i, fly_y, '-');
}

int get_rand_num(int minr, int maxr)
{
    std::uniform_int_distribution<> distrib(minr, maxr);
    int my_random = distrib(engine); // 随机数
    return my_random;
}

void updBullet(int w, int h, draw_function draw) {
    auto it = normal_bullet.begin();
    for(; it != normal_bullet.end();) {
        int nx = it->first, ny = it->second-2;
        it = normal_bullet.erase(it);
        if(ny < 0) continue;
        normal_bullet.insert({nx, ny});
    }

    auto it2 = Kmode_bullet.begin();
    for(; it2 != Kmode_bullet.end();) {
        int nx = it2->first, ny = it2->second-2;

        if(ny >= 0) {
            if(nx+1 <= w)
                Kmode_bullet2.insert({nx+1, ny});
            if(nx-1 >= 0)
                Kmode_bullet2.insert({nx-1, ny});
        }

        it2 = Kmode_bullet.erase(it2);
        if(ny < 0) continue;
        Kmode_bullet.insert({nx, ny});
    }

    if(!K_mode) {
        normal_bullet.insert({fly_x+1, fly_y-3});
        normal_bullet.insert({fly_x+2, fly_y-3});
        if(fruit_mode) {
            normal_bullet.insert({fly_x, fly_y-3});
            normal_bullet.insert({fly_x+3, fly_y-3});
        }
    } else {
        Kmode_bullet.insert({fly_x+1, fly_y-3});
        Kmode_bullet.insert({fly_x+2, fly_y-3});
    }

    Kmode_bullet.insert(Kmode_bullet2.begin(), Kmode_bullet2.end());
    Kmode_bullet2.clear();
}

void collision_detection() {
    // normal_bullet 不会与 Kmode_bullet 碰撞
    for(auto it = targets.begin(); it != targets.end();) {
        bool hit = false;
        auto b = normal_bullet.begin();
        for(; b != normal_bullet.end(); ++b) {
            if(*it == *b) {
                hit = true;
                normal_bullet.erase(b);
                break;
            }
        }
        auto kb = Kmode_bullet.begin();
        for(; kb != Kmode_bullet.end(); ++kb) {
            if(*it == *kb) {
                hit = true;
                Kmode_bullet.erase(kb);
                break;
            }
        }
        if(hit) {
            it = targets.erase(it);
            ++score;
        }
        else ++it;
    }
}

void updEnemy(int w, int h, draw_function draw) {
    auto it = targets.begin();
    for(; it != targets.end();) {
        int nx = it->first, ny = it->second+1;
        it = targets.erase(it);
        if(ny >= h) continue;
        targets.insert({nx, ny});
    }

    collision_detection();

    if(fpscnt > FPSCNT) {
        fpscnt = 0;
        int target_num = get_rand_num(1, static_cast<int>(w/4.0));
        for(int i=1; i<=target_num; ++i) {
            int nx = get_rand_num(1, w);
            int ny = get_rand_num(-8, 1); // 扰动
            targets.insert({nx, ny});
        }
        FPSCNT = get_rand_num(1, static_cast<int>(MYFPS/1.0));
    }
}

void drawPic(int w, int h, draw_function draw) {
    auto it = normal_bullet.begin();
    for(; it != normal_bullet.end(); it++) {
        draw(it->first, it->second, '^');
    }
    
    auto it2 = Kmode_bullet.begin();
    for(; it2 != Kmode_bullet.end(); it2++) {
        draw(it2->first, it2->second, '~');
    }

    auto it3 = targets.begin();
    for(; it3 != targets.end(); it3++) {
        draw(it3->first, it3->second, '@');
    }
}

void drawScore(int w, int h, draw_function draw) {
    draw(1, h-1, 's');
    draw(2, h-1, 'c');
    draw(3, h-1, 'o');
    draw(4, h-1, 'r');
    draw(5, h-1, 'e');
    draw(6, h-1, ':');
    draw(7, h-1, ' ');
    std::string score_as_string = std::to_string(score);
    for(int i=0; i<score_as_string.size(); i++) {
        draw(8+i, h-1, score_as_string[i]);
    }
}

void update(int w, int h, draw_function draw) {
    if(flag) {
        fly_x = w/2, fly_y = h-1;
        flag = false;
    }
    
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            draw(x, y, ' ');
        }
    }

    ++fpscnt;
    drawBody(w, h, draw);
    updBullet(w, h, draw);
    updEnemy(w, h, draw);
    drawPic(w, h, draw);
    drawScore(w, h, draw);
}

int main() {
    toybox_run(MYFPS, update, keypress);
    return 0;
}