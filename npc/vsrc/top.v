module top(
    input clk, reset,
    output [31:0] writedata, dataadr,
    output memwrite
);


    reg [31:0] pc, instr, readdata;


    mips my_mips(
        .clk(clk),
        .reset(reset),
        .pc(pc),
        .instr(instr),
        .memwrite(memwrite),
        .dataadr(dataadr),
        .writedata(writedata),
        .readdata(readdata)
    );


    imem my_imem(
        .a(pc[7:2]),
        .rd(instr)
    );


    dmem my_dmem(
        .clk(clk),
        .we(memwrite), 
        .a(dataadr),
        .wd(writedata),
        .rd(readdata)
    );

endmodule