module datapath(
    input clk, reset,
    input memtoreg, pcsrc,
    input [1:0] alusrc,//LUI
    input regdst,
    input regwrite, jump,
    input [3:0] alucontrol,
    output zero,
    output [31:0] pc,
    input [31:0] instr,
    output [31:0] aluout, writedata,
    input [31:0] readdata,
	output ltez
);


    reg [4:0] writereg;
    reg [31:0] pcnext, pcnextbr, pcplus4, pcbranch;
    reg [31:0] signimm, signimmsh;
	reg [31:0] upperimm; //LUI
    reg [31:0] srca, srcb;
    reg [31:0] result;
	reg [31:0] memdata;

//next pc
    flopr #(32) pcreg(clk, reset, pcnext, pc);
    adder pcadd1(pc, {16'b0, 16'b1}, pcplus4);
	s12 immsh(signimm, signimmsh);
	adder pcadd2(pcplus4, signimmsh, pcbranch);   
    mux2 #(32) pcbrmux(pcplus4, pcbranch, pcsrc, pcnextbr);
    mux2 #(32) pcmux(pcnextbr, {pcplus4[31:28], instr[25:0], 2'b00}, jump, pcnext);
    
//register file
    regfile rf(clk, regwrite, instr[25:21], instr[20:16], writereg, result, srca, writedata);    
    mux2 #(5) wrmux(instr[20:16], instr[15:11], regdst, writereg);    
    signext se(instr[15:0], signimm);
	upimm ui(instr[15:0], upperimm);
	
//ALU 
    mux2 #(32) resmux(aluout, readdata, memtoreg, result);
    mux3 #(32) srcbmux(writedata, signimm, upperimm, alusrc, srcb);//LUI
    alu alu(srca, srcb, alucontrol, instr[10:6], aluout, zero ,ltez);
    
endmodule