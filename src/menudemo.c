/**
 * \file menudemo.c
 * \author Aftersol
 * \date 2026-05-11
 * \brief A simple menu example for libdragon.
 * 
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 * 
 * Credits:
 * - MiaFan2010: You Would Be Here (miafan2010_-_you_would_be_here.xm)
 *   https://modarchive.org/index.php?request=view_by_moduleid&query=172936
 * - madameberry: Public Domain Backgrounds - sunset.png
 *   https://opengameart.org/content/public-domain-backgrounds
 */

#include <libdragon.h>

#include <stdio.h>

bool play_sfx = true;
bool play_music = true;
char main_menu_items [3][256]  = {
    "Start Game",
    "Options",
    "Credits"
};

char options_menu_items [3][256] = {
    "Music - ON",
    "SFX - ON",
    "Back"
};

char credits_menu_items [3][256] = {
    "Programming and SFX by Aftersol",
    "madameberry - sunset.png, MiaFan2010 - you_would_be_here.xm",
    "Back"
};

int main() {
    int menuIndex = 0, menuID = 0;
    xm64player_t music;
    wav64_t bop, bap;
    char *menuText[3] = {
        main_menu_items[0],
        main_menu_items[1],
        main_menu_items[2]
    };
    debug_init_emulog();
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);
    display_init(
        RESOLUTION_320x240,
        DEPTH_16_BPP,
        2,
        GAMMA_NONE,
        FILTERS_DISABLED
    );

    joypad_init();
    rdpq_init();
    audio_init(48000, 3);
    mixer_init(32);

    wav64_open(&bop, "rom:/bop.wav64"); // Load audio assets
    wav64_open(&bap, "rom:/bap.wav64");
    xm64player_open(&music, "rom:/miafan2010_-_you_would_be_here.xm64");
    xm64player_set_loop(&music, true);
    xm64player_play(&music, 0);

    sprite_t* background = sprite_load("rom:/background.sprite"); // Load sprites
    sprite_t* logo = sprite_load("rom:/logo.sprite");

    rdpq_font_t *font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
    rdpq_text_register_font(1, font); // Load and register fonts

    while (1) {
        surface_t* disp;

        char menuTextBuffer[3][256];
        joypad_buttons_t button_port_1;

        while(!(disp = display_try_get())) {;}

        joypad_poll(); // Poll controls for button inputs
        mixer_try_play(); // Required for audio playback

        button_port_1 = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (button_port_1.d_up || button_port_1.c_up || joypad_get_axis_pressed(JOYPAD_PORT_1, JOYPAD_AXIS_STICK_Y) > 0) {
            if (play_sfx) {wav64_play(&bap, 31);}
            menuIndex = (menuIndex - 1 + 3) % 3; // Move up in the menu
        }

        if (button_port_1.d_down || button_port_1.c_down || joypad_get_axis_pressed(JOYPAD_PORT_1, JOYPAD_AXIS_STICK_Y) < 0) {
            if (play_sfx) {wav64_play(&bap, 31);}
            menuIndex = (menuIndex + 1) % 3; // Move down in the menu
        }

        if (button_port_1.b && menuID != 0) {
            if (play_sfx) {wav64_play(&bop, 31);}
            // Back selected
            menuID = 0; // Return to main menu
            menuIndex = 0; // Reset menu index for main menu
            menuText[0] = main_menu_items[0];
            menuText[1] = main_menu_items[1];
            menuText[2] = main_menu_items[2];
        }

        else if (button_port_1.a) {
            switch (menuID) {
                case 0: // Main Menu
                {
                    if (menuIndex == 1) {
                        // Options selected
                        menuID = 1; // Switch to options menu
                        menuIndex = 0; // Reset menu index for options
                        menuText[0] = options_menu_items[0];
                        menuText[1] = options_menu_items[1];
                        menuText[2] = options_menu_items[2];
                    }
                    if (menuIndex == 2) {
                        // Credits selected
                        menuID = 2; // Switch to credits menu
                        menuIndex = 0; // Reset menu index for credits
                        menuText[0] = credits_menu_items[0];
                        menuText[1] = credits_menu_items[1];
                        menuText[2] = credits_menu_items[2];
                    }
                    break;
                }
                case 1: // Options Menu
                    {
                        if (menuIndex == 0) { // Toggle Music
                            sys_hw_memset(menuText[0], 0, 256);
                            if (play_music) { // Silence music; music stills plays silently
                                sprintf(options_menu_items[0], "Music - OFF");
                                xm64player_set_vol(&music, 0.0f);
                                play_music = false;
                            } else { // Turn the music back on
                                sprintf(options_menu_items[0], "Music - ON");
                                xm64player_set_vol(&music, 1.0f);
                                play_music = true;
                            }
                        }
                        if (menuIndex == 1) { // Toggle SFX
                            sys_hw_memset(options_menu_items[1], 0, 256);
                            if (play_sfx) { // Disable, no sound as confirmation that sound is off
                                sprintf(options_menu_items[1], "SFX - OFF");
                                play_sfx = false;
                            } else {
                                sprintf(options_menu_items[1], "SFX - ON");
                                play_sfx = true;
                            }
                        } else if (menuIndex == 2) {
                            // Back selected
                            menuID = 0; // Return to main menu
                            menuIndex = 0; // Reset menu index for main menu
                            menuText[0] = main_menu_items[0];
                            menuText[1] = main_menu_items[1];
                            menuText[2] = main_menu_items[2];
                        }
                        break;
                }
                case 2:
                {
                    if (menuIndex == 2) { // Back selected
                        menuID = 0; // Return to main menu
                        menuIndex = 0; // Reset menu index for main menu
                        menuText[0] = main_menu_items[0];
                        menuText[1] = main_menu_items[1];
                        menuText[2] = main_menu_items[2];
                    }
                }
            }
            if (play_sfx) {wav64_play(&bop, 31);} // Play confirm sound
        }

        rdpq_attach(disp, NULL);
        rdpq_set_mode_copy(true);
        rdpq_sprite_blit(background, 0, 0, NULL); // Draw 2D elements to screen
        rdpq_sprite_blit(logo, (320/2)-(114/2), 32, NULL);

        rdpq_set_mode_standard();
        for (int i = 0; i < 3; i++) { // Write indicator to text buffer
            if (i == menuIndex) {
                sprintf(menuTextBuffer[i], "> %s <", menuText[i]);
            } else {
                sprintf(menuTextBuffer[i], "  %s  ", menuText[i]);
            }
        }
        rdpq_text_printf(&(rdpq_textparms_t) { // Draw menu text to screen
            .width = 320-32,
            .align = ALIGN_CENTER,
            .wrap = WRAP_WORD,
            }, 1, 16, 128, "%s\n%s\n%s\n",
            menuTextBuffer[0], 
            menuTextBuffer[1], 
            menuTextBuffer[2]);

        for (int i = 0; i < 3; i++) // Clear text buffer for next frame
            sys_hw_memset(menuTextBuffer[i], 0, 64);

        rdpq_detach_show(); // Send the contents of the frame to the screen

    }
}
