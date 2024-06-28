

module maindec(
    input [5:0] op,
    output reg memtoreg, memwrite,
    output reg branch, 
    output reg [1:0] alusrc, // LUI
    output reg regdst,
    output reg regwrite,
    output reg jump,
    output reg [1:0] aluop,
    output reg blez // BLEZ
);

    // Increase control width for LUI, BLEZ
    reg [10:0] controls;

    assign {regwrite, regdst, alusrc, branch, memwrite,
            memtoreg, aluop, jump, blez} = controls;

    always @(op) begin
        case(op)
            6'b000000: controls = 11'b11000001000; // R-type
            6'b100011: controls = 11'b10010010000; // LW
            6'b101011: controls = 11'b00010100000; // SW
            6'b000100: controls = 11'b00001000100; // BEQ
            6'b001000: controls = 11'b10010000000; // ADDI
            6'b000010: controls = 11'b00000000010; // J
            6'b001010: controls = 11'b10010001100; // SLTI
            6'b001111: controls = 11'b10100000000; // LUI
            6'b000110: controls = 11'b00000000101; // BLEZ
            default: controls = 11'bxxxxxxxxxxx; // ???
        endcase
    end
endmodule
