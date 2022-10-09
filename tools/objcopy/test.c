#include <stdio.h>

extern char _binary_image_png_start;
extern char _binary_image_png_end;
extern char _binary_image_png_size;

int main()
{
      int i = 0;
      unsigned char *p = NULL;
      printf("_binary_image_png_start 0x%lx\n",(unsigned long)&_binary_image_png_start);
      printf("_binary_image_png_end 0x%lx\n",(unsigned long)&_binary_image_png_end);

      p = (unsigned char *)(unsigned long)&_binary_image_png_start;
      printf("First 8 bytes of the Image : \n");
      for(i=0;i<8;++i){
            printf("0x%02x ",*p++);
      }
      printf("\n");

      return 0;
}

