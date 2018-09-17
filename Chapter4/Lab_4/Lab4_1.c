#include <stdint.h>
#include <stdbool.h>

#define WADDRESS 0x20002000
#define RADDRESS 0x20003000

// Function prototypes
void pushdata(int);
void popdata(int);

int main(void) {
	uint32_t uiData;
	
	for (uiData = 0; uiData < 10; uiData++) {
		pushdata(uiData);
	}
	
	for (uiData = 0; uiData < 10; uiData++) {
		popdata(uiData);
	}
}

void pushdata(int uidata) {
	int R0, R2, SP;
	
	__asm {
		mov R0, WADDRESS
		mov SP, R0
		mov R2, uidata
		str R2, [SP,uidata]
	}
}

void popdata(int uidata) {
	int R0, R1, R3, SP;
	
	__asm {
		mov R0, WADDRESS
		mov R1, RADDRESS
		mov SP, R0
		ldr R3, [SP, uidata]
		str R3, [R1, uidata]
	}
}
