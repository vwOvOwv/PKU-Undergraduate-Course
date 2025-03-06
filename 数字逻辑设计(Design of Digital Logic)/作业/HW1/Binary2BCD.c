#include <stdio.h>
#include <stdlib.h>

unsigned int Bin2BCD(unsigned short BIN_in){ // BIN_in is a 16-bit positive binary number
    unsigned int BCD_out = 0;
    unsigned int BCD_in = 0;
    for(int i = 0; i < 16; i++){
        BCD_in = (BIN_in >> (15 - i)) & 1;
        // printf("%d", BCD_in);
        BCD_out = (BCD_out << 1) | BCD_in;
        if(i != 15)
            for(int j = 0; j < 8; j++){
                // printf("%d", (BCD_out >> (j * 4)) & 0xF);
                if(((BCD_out >> (j * 4)) & 0xF) >= 5){
                    BCD_out += (3 << (j * 4));
                }
            }
        // printf("0x%x\n", BCD_out);
    }
    return BCD_out;
}

int main(){
    unsigned short BIN_in;
    while (1) {
        printf("Please input a 16-bit binary number (range: [0, 65535]): ");
        scanf("%hu", &BIN_in);
        // printf("The binary number you input is: 0x%x\n", BIN_in);
        printf("The BCD code of the binary number is: 0x%x\n", Bin2BCD(BIN_in));
    }
    return 0;
}