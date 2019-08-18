/**
# Created By: Noman^Alien#0637
# Created On: 7/31/2019; 5:19 AM
*/

#include <Console/Buffer.h>

/* ============================== Constructors ========================= */

CBuffer *cbuffer_init(unsigned short int screenSizeX,
                      unsigned short int screenSizeY, unsigned short int posX,
                      unsigned short int posY, size_t bufferRows,
                      size_t bufferCols, unsigned char scrollEnabled,
                      unsigned short int noOfContainedBuffers,
                      CBuffer **containedBuffers) {
	
	CBuffer *cBuffer = (CBuffer *) malloc(sizeof(CBuffer));
	
	cBuffer->screenSize = calloc(2, sizeof(unsigned short int));
	cBuffer->screenSize[0] = screenSizeX;
	cBuffer->screenSize[1] = screenSizeY;
	
	cBuffer->position = calloc(2, sizeof(unsigned short int));
	
	cBuffer->bufferSize = calloc(2, sizeof(unsigned short int));
	cBuffer->bufferSize[0] = bufferRows;
	cBuffer->bufferSize[1] = bufferCols;
	
	cBuffer->cursorPos = 0;
	cBuffer->bufferOverflow = 0;
	
	// Allocating a 2D array with continuous block of memory
	cBuffer->buffer = (char **) calloc(bufferRows, sizeof(char *));
	cBuffer->buffer[0] = (char *) calloc(bufferRows * bufferCols, sizeof(char));
	for (; bufferRows > 0; bufferRows--) {
		cBuffer->buffer[bufferRows] = cBuffer->buffer[0] + bufferRows * bufferCols;
	}
	
	cBuffer->noOfContainedBuffers = noOfContainedBuffers;
	cBuffer->containedBuffers = containedBuffers;
	
	cBuffer->scrollEnabled = (scrollEnabled == 'N' || scrollEnabled == 0) ? 0 : 1;
	
	cBuffer->position[0] = posX;
	cBuffer->position[1] = posY;
	
	return cBuffer;
}

/* ============================== Writing To Buffers =========================
 */

char *_tempCBufferPntr = NULL;
size_t _tempStringLen = 0;

/**
 * Handles Buffer 	Overwriting and Detects Buffer Overflow
 * @param cBuffer 	The Console Buffer Instance in which Data will be
 * written
 * @param strLength	Length of the new formatted data
 * @return			Total Number of Characters it will write into
 * cBuffer
 */
size_t _cbuffer_manageHandle(CBuffer *cBuffer, int strLength) {
	/*	size_t i; */
	
	size_t len = cbuffer_maxLen(cBuffer);
	size_t cursorPos = cBuffer->cursorPos;
	size_t spaceAvailable = len - cursorPos;
	size_t shiftLeft = strLength - spaceAvailable;
	_tempStringLen = len;
	
	if (spaceAvailable >= strLength) {
		/* Appending Buffer */
		_tempCBufferPntr = cBuffer->buffer[0] + cursorPos;
		
		cBuffer->cursorPos += strLength;
	} else if (strLength <= len) {
		// Overwriting Buffer
		
		/* Alternative Buffer Overwriting to String Copy */
		/*
			for (i = 0; i < shiftLeft; i++) {
				cBuffer->buffer[0][i] = cBuffer->buffer[0][i + shiftLeft];
			}
		*/
		
		strcpy(cBuffer->buffer[0], cBuffer->buffer[0] + shiftLeft + 1);
		_tempCBufferPntr = cBuffer->buffer[0] + (len - strLength - 1);
		
		// At the end of overwriting and new data entry the cursor will be at the end too
		cBuffer->cursorPos = len;
	} else {
		cBuffer->bufferOverflow = 1;
		_tempCBufferPntr = (char *) calloc(strLength, sizeof(char));
		_tempStringLen = strLength;
	}
	
	return strLength <= len ? strLength : len;
}

/**
 * Resets the temporary buffers and handles Buffer Overflows
 * @param cBuffer Console buffer to work on
 */
void _cbuffer_resetHandle(CBuffer *cBuffer) {
	if (cBuffer->bufferOverflow) {
		size_t len = cbuffer_maxLen(cBuffer);
		strcpy(cBuffer->buffer[0], _tempCBufferPntr + (_tempStringLen - len + 1));
		cBuffer->cursorPos = len;
		free(_tempCBufferPntr);
	}
	_tempCBufferPntr = NULL;
	_tempStringLen = 0;
}

/* ============================== Rendering/Printing Buffer ========================= */

void _cbuffer_render(CBuffer *cBuffer) {
	if (cBuffer->noOfContainedBuffers) {
		// todo: render the container buffers here
	}
}

void cbuffer_show(CBuffer *cBuffer) {
	_cbuffer_render(cBuffer);
	size_t rows = (cBuffer->screenSize[0] < cBuffer->bufferSize[0]) ?
	              cBuffer->screenSize[0] : cBuffer->bufferSize[0];
	size_t cols = (cBuffer->screenSize[1] < cBuffer->bufferSize[1]) ?
	              cBuffer->screenSize[1] : cBuffer->bufferSize[1];
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			printf("%c", cBuffer->buffer[i][j]);
		}
		printf("\n");
	}
	
	_cbuffer_liveRender(cBuffer);
}


void _cbuffer_liveRender(CBuffer *cBuffer) {
	char *liveRenderCommand = calloc(100, sizeof(char));
	printf(">>> ");
	scanf("%[^\n]%*c", liveRenderCommand);
	
	
}

/* ============================== Buffer: Utility ========================= */

size_t cbuffer_maxLen(CBuffer *cBuffer) {
	return cBuffer->bufferSize[0] * cBuffer->bufferSize[1];
}
