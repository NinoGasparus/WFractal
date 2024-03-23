#include "../globalVars.h"
#include "../helperFunctions/helperFunctions.h"
#include <iostream>
void startThreads(){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
           //    std::cout << "x: " << x << ", y: " << y << std::endl;
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
                 
                try{
                
                if (c == maxIteration) {
                   // printf("aaaaaaaaaaaaaaaaaaaaaaa");
                    data[x][y][0] = 0;
                    continue;
                }   
                //    printf("b");
                data[x][y][0] = c;
                }catch(std::exception e){

                }      

        }
    }
}

void reDraw(){
    startThreads();

    for (int i = 0; i < width; i += 1) {
        for ( int j = 0; j < height; j += 1) {
			//Navigation through the array with step of 4 on both i and j.
            int index = (j * 4) * width + (i * 4);
            

            int delta = 0;
            
            delta = (int)helperFunctions::mapValue(data[i][j][0],0,maxIteration,0,255);

            pictureData[index]   = delta;
            pictureData[index+1] = delta;
            pictureData[index+2] = delta;
            pictureData[index+3] = 255;
        }
    }
	
	picture.update(pictureData);
	sprite.setTexture(picture,true);

    printf("Window Redrawn\n");
}