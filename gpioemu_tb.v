`timescale 1 ns/10 ps
module gpioemu_tb;

// Deklaracja rejestrow i zmiennych
reg n_reset = 1;
reg [15:0] saddress = 0;
reg srd = 0;
reg swr = 0;
reg [31:0] sdata_in = 0;
reg [31:0] gpio_in = 0;
reg gpio_latch = 0;
reg clk = 0;
integer i;

// Deklaracja wyjsc
output [31:0] gpio_in_s_insp;
output [31:0] gpio_out;

// Inicjalizacja sygnalow debugowania
initial begin
    $dumpfile("gpioemu.vcd");
    $dumpvars(0, gpioemu_tb);
    clk = 0;
end

// Generowanie sygnalu zegarowego
initial begin
    for(i = 0; i < 2500000; i = i + 1)
        #2 clk = ~clk;
end

// Glowna sekwencja testowa
initial begin
    // Resetowanie systemu
    #5 n_reset = 0;
    #5 n_reset = 1;

    // Test dla kazdej wartosci A
    // Test dla A = 1
    #5 sdata_in = 32'd1;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 2
    #5 sdata_in = 32'd2;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 7
    #5 sdata_in = 32'd7;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 10
    #5 sdata_in = 32'd10;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 28
    #5 sdata_in = 32'd28;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 100
    #5 sdata_in = 32'd100;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 255
    #5 sdata_in = 32'd255;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 500
    #5 sdata_in = 32'd500;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 750
    #5 sdata_in = 32'd750;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 850
    #5 sdata_in = 32'd850;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #3000;

    // Test dla A = 1000
    #5 sdata_in = 32'd1000;
    #5 saddress = 16'h224;
    #5 swr = 1;
    #5 swr = 0;
    #40000;
    #30 saddress = 16'h234;
    #5 srd = 1;
    #5 srd = 0;
    #20000;

    $finish;
end

// Deklaracja sygnalow wyjsciowych
wire [31:0] gpio_out_test;
wire [31:0] sdata_out_test;
wire [31:0] gpio_in_s_insp_test;
wire [63:0] is_prime;
wire [63:0] comparison_value;

// Instancja testowanego modulu
gpioemu test(n_reset, saddress, srd, swr, sdata_in, sdata_out_test, gpio_in, gpio_latch, gpio_out, clk, gpio_in_s_insp);

endmodule
