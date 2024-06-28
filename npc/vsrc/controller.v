module controller(
    input [5:0] op, funct,
    input zero,
    output reg memtoreg, memwrite,
    output reg pcsrc, 
    output reg [1:0] alusrc, // LUI
    output reg regdst,
    output reg regwrite,
    output reg jump,
    output reg [3:0] alucontrol, // SLL
    input ltez // BLEZ
);

    wire [1:0] aluop;
    wire branch;
    wire blez; // BLEZ

    maindec md(
        .op(op), 
        .memtoreg(memtoreg), 
        .memwrite(memwrite), 
        .branch(branch),
        .alusrc(alusrc), 
        .regdst(regdst), 
        .regwrite(regwrite), 
        .jump(jump),
        .aluop(aluop),
        .blez(blez) // BLEZ
    );

    aludec ad(
        .funct(funct), 
        .aluop(aluop), 
        .alucontrol(alucontrol)
    );

    // BLEZ
    always @(*) begin
        pcsrc = (branch & zero) | (blez & ltez); 
    end
endmodule
