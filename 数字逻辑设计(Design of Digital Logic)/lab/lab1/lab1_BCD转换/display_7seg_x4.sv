`timescale 1ns / 1ps

module display_7seg_x4(
    input logic CLK_1KHz,
    input logic [3:0] in3, in2, in1, in0,
    output logic [0:3] an,
    output logic [0:6] seg
    );
    
    logic [1:0] sel;
    logic [3:0] seg7_dec_in;
    
    always_ff @(posedge CLK_1KHz)
    begin
        sel <= sel + 1;
    end
    
    always_comb
    begin
        case (sel)
        0: begin an = 4'b0111; seg7_dec_in = in0; end
        1: begin an = 4'b1011; seg7_dec_in = in1; end
        2: begin an = 4'b1101; seg7_dec_in = in2; end
        3: begin an = 4'b1110; seg7_dec_in = in3; end
        default: begin an = 4'b1111; seg7_dec_in = 4'bxxxx; end
        endcase
    end
    
    always_comb
    begin
        case (seg7_dec_in)
        //          ABC_DEFG
        0: seg = 7'b000_0001;
        1: seg = 7'b100_1111;
        2: seg = 7'b001_0010;
        3: seg = 7'b000_0110;
        4: seg = 7'b100_1100;
        5: seg = 7'b010_0100;
        6: seg = 7'b010_0000;
        7: seg = 7'b000_1111;
        8: seg = 7'b000_0000;
        9: seg = 7'b000_0100;
        default: seg = 7'bxxx_xxxx;
        endcase
    end
endmodule
