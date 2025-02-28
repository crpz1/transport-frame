#include "Arduino.h"
#include "Inkplate.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold24pt7b.h"

extern Inkplate display;

int rect0_a_x = 0;
int rect0_a_y = 0;
int rect0_b_x = 802;
int rect0_b_y = 75;
int rect0_fill = 1;
int rect0_radius = -1;
int rect0_color = 1;

String text2_content = "Time now";
int text2_cursor_x = 701;
int text2_cursor_y = 27;
const GFXfont *text2_font = &FreeSans9pt7b;

int digital_clock2_h = 9;
int digital_clock2_m = 41;
int digital_clock2_location_x = 719;
int digital_clock2_location_y = 40;
int digital_clock2_size = 16;
int digital_clock2_bitmask[] = {119, 48, 93, 121, 58, 107, 111, 49, 127, 59};
int digital_clock2_triangleX[] = {83, 101, 108, 101, 108, 277, 101, 108, 277, 257, 277, 108, 257, 277, 286, 76, 60, 98, 60, 98, 80, 80, 39, 60, 80, 39, 55, 31, 55, 73, 31, 73, 52, 31, 9, 52, 9, 52, 20, 61, 86, 80, 86, 80, 233, 233, 227, 80, 233, 227, 252, 260, 292, 305, 305, 260, 240, 305, 281, 240, 240, 281, 260, 259, 234, 276, 234, 276, 256, 256, 214, 234, 214, 256, 237, 38, 27, 60, 38, 60, 207, 207, 38, 212, 212, 207, 230};
int digital_clock2_triangleY[] = {30, 13, 60, 13, 60, 14, 13, 60, 14, 57, 14, 60, 57, 14, 29, 36, 47, 61, 47, 61, 198, 198, 201, 47, 198, 201, 219, 252, 232, 253, 252, 253, 390, 252, 406, 390, 406, 390, 416, 227, 202, 249, 202, 249, 203, 203, 247, 249, 203, 247, 224, 60, 35, 49, 49, 60, 200, 50, 201, 200, 200, 201, 220, 231, 252, 252, 252, 252, 403, 403, 390, 252, 390, 403, 415, 439, 424, 392, 439, 392, 394, 394, 439, 439, 439, 394, 424};
int digital_clock2_maxX = 310;
int digital_clock2_maxY = 440;

String text4_content = "Next Departures";
int text4_cursor_x = 18;
int text4_cursor_y = 52;
const GFXfont *text4_font = &FreeSansBold24pt7b;

int line0_start_x = 400;
int line0_start_y = 76;
int line0_end_x = 400;
int line0_end_y = 600;
int line0_color = 5;
int line0_thickness = 1;
int line0_gradient = 0;

void mainDraw() {
    if (rect0_radius != -1 && rect0_fill != -1)
       display.fillRoundRect(rect0_a_x, rect0_a_y, rect0_b_x - rect0_a_x, rect0_b_y - rect0_a_y, rect0_radius, rect0_color);
   else if (rect0_radius != -1 && rect0_fill == -1)
       display.drawRoundRect(rect0_a_x, rect0_a_y, rect0_b_x - rect0_a_x, rect0_b_y - rect0_a_y, rect0_radius, rect0_color);
   else if (rect0_radius == -1 && rect0_fill != -1)
       display.fillRect(rect0_a_x, rect0_a_y, rect0_b_x - rect0_a_x, rect0_b_y - rect0_a_y, rect0_color);
   else if (rect0_radius == -1 && rect0_fill == -1)
       display.drawRect(rect0_a_x, rect0_a_y, rect0_b_x - rect0_a_x, rect0_b_y - rect0_a_y, rect0_color);

    display.setFont(text2_font);
    display.setTextColor(7, 7);    display.setTextSize(1);    display.setCursor(text2_cursor_x, text2_cursor_y);
    display.print(text2_content);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < sizeof(digital_clock2_triangleX) / sizeof(digital_clock2_triangleX[0]); j += 3) {
            int temp[4] = {digital_clock2_h / 10 % 10, digital_clock2_h % 10, digital_clock2_m / 10 % 10, digital_clock2_m % 10};
            int b = digital_clock2_bitmask[temp[i]];
            if (b & (1 << ((j - 1) / (3 * 4)))) {
                display.fillTriangle(
                    (int)((float)i * (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * 1.1 + (float)digital_clock2_location_x + (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * (float)digital_clock2_triangleX[j + 0] / (float)digital_clock2_maxX),
                    (int)((float)digital_clock2_location_y + (float)digital_clock2_size * (float)digital_clock2_triangleY[j + 0] / (float)digital_clock2_maxY),

                    (int)((float)i * (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * 1.1 + (float)digital_clock2_location_x + (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * (float)digital_clock2_triangleX[j + 1] / (float)digital_clock2_maxX),
                    (int)((float)digital_clock2_location_y + (float)digital_clock2_size * (float)digital_clock2_triangleY[j + 1] / (float)digital_clock2_maxY),

                    (int)((float)i * (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * 1.1 + (float)digital_clock2_location_x + (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * (float)digital_clock2_triangleX[j + 2] / (float)digital_clock2_maxX),
                    (int)((float)digital_clock2_location_y + (float)digital_clock2_size * (float)digital_clock2_triangleY[j + 2] / (float)digital_clock2_maxY),

                    0);
            }
        }
    }

    int digital_clock2_r = 0.05 * (float)digital_clock2_size;

    display.fillCircle((int)((float)digital_clock2_location_x + 4.0 * (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * 1.075 / 2.0), (int)((float)digital_clock2_location_y + (float)digital_clock2_size * 0.4), digital_clock2_r, 0);
    display.fillCircle((int)((float)digital_clock2_location_x + 4.0 * (float)digital_clock2_maxX / (float)digital_clock2_maxY * (float)digital_clock2_size * 1.075 / 2.0), (int)((float)digital_clock2_location_y + (float)digital_clock2_size * 0.6), digital_clock2_r, 0);
    display.setFont(text4_font);
    display.setTextColor(7, 7);    display.setTextSize(1);    display.setCursor(text4_cursor_x, text4_cursor_y);
    display.print(text4_content);

   if (line0_gradient <= line0_color && line0_thickness == 1)
       display.drawLine(line0_start_x, line0_start_y, line0_end_x, line0_end_y, line0_color);
   else if (line0_gradient <= line0_color && line0_thickness != 1)
       display.drawThickLine(line0_start_x, line0_start_y, line0_end_x, line0_end_y, line0_color, line0_thickness);
   else if (line0_gradient > line0_color && line0_thickness == 1)
       display.drawGradientLine(line0_start_x, line0_start_y, line0_end_x, line0_end_y, line0_color, line0_gradient, 1);
   else if (line0_gradient > line0_color && line0_thickness != 1)
       display.drawGradientLine(line0_start_x, line0_start_y, line0_end_x, line0_end_y, line0_color, line0_gradient, line0_thickness);

}
