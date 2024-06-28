module dmem(input clk,we,
            input [31:0] a,wd,
			output [31:0]rd);
			
	reg [31:0] RAM[63:0];
    initial begin
        for (int i = 0; i < 64; i = i + 1) begin
            RAM[i] = 32'b0;  
        end
    end

    assign rd = RAM[a[31:2]];


    always @(posedge clk) begin
        if (we) begin
            RAM[a[31:2]] <= wd;
        end
    end

endmodule