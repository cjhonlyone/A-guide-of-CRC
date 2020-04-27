//-----------------------------------------------------
// Design Name : serial_crc_ccitt
// File Name   : serial_crc.v
// Function    : CCITT Serial CRC
// Coder       : Deepak Kumar Tala
//-----------------------------------------------------
module serial_crc_ccitt #(
    parameter [15:0] init_value = 16'h0000)
    (
    //-----------Input Ports---------------
    input clk     ,
    input reset   ,
    input enable  ,
    input init    ,
    input m ,
    //-----------Output Ports---------------
    output [15:0] crc_out
    );

//------------Internal Variables--------
reg   [15:0] c;
//-------------Code Start-----------------
assign crc_out = c;
// Logic to CRC Calculation
always @ (posedge clk) begin
    if (reset) begin
        c <= init_value;
    end else if (enable) begin
        if (init) begin
            c <= init_value;
        end else begin
            c[0]  <= c[15] ^ m;
            c[1]  <= c[0];
            c[2]  <= c[1];
            c[3]  <= c[2];
            c[4]  <= c[3];
            c[5]  <= c[4] ^ c[15] ^ m;
            c[6]  <= c[5];
            c[7]  <= c[6];
            c[8]  <= c[7];
            c[9]  <= c[8];
            c[10] <= c[9];
            c[11] <= c[10];
            c[12] <= c[11] ^ c[15] ^ m;
            c[13] <= c[12];
            c[14] <= c[13];
            c[15] <= c[14];
        end
    end
end

endmodule
