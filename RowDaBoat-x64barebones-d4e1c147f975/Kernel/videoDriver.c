
#include <videoDriver.h>
#include <textDriver.h>

static const VBEModeInfoBlock * vesaInfo =  (uint64_t*)0x0000000000005C00;
static uint32_t videoCursor = 0;


void putpixel(int x,int y) {
	if(validPixelPosition(x,y)) {
		uint8_t * position = vesaInfo->PhysBasePtr + ( x + y* vesaInfo->XResolution)*3;
	    position[0] = 0xFF;          
	    position[1] = 0xFF; 
	    position[2] = 0xFF; 
	}
}

void putBlackPixel(int x, int y){
	int8_t * position = vesaInfo->PhysBasePtr + ( x + y* vesaInfo->XResolution)*3;
	position[0] = 0x00;          
	position[1] = 0x00; 
	position[2] = 0x00; 
}

int validPixelPosition(int x, int y) {
	if(x < vesaInfo->XResolution && y < vesaInfo->YResolution && x>=0 && y>=0)
		return 1;

	return 0;
}

int validCharPosition(int x, int y) {
	if(x>=0 && y>=0 && x<vesaInfo->XResolution/CHAR_WIDTH && y<vesaInfo->YResolution/CHAR_HEIGHT)
		return 1;

	return 0;
}

void printChar(char c) {
	if(videoCursor >= charsPerHeight*charsPerLine){
		endScreen();
	}

	if(c == '\n'){
		nLine();
	}
	else if(c == '\b'){
		//Borra aunque este en la primer posicion
		erase();
	}
	else{
		int y = videoCursor / charsPerLine;
		int x = videoCursor % charsPerLine;
		unsigned char * char_map = pixel_map(c);

		for(int i = 0; i < CHAR_HEIGHT; i++) {
			unsigned char pixel = char_map[i];
			printCharLine(pixel, x*CHAR_WIDTH, (y*CHAR_HEIGHT+i));
		}
		videoCursor++;
	}
}


void endScreen(){
	for(int i = 0; i < (vesaInfo->YResolution - CHAR_HEIGHT); i++){
		for(int j = 0; j < (vesaInfo->XResolution) * 3; j++){
			*((uint8_t *)(vesaInfo->PhysBasePtr) + i * charsPerLine*CHAR_WIDTH * 3 + j) = *((uint8_t *)(vesaInfo->PhysBasePtr) + (i + CHAR_HEIGHT) * charsPerLine*CHAR_WIDTH*3 + j);
		}
	}
	for(int i = vesaInfo->YResolution - CHAR_HEIGHT; i < vesaInfo->YResolution; i++){
		for(int j = 0; j < vesaInfo->XResolution; j++){
			putBlackPixel(j, i);
		}
	}
	videoCursor -= charsPerLine; 
}
void blackOut(){
	for(int i = 0; i < vesaInfo->XResolution; i++){
		for(int j = 0; j < vesaInfo->YResolution; j++){
			putBlackPixel(i, j);
		}
	}
	videoCursor = 0;
}
void printCharLine(unsigned char line, int x, int y) {
	char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
	int j = 0;
	char aux;
	for (j = 0; j < CHAR_WIDTH; j++) {
		aux = line & mask[j];
		if (aux != 0) { 
			putpixel(x+j, y);
		}
	}
}

void printString(char*  s){
	while(*s){
		printChar(*s);
		s++;
	}
}

void nLine(){
	if(videoCursor/charsPerLine == charsPerHeight -1){
		endScreen();
	}									
	videoCursor += charsPerLine - videoCursor % charsPerLine;	
}

void erase() {
	
	if(videoCursor == 0)
		return;
	videoCursor --;
	for(int i = 0; i< CHAR_HEIGHT; i++){
		for(int j = 0; j < CHAR_WIDTH ; j++){
			putBlackPixel((videoCursor % charsPerLine) * CHAR_WIDTH + j, (videoCursor / charsPerLine) * CHAR_HEIGHT + i);
		}
	}
}




