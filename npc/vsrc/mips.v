module mips(
    input clk, reset,
    output [31:0] pc,
    input [31:0] instr,
    output memwrite,
    output [31:0] aluresult, 
    output [31:0] writedata,
    input [31:0] readdata
);

    reg memtoreg;
    reg [1:0] alusrc; // LUI
    reg regdst; 
    reg regwrite, jump, pcsrc, zero;
    reg [3:0] alucontrol; // SLL
    reg ltez; // BLEZ

    controller c(
        .instr(instr[31:26]), 
        .funct(instr[5:0]), 
        .zero(zero),
        .memtoreg(memtoreg), 
        .memwrite(memwrite), 
        .pcsrc(pcsrc),
        .alusrc(alusrc), 
        .regdst(regdst), 
        .regwrite(regwrite), 
        .jump(jump),
        .alucontrol(alucontrol), 
        .ltez(ltez)  // BLEZ
    );

    datapath dp(
        .clk(clk), 
        .reset(reset), 
        .memtoreg(memtoreg), 
        .pcsrc(pcsrc),
        .alusrc(alusrc), 
        .regdst(regdst), 
        .regwrite(regwrite), 
        .jump(jump),
        .alucontrol(alucontrol),
        .zero(zero), 
        .pc(pc), 
        .instr(instr),
        .aluresult(aluresult), 
        .writedata(writedata), 
        .readdata(readdata), 
        .ltez(ltez)  // BLEZ
    );

endmodule