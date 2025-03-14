/* verilator lint_off UNUSED */ 
/* verilator lint_off UNDRIVEN */ 
/* verilator lint_off BLKSEQ */
/* verilator lint_off MULTIDRIVEN */ 
/* verilator lint_off COMBDLY */
/* verilator lint_off WIDTH */
module gpioemu(
    n_reset,
    saddress[15:0],
    srd,
    swr,
    sdata_in[31:0],
    sdata_out[31:0],
    gpio_in[31:0],
    gpio_latch,
    gpio_out[31:0],
    clk,
    gpio_in_s_insp[31:0]
);

// Deklaracje portow
input n_reset;
input [15:0] saddress;
input srd;
input swr;
input [31:0] sdata_in;
output [31:0] sdata_out;

input [31:0] gpio_in;
input gpio_latch;

output [31:0] gpio_out;
reg [31:0] gpio_in_s;
reg [31:0] gpio_out_s;
reg [31:0] sdata_out_s;
reg [2:0] state;

output [31:0] gpio_in_s_insp;

input clk;

// Wewnetrzne rejestry do obliczania liczby pierwszej
reg [31:0] A; // Numer N-tej liczby pierwszej do znalezienia
reg [31:0] S; // Aktualny status maszyny stanow (0: inicjalizacja, 1: liczenie, 2: obliczone)
reg [31:0] W; // Wynikowa liczba pierwsza

reg [31:0] counter; // Licznik dzielnikow biezacej liczby

reg [31:0] prime_number; // Znaleziona liczba pierwsza
reg [31:0] current_number; // Aktualna liczba do sprawdzenia
reg [31:0] prime_counter; // Licznik znalezionych liczb pierwszych

reg is_prime; // Zmienna pomocnicza do sprawdzania pierwszosci
integer i; // Zmienna pomocnicza

// Zerowanie zmiennych przy zboczu opadajacym sygnalu n_reset
always @(negedge n_reset) begin
    A <= 0;
    S <= 0;
    W <= 0;
    counter <= 0;
    prime_number <= 0;
    current_number <= 0;
    prime_counter <= 0;

    gpio_in_s <= 0;
    gpio_out_s <= 0;
    sdata_out_s <= 0;
    state <= 3;
end

// Odczytanie danych przy zboczu narastajacym sygnalu swr
always @(posedge swr) begin
    if (saddress == 16'h224) begin
        A <= sdata_in[31:0];
        S <= 0;
        W <= 0;
        state <= 0;
        current_number <= 2;
        prime_counter <= 0;
        prime_number <= 0;
    end
end

// Wypisanie danych na sdata_out przy zboczu narastajacym srd
always @(posedge srd) begin
    case (saddress)
        16'h234: sdata_out_s <= W[31:0]; // Adres wyniku
        16'h23C: sdata_out_s <= S[31:0]; // Adres statusu
        default: sdata_out_s <= 0;
    endcase
end

// Maszyna stanow do obliczania N-tej liczby pierwszej
always @(posedge clk) begin
    case (state)
    0: begin // Stan inicjalizacji
        S <= 1; // Ustawienie statusu na "liczenie"
        if (A == 1) begin // Jesli A = 1, bezposrednio ustaw pierwsza liczbe pierwsza
            prime_counter <= 1;
            prime_number <= 2;
            state <= 2;
        end else begin
            current_number <= 2;
            prime_counter <= 0;
            state <= 1;
        end
    end

    1: begin // Stan liczenia
        is_prime = 1;
        for (i = 2; (i * i <= current_number) && (is_prime == 1); i = i + 1) begin
            if (current_number % i == 0) begin // Sprawdzenie, czy liczba jest pierwsza
                is_prime = 0;
            end
        end
        if (is_prime) begin // Aktualizacja licznika i ostatniej znalezionej liczby pierwszej
            prime_counter <= prime_counter + 1;
            prime_number <= current_number;
        end
        if (A == prime_counter) begin // Sprawdzenie, czy znaleziono N-ta liczbe pierwsza
            state <= 2;
        end else begin
            current_number <= current_number + 1; // Przejdz do nastepnej liczby
        end
    end

    2: begin // Stan zakonczenia obliczen
        W <= prime_number; // Zapisanie wyniku
        gpio_out_s <= gpio_out_s + prime_counter; // Aktualizacja wyjscia GPIO
        S <= 2; // Ustawienie statusu na "obliczone"
        state <= 3; // Przejscie do stanu oczekiwania
    end
    endcase
end

assign gpio_out = {16'h0, gpio_out_s[15:0]};
assign gpio_in_s_insp = gpio_in_s;
assign sdata_out = sdata_out_s;

endmodule
