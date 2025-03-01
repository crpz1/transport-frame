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
int digital_clock2_location_x = 690;
int digital_clock2_location_y = 56;
int digital_clock2_size = 16;
String digital_clock2_content = "88:88 pm";

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

    display.setFont(text2_font);
    display.setTextColor(7, 7);    display.setTextSize(1);    display.setCursor(digital_clock2_location_x, digital_clock2_location_y);
    display.print(digital_clock2_content);

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
