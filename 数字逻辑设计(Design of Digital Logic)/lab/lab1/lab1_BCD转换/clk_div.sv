`timescale 1ns / 1ps

module clk_div
    #(parameter DIVISION = 100000000)
    (
    input  logic clk_100MHz,        // 100MHz
    output logic clk_sys     // 1Hz; 100MHz / DIVISION
    );
    
    logic [26:0] div_counter;
    logic clk_div;
    
    always_ff @(posedge clk_100MHz)
    begin
        if (div_counter == DIVISION / 2)
        begin
            clk_div <= ~clk_div;
            div_counter <= 0;
        end
        else
        begin
            div_counter <= div_counter + 1;
        end
    end
    
    BUFG CLK0_BUFG_INST (.I(clk_div),
                         .O(clk_sys));
endmodule
