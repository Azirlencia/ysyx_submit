module imem(
    input [5:0] a,
    output [31:0] rd
);

    reg [31:0] RAM[63:0];
    initial begin

        $readmemh("memfile.dat", RAM, 0, 63);
    end  
    assign rd = RAM[a];

endmodule