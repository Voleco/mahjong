#pragma once

// #include "sim_logplay.hpp"
// #include "canvas_gui.hpp"
// #include "handler/mouse_record.hpp"
// #include "pesudo/pudumap.hpp"
// #include "util/gui_filelist.hpp"
// #include "util/json_struct.hpp"
// #include "gui_items.hpp"
// #include "gui_progress_bar.hpp"
// #include <fmt/format.h>

// // Std Libraries
// #include <memory>
// #include <optional>
// #include <random>

// namespace simu::logplay {

// using canvas_ns::CanvasData;
// using canvas_ns::CoordSys;
// using canvas_ns::DragZoomHandler;
// using canvas_ns::MouseRecorderHandler;
// using canvas_ns::CanvasImage;

// using milliseconds = std::chrono::milliseconds;

// using GUIRobot = util::gui_robot::RobotOption;
// using GUIRobotData = util::gui_robot::RobotData;
// using GUIItemType = util::gui_items::ItemType;
// using GUIItemConfig = util::gui_items::ItemConfig;
// using ColorList = std::vector<sf::Color>;
// using ProgressBar = util::progress_bar::ProgressBar;
// using GUICellmap = newsch::maptool::gui::GUICellmapBasic;

// TOOL_NLOH_SCH_JSON_STRUCT(GUIConfig,
//     bool, show_cells, false,
//     bool, show_topo, true,
//     bool, show_traj, true,
//     bool, show_goal, true,
//     bool, show_sources, true,
//     bool, show_background, true,
//     bool, only_move_details, true,
//     bool, label_source, true,
//     bool, label_node, false,
//     bool, show_dual_track, true,
//     bool, highligh_stuck, true,
//     bool, show_taskinfo, false,
//     bool, show_loopinfo, false,
//     bool, show_full_name, false,
//     GUIRobot, robot_config, {},
//     GUICellmap, cellmap_config, {},
//     int, robot_alpha, 150,
//     float, robot_radius, 0.3f
// );



// struct AppData;
// class AppPlayControl: public canvas_ns::CanvasHandlerBase {
// public:
//     AppPlayControl(AppData* app, canvas_ns::CanvasData* canvas, canvas_ns::CanvasHandlerBase* parent = nullptr)
//         : canvas_ns::CanvasHandlerBase(canvas, parent), m_app(app) {}

// protected:
//     bool impl_onKeyPressed(const sf::Event& e);
// private:
//     AppData* m_app = nullptr;
// };

// struct AppData {
//     AppData(SimLogPlayOption ext_option, CanvasData& canvas, MapItemExport* map_service);
//     ~AppData() {}

//     void update_data(double dt);
//     void show_imgui_window();
//     void show_imgui_basic_info();
//     void show_imgui_sim_config();
//     void show_imgui_robot_config();
//     void show_imgui_task_info();
//     void show_imgui_loop_info();

//     void draw_canvas();
//     void draw_items();
//     void draw_robots();
//     void draw_progress_bar();

//     void on_loadmap(const std::string& fname);
//     void on_loadlog(const std::string& fname);
//     void on_loadlog();

//     void change_speed(int diff_level);

//     SimLogPlayOption app_option;
//     CanvasData& canvas;
//     MapItemExport* current_map;
//     DragZoomHandler drag_zoom;
//     MouseRecorderHandler mouse_measure;
//     AppPlayControl play_control;
//     ProgressBar progress;

//     CanvasImage image_data;
//     newsch::maptool::CellGraph cellgraph;
//     newsch::maptool::WorkingGraphData working_graph;

//     GUIConfig gui_config;
//     std::map<int, GUIRobot> gui_robots;
//     std::vector<GUIItemConfig> gui_item_configs;
    
//     SimuData log_data;

//     util::FolderContent log_folder;
//     util::FolderContent map_folder;

//     int speed_level;
//     std::vector<double> speed_faster;
//     std::vector<double> speed_slower;
// };

// } // namespace simu::logplay


// int run_logplay_gui(SimLogPlayOption option, bool init_log);