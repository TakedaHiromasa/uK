typedef volatile unsigned char* u8;
#define IND_u8(a) (*((u8)a)) /* indirection 関節参照 */

#define GLED_ADDR ((volatile unsigned char*)0x4005)
#define GLED (*(GLED_ADDR))

int a = 0xAA55;
int b;
char c = 0xA5;
char d;

static int e = 0x55AA;
static int f;

void kernel_start()
{
  static int g = 0xA5A5;
  static int h;
	
  while(1){
		GLED = 0xFF;
	  }
}