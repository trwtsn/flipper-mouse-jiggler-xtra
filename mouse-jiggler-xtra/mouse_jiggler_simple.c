#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_random.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <input/input.h>

typedef enum {
    ViewIdMenu,
    ViewIdJiggle,
} ViewId;

typedef enum {
    ModeSimple,
    ModeShort,
    ModeShortHorizontal,
    ModeShortVertical,
    ModeLong,
    ModeLongHorizontal,
    ModeLongVertical,
    ModeDVD,
    ModeChaosLite,
    ModeChaos,
    ModeCount,
} JiggleMode;

static const char* mode_names[] = {
    "Simple",
    "Short",
    "Short Horizontal",
    "Short Vertical",
    "Long",
    "Long Horizontal",
    "Long Vertical",
    "DVD",
    "Chaos Lite",
    "Chaos",
};

typedef struct {
    int8_t dx;
    int8_t dy;
} JiggleParams;

static const JiggleParams mode_params[] = {
    [ModeSimple] = {2, 2},
    [ModeShort] = {20, 20},
    [ModeShortHorizontal] = {50, 0},
    [ModeShortVertical] = {0, 50},
    [ModeLong] = {200, 80},
    [ModeLongHorizontal] = {200, 0},
    [ModeLongVertical] = {0, 200},
    [ModeDVD] = {0, 0},
    [ModeChaosLite] = {0, 0},
    [ModeChaos] = {0, 0},
};

typedef struct {
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    View* jiggle_view;
    FuriTimer* timer;
    FuriHalUsbInterface* usb_mode_prev;
    JiggleMode mode;
    bool direction;
    ViewId current_view;
    // DVD bounce state
    int16_t dvd_x;
    int16_t dvd_y;
    int8_t dvd_vx;
    int8_t dvd_vy;
} MouseJigglerApp;

typedef struct {
    JiggleMode mode;
} JiggleViewModel;

static void jiggle_view_draw(Canvas* canvas, void* model) {
    JiggleViewModel* vm = model;
    canvas_clear(canvas);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 0, 10, "Mouse Jiggler Xtra");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 0, 26, "Mode:");
    canvas_draw_str(canvas, 36, 26, mode_names[vm->mode]);

    canvas_draw_str(canvas, 0, 63, "Press [back] for menu");
}

static bool jiggle_view_input(InputEvent* event, void* ctx) {
    UNUSED(ctx);
    UNUSED(event);
    return false;
}

static void jiggle_timer_callback(void* ctx) {
    MouseJigglerApp* app = ctx;

    int8_t dx, dy;

    if(app->mode == ModeDVD) {
        int16_t next_x = app->dvd_x + app->dvd_vx;
        int16_t next_y = app->dvd_y + app->dvd_vy;

        if(next_x <= 0 || next_x >= 720) app->dvd_vx = -app->dvd_vx;
        if(next_y <= 0 || next_y >= 480) app->dvd_vy = -app->dvd_vy;

        app->dvd_x += app->dvd_vx;
        app->dvd_y += app->dvd_vy;

        furi_hal_hid_mouse_move(app->dvd_vx, app->dvd_vy);
        return;
    }

    if(app->mode == ModeChaos) {
        dx = (int8_t)((furi_hal_random_get() % 255) - 127);
        dy = (int8_t)((furi_hal_random_get() % 255) - 127);
    } else if(app->mode == ModeChaosLite) {
        dx = (int8_t)((furi_hal_random_get() % 77) - 38);
        dy = (int8_t)((furi_hal_random_get() % 77) - 38);
    } else {
        JiggleParams p = mode_params[app->mode];
        dx = app->direction ? (int8_t)(-p.dx) : p.dx;
        dy = app->direction ? (int8_t)(-p.dy) : p.dy;
    }

    furi_hal_hid_mouse_move(dx, dy);
    app->direction = !app->direction;
}

static void menu_callback(void* ctx, uint32_t index) {
    MouseJigglerApp* app = ctx;
    app->mode = (JiggleMode)index;
    app->direction = false;

    if(app->mode == ModeDVD) {
        app->dvd_x = 360;
        app->dvd_y = 240;
        app->dvd_vx = 4;
        app->dvd_vy = 3;
    }

    with_view_model(
        app->jiggle_view,
        JiggleViewModel * vm,
        { vm->mode = app->mode; },
        true);

    uint32_t tick_interval;
    if(app->mode == ModeChaos) {
        tick_interval = furi_kernel_get_tick_frequency() / 20;
    } else if(app->mode == ModeChaosLite) {
        tick_interval = furi_kernel_get_tick_frequency() / 6;
    } else if(app->mode == ModeDVD) {
        tick_interval = furi_kernel_get_tick_frequency() / 30;
    } else {
        tick_interval = furi_kernel_get_tick_frequency();
    }
    furi_timer_start(app->timer, tick_interval);
    app->current_view = ViewIdJiggle;
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewIdJiggle);
}

static bool nav_callback(void* ctx) {
    MouseJigglerApp* app = ctx;
    if(app->current_view == ViewIdJiggle) {
        furi_timer_stop(app->timer);
        app->current_view = ViewIdMenu;
        view_dispatcher_switch_to_view(app->view_dispatcher, ViewIdMenu);
        return true;
    }
    return false;
}

static bool custom_event_callback(void* ctx, uint32_t event) {
    UNUSED(ctx);
    UNUSED(event);
    return false;
}

int32_t mouse_jiggler_xtra_app(void* p) {
    UNUSED(p);

    MouseJigglerApp* app = malloc(sizeof(MouseJigglerApp));
    app->direction = false;
    app->mode = ModeSimple;
    app->current_view = ViewIdMenu;

    app->usb_mode_prev = furi_hal_usb_get_config();
    furi_check(furi_hal_usb_set_config(&usb_hid, NULL) == true);

    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, nav_callback);

    app->submenu = submenu_alloc();
    submenu_set_header(app->submenu, "Jiggle Mode");
    for(uint32_t i = 0; i < ModeCount; i++) {
        submenu_add_item(app->submenu, mode_names[i], i, menu_callback, app);
    }
    view_dispatcher_add_view(app->view_dispatcher, ViewIdMenu, submenu_get_view(app->submenu));

    app->jiggle_view = view_alloc();
    view_set_context(app->jiggle_view, app);
    view_allocate_model(app->jiggle_view, ViewModelTypeLocking, sizeof(JiggleViewModel));
    view_set_draw_callback(app->jiggle_view, jiggle_view_draw);
    view_set_input_callback(app->jiggle_view, jiggle_view_input);
    view_dispatcher_add_view(app->view_dispatcher, ViewIdJiggle, app->jiggle_view);

    app->timer = furi_timer_alloc(jiggle_timer_callback, FuriTimerTypePeriodic, app);

    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewIdMenu);
    view_dispatcher_run(app->view_dispatcher);

    furi_timer_stop(app->timer);
    furi_timer_free(app->timer);

    view_dispatcher_remove_view(app->view_dispatcher, ViewIdMenu);
    view_dispatcher_remove_view(app->view_dispatcher, ViewIdJiggle);
    submenu_free(app->submenu);
    view_free(app->jiggle_view);
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close(RECORD_GUI);

    furi_hal_usb_set_config(app->usb_mode_prev, NULL);

    free(app);
    return 0;
}
