#include "../globalVars.h"
#include "../helperFunctions/helperFunctions.h"

void startThreads(){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
                double xo = helperFunctions::mapValue(x, 0, 1920, (nx / z)+xoff, (px / z)+xoff);
                double yo = helperFunctions::mapValue(y, 0, 1080, (ny / z)+yoff, (py / z)+yoff);

                double r = xo;
                double i = yo;
                int c = 0;
                double a, b, d;

                while ((r * r + i * i )<4 && ++c < maxIteration) {
                    a = r * r;
                    b = 2 * r * i;
                    d = i * i * -1;

                    r = a + d + xo;
                    i = b + yo;
                }

                if (c == maxIteration) {
                    data[y][x][0] = 0;
                    continue;
                }

                data[y][x][0] = c;
        }
    }
    
}