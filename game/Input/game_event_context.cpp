#include "game_event_context.h"

#include "buttons.h"

Button* GameEventContext::get_button(int row, int column) const {
    for (Button* button : buttons) {
        if (button->get_row() == row && button->get_column() == column) {
            return button;
        }
    }
    return nullptr;
}
