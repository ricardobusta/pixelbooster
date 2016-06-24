CONFIG += c++11

SOURCES += \
    main.cpp \
    widgets/image_edit_widget.cpp \
    screens/new_image_file_dialog.cpp \
    application/pixel_booster.cpp \
    logic/action_handler.cpp \
    widgets/image_canvas_container.cpp \
    widgets/image_canvas_widget.cpp \
    application/global_options.cpp \
    screens/about_dialog.cpp \
    screens/set_tile_size_dialog.cpp \
    screens/main_window.cpp \
    widgets/color_palette_widget.cpp \
    logic/undo_redo.cpp \
    logic/tool_algorithm.cpp \
    #utils/pb_math.cpp \
    widgets/color_dialog.cpp \
    logic/tool/pencil_tool.cpp \
    logic/tool/flood_fill_tool.cpp \
    logic/tool/line_tool.cpp \
    logic/tool/rectangle_tool.cpp \
    logic/tool/ellipse_tool.cpp \
    logic/tool/selection_tool.cpp \
    logic/tool/zoom_tool.cpp \
    screens/resize_image_dialog.cpp \
    screens/help_dialog.cpp

HEADERS  += \
    widgets/image_edit_widget.h \
    screens/new_image_file_dialog.h \
    application/pixel_booster.h \
    logic/action_handler.h \
    #utils/debug.h \
    resources/translations/international_text.h \
    widgets/image_canvas_container.h \
    widgets/image_canvas_widget.h \
    application/global_options.h \
    screens/about_dialog.h \
    screens/set_tile_size_dialog.h \
    screens/main_window.h \
    widgets/color_palette_widget.h \
    resources/version.h \
    logic/undo_redo.h \
    logic/tool_algorithm.h \
    #utils/pb_math.h \
    widgets/color_dialog.h \
    logic/tool/pencil_tool.h \
    logic/tool/flood_fill_tool.h \
    logic/tool/line_tool.h \
    logic/tool/rectangle_tool.h \
    logic/tool/ellipse_tool.h \
    logic/tool/selection_tool.h \
    logic/tool/zoom_tool.h \
    screens/resize_image_dialog.h \
    screens/help_dialog.h
