////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1999-2008 Easics NV.
// This source file may be used and distributed without restriction
// provided that this copyright statement is not removed from the file
// and that any derivative work contains the original copyright notice
// and the associated disclaimer.
//
// THIS SOURCE FILE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
// WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Purpose : synthesizable CRC function
//   * polynomial: x^16 + x^12 + x^5 + 1
//   * data width: 8
//
// Info : tools@easics.be
//        http://www.easics.com
////////////////////////////////////////////////////////////////////////////////
module CRC16_D8;

  // polynomial: x^16 + x^12 + x^5 + 1
  // data width: 8
  // convention: the first serial bit is D[7]
  function [15:0] nextCRC16_D8;

    input [7:0] Data;
    input [15:0] crc;
    reg [7:0] d;
    reg [15:0] c;
    reg [15:0] newcrc;
  begin
    d = Data;
    c = crc;

    newcrc[0] = d[4] ^ d[0] ^ c[8] ^ c[12];
    newcrc[1] = d[5] ^ d[1] ^ c[9] ^ c[13];
    newcrc[2] = d[6] ^ d[2] ^ c[10] ^ c[14];
    newcrc[3] = d[7] ^ d[3] ^ c[11] ^ c[15];
    newcrc[4] = d[4] ^ c[12];
    newcrc[5] = d[5] ^ d[4] ^ d[0] ^ c[8] ^ c[12] ^ c[13];
    newcrc[6] = d[6] ^ d[5] ^ d[1] ^ c[9] ^ c[13] ^ c[14];
    newcrc[7] = d[7] ^ d[6] ^ d[2] ^ c[10] ^ c[14] ^ c[15];
    newcrc[8] = d[7] ^ d[3] ^ c[0] ^ c[11] ^ c[15];
    newcrc[9] = d[4] ^ c[1] ^ c[12];
    newcrc[10] = d[5] ^ c[2] ^ c[13];
    newcrc[11] = d[6] ^ c[3] ^ c[14];
    newcrc[12] = d[7] ^ d[4] ^ d[0] ^ c[4] ^ c[8] ^ c[12] ^ c[15];
    newcrc[13] = d[5] ^ d[1] ^ c[5] ^ c[9] ^ c[13];
    newcrc[14] = d[6] ^ d[2] ^ c[6] ^ c[10] ^ c[14];
    newcrc[15] = d[7] ^ d[3] ^ c[7] ^ c[11] ^ c[15];
    nextCRC16_D8 = newcrc;
  end
  endfunction
endmodule
