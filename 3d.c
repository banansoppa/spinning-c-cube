#include "stdio.h"
#include "math.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdlib.h"

struct point3 {
    float x;
    float y;
    float z;
};

struct point2 {
    float x;
    float y;
};

struct line2 {
    struct point2 start;
    struct point2 end;
};

void 
printpoint3(struct point3 *pt, char* str) 
{
    printf("(%f, %f, %f) ", pt->x, pt->y, pt->z);
    printf(str);
}

void 
draw(struct point2* pt, struct line2* ln, int w, int h, float scalex, float scaley, int ptlen, int lnlen)
{
    char out[h][w+1];
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w+1; x++) {
            out[y][x] = ' ';
        }
    }
    //draw all the points
    for(int i = 0; i < ptlen; i++) {
        int ax = (pt[i].x+1)*scalex;
        int ay = (pt[i].y+1)*scaley;
        if(ax<w && ay<h && ax>=0 && ay>=0) {
            out[ay][ax] += 1;
        }
    }
    //draw all the lines
    for(int i = 0; i < lnlen; i++) {
        float p1x, p2x, p1y, p2y;
        if(ln[i].start.x<ln[i].end.x) {
            p2x = (ln[i].end.x+1)*scalex;
            p1x = (ln[i].start.x+1)*scalex;
            p2y = (ln[i].end.y+1)*scaley;
            p1y = (ln[i].start.y+1)*scaley;
        } else {
            p2x = (ln[i].start.x+1)*scalex;
            p1x = (ln[i].end.x+1)*scalex;
            p2y = (ln[i].start.y+1)*scaley;
            p1y = (ln[i].end.y+1)*scaley;
        }
        float k = ((float)p2y-p1y)/(p2x-p1x);
        for(float x = 0; x < p2x-p1x; x+=0.1) {
            int newx = p1x+x;
            int newy = p1y+(x*k);
            if(newx<w-1 && newy<h-1 && newx>=1 && newy>=1) {
                out[newy][newx] += 1;
            }
        }
    }
    for(int y = -1; y < h; y++) {
        for(int x = 0; x < w+1; x++) {
            putchar(out[y][x]);
        }
        putchar('\n');
    }
}

struct point3 
rotateX(struct point3 p, float angle)
{
    float y = p.y*cos(angle)-p.z*sin(angle);
    float z = p.y*sin(angle)+p.z*cos(angle);
    struct point3 output = {p.x, y, z};
    return output;
}

struct point3 
rotateZ(struct point3 p, float angle) {
    float x = p.x*cos(angle)-p.y*sin(angle);
    float y = p.x*sin(angle)+p.y*cos(angle);
    struct point3 output = {x, y, p.z};
    return output;
}

//assumes the viewer is located at (0, 0, VIEWER_POS), and that the viewer is looking straight forward, the projection plane becomes x+0y+0z=0
struct point2
point3flatten(struct point3 p, float viewer_distance) {
    float t = -viewer_distance/(p.x-viewer_distance);
    float flatx = t*p.y;
    float flaty = t*p.z;
    struct point2 output = {flatx, flaty};
    return output;
}

#define VIEWER_POS -1
#define CUBE_POS 2

#define PI 3.1415

int main(int argc, char **argv) {
    int frames = 200;
    float angle1 = 3*(PI/10);
    float angle2 = 4*(PI/10);
    int scalex = 14;
    int scaley = 10;
    int height = 20;
    int width = 40;
    if(argc>1) {
        frames = atoi(argv[1]);
    }  
    if (argc > 2) {
        angle1 = atoi(argv[2])*(PI/10);
        angle2 = atoi(argv[3])*(PI/10);
    } if (argc > 4) {
        width = atoi(argv[4]);
        height = atoi(argv[5]);
        scalex = atoi(argv[6]);
        scaley = atoi(argv[7]);
    }

    struct point3 points[8];
    struct point2 flatpoints[8];
    struct line2 flatlines[12];
    for(int count = 0; count < frames; count++) {
        angle1 += 0.1;
        angle2 -= 0.04;
        //recalculate where the points and lines are.
        for(int x = 0; x < 2; x++) {
            for(int y = 0; y < 2; y++) {
                for(int z = 0; z < 2; z++) {
                    int index = x*4+y*2+z;
                    float ax = x*2-1;
                    float ay = y*3-2;
                    float az = z*2-1;
                    struct point3 point = {ax, ay, az};
                    point = rotateX(point, angle1);
                    point = rotateZ(point, angle2);
                    point.x += CUBE_POS;
                    points[index] = point;
                    flatpoints[index] = point3flatten(points[index], VIEWER_POS);
                }            
            }
        }

        flatlines[0].start = flatpoints[0];
        flatlines[0].end = flatpoints[1];
        flatlines[1].start = flatpoints[0];
        flatlines[1].end = flatpoints[2];
        flatlines[2].start = flatpoints[0];
        flatlines[2].end = flatpoints[4];
        
        flatlines[3].start = flatpoints[7];
        flatlines[3].end = flatpoints[5];
        flatlines[4].start = flatpoints[7];
        flatlines[4].end = flatpoints[6];
        flatlines[5].start = flatpoints[7];
        flatlines[5].end = flatpoints[3];

        flatlines[6].start = flatpoints[5];
        flatlines[6].end = flatpoints[4];
        flatlines[7].start = flatpoints[5];
        flatlines[7].end = flatpoints[1];

        flatlines[8].start = flatpoints[4];
        flatlines[8].end = flatpoints[6];
        flatlines[9].start = flatpoints[2];
        flatlines[9].end = flatpoints[6];

        flatlines[10].start = flatpoints[3];
        flatlines[10].end = flatpoints[2];
        flatlines[11].start = flatpoints[3];
        flatlines[11].end = flatpoints[1];

        //printf("\n\n\n\n");
        draw(flatpoints, flatlines, width, height, scalex, scaley, 8, 12);
        printf("\033[%dA", height+1);
        printf("\033[%dD", width+1);
        
        usleep(100000);
    }
    for(int i = 0; i < height+1; i++) {
        putchar('\n');
    }
    return 0;
}
