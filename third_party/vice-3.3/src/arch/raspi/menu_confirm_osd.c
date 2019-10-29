/*
 * menu_confirm_osd.c
 *
 * Written by
 *  Randy Rossi <randy.rossi@gmail.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "menu_confirm_osd.h"

#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// VICE includes
#include "datasette.h"
#include "resources.h"

// RASPI includes
#include "menu.h"
#include "ui.h"

static void menu_item_changed(struct menu_item *item) {
  switch (item->id) {
  case MENU_CONFIRM_OK:
    // Pass along to main loop as quick func invoke
    circle_emu_quick_func_interrupt(item->value);
    ui_pop_all_and_toggle();
    return;
  case MENU_CONFIRM_CANCEL:
    ui_pop_all_and_toggle();
    return;
  }
}

void show_confirm_osd_menu(int func) {
  // We only show OSD when the emulator is running. (not in the trap)
  if (ui_enabled) {
    ui_dismiss_osd_if_active();
    return;
  }
  struct menu_item *root = ui_push_menu(8, 2);
  root->on_popped_off = glob_osd_popped;

  struct menu_item *child;
  child = ui_menu_add_button(MENU_CONFIRM_OK, root, "OK");
  child->on_value_changed = menu_item_changed;
  child->value = func;

  child = ui_menu_add_button(MENU_CONFIRM_CANCEL, root, "CANCEL");
  child->on_value_changed = menu_item_changed;

  // This will turn on ui rendering from the emuation side which will
  // now see the OSD we just created.
  ui_enable_osd();
}
