module regfile(
    input clk,
    input we3,
    input [4:0] ra1, ra2, wa3,
    input [31:0] wd3,
    output [31:0] rd1, rd2
);

    reg [31:0] rf[31:0];
    

    always @(posedge clk) begin
        if (we3) begin
            rf[wa3] <= wd3; 
        end
    end

    assign rd1 = (ra1 != 0) ? rf[ra1] : 32'b0; 
    assign rd2 = (ra2 != 0) ? rf[ra2] : 32'b0; 


    initial begin
        rf[0] = 32'b0;
    end

endmodule