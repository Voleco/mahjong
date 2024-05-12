// #include "sim_logplay_gui.hpp"
// #include "policy_robot.hpp"
// #include "policy_items.hpp"

// #include "implot.h"

// #include <yaml-cpp/yaml.h>
// #include "tinyfiledialogs.h"
// #include "../maptool/gui/gui_view_sfml.hpp"
// #include "util/gui_json_table.hpp"
// #include "taskinfo/serialize.hpp"
// namespace simu::logplay {


// AppData::AppData(SimLogPlayOption ext_option, 
//     CanvasData& canvas, MapItemExport* map_service)
//     : app_option(ext_option),
//       canvas(canvas), 
//       current_map(map_service),
//       drag_zoom(&canvas), 
//       mouse_measure(&canvas),
//       play_control(this, &canvas),
//       progress(&canvas)
// {
//     canvas.handler = &play_control;
//     play_control.setParent(&progress);
//     progress.setParent(&mouse_measure);
//     mouse_measure.setParent(&drag_zoom);
    
//     if (app_option.map_folder.empty()) {
//         app_option.map_folder = "temp_map";
//     }
//     progress.text_left = "00:00:00 000";
//     progress.text_right = "00:00:00 000";
//     progress.on_progress_change = [&](float ratio, bool changing) {
//         log_data.config.paused = true; // always pause update
//         if (log_data.records.empty())
//             return ;
        
//         double t0 = log_data.records.front().time.to_sec();
//         double t1 = log_data.records.back().time.to_sec();
//         double tnow = (1 - ratio)*t0 + ratio*t1;
//         log_data.record_time = log_data.project_time(tnow);
//         log_data.update_state();

//         auto temp = Time::from_sec(log_data.record_time);
//         progress.text_center = fmt::format("{:02}:{:02}:{:02} {:03d}", 
//             temp.hour, temp.min, temp.sec, temp.msec);
//     };

//     log_folder.use_tiny_fd = true;
//     log_folder.load(app_option.log_folder, ".log");
//     // log_folder.load("/home/scs/code/apk/0527/test2", ".log");
//     log_folder.btn_confirm = [&](const std::string& fname) {
//         namespace fs = std::experimental::filesystem;
//         std::string name = fs::path(fname).filename();

//         on_loadlog(fname);
        
        
//         return fmt::format("Loaded {}", name);
//     };

//     map_folder.use_tiny_fd = true;
//     map_folder.load("./", ".pdmap");
//     // map_folder.load("/home/scs/code/apk/0527/", ".pdmap");
//     map_folder.btn_confirm = [&](const std::string& fname) {
//         namespace fs = std::experimental::filesystem;
//         std::string name = fs::path(fname).filename();

//         on_loadmap(fname);
//         return fmt::format("Loaded {}", name);
//     };
//     speed_level = 0;
//     speed_faster = {1.0, 2.0, 3.0, 5.0, 10.0, 20.0, 30.0, 50.0};
//     speed_slower = {1.0, 0.5, 0.3, 0.2, 0.1, 0.05, 0.02};

//     gui_item_configs.clear();
//     gui_item_configs.resize(GuiItemType::NumGuiItems);

//     gui_item_configs[GuiItemType::Pose].show = true;
//     gui_item_configs[GuiItemType::GlobalPath].show = true;
//     gui_item_configs[GuiItemType::AstarPath].show = true;
//     gui_item_configs[GuiItemType::OptPath].show = true;
//     gui_item_configs[GuiItemType::ForwardPath].show = true;
//     gui_item_configs[GuiItemType::LocalMap].show = true;

//     gui_item_configs[GuiItemType::LaserMap].show = false;
//     gui_item_configs[GuiItemType::AuxLaserMap].show = false;
//     gui_item_configs[GuiItemType::BackLaserMap].show = false;
//     gui_item_configs[GuiItemType::RGBDMap].show = false;
//     gui_item_configs[GuiItemType::LeftRGBDMap].show = false;
//     gui_item_configs[GuiItemType::RightRGBDMap].show = false;
//     gui_item_configs[GuiItemType::DownRGBDMap].show = false;
//     gui_item_configs[GuiItemType::StaticMap].show = false;
//     gui_item_configs[GuiItemType::DynamicObMap].show = false;
// }


// bool load_background(CanvasImage&, const std::string&);

// bool AppPlayControl::impl_onKeyPressed(const sf::Event& e) {
//     if (!m_app) return false;

//     if (e.key.code == sf::Keyboard::Space) {
//         bool pause = m_app->log_data.config.paused;
//         m_app->log_data.config.paused = !pause;
//         return true;
//     }
//     if (e.key.code == sf::Keyboard::Right) {
//         if (e.key.control)
//             m_app->log_data.move_time(5);
//         else
//             m_app->log_data.move_time(30);
//         return true;
//     }
//     if (e.key.code == sf::Keyboard::Left) {
//         if (e.key.control)
//             m_app->log_data.move_time(-5);
//         else
//             m_app->log_data.move_time(-30);
//         return true;
//     }
//     if (e.key.code == sf::Keyboard::Up) {
//         m_app->change_speed(1);
//         return true;
//     }
//     if (e.key.code == sf::Keyboard::Down) {
//         m_app->change_speed(-1);
//         return true;
//     }
//     if (e.key.code == sf::Keyboard::PageUp) {
//         m_app->log_folder.select_change(-1);
//         m_app->on_loadlog();
//         fmt::print("Load prev log {}\n", m_app->log_folder.selected_file());
//         return true;
//     }
//     if (e.key.code == sf::Keyboard::PageDown) {
//         m_app->log_folder.select_change(1);
//         m_app->on_loadlog();
//         fmt::print("Load next log {}\n", m_app->log_folder.selected_file());
//         return true;
//     }
//     if (e.key.code == sf::Keyboard::Home) {
//         m_app->log_folder.select_reset(0);
//         m_app->on_loadlog();
//         fmt::print("Load first log {}\n", m_app->log_folder.selected_file());
//         return true;
//     }
//     return false;
// }

// } // namespace simu::logplay

// #include "CLI11.hpp"
// #include "sim_logplay.hpp"
// #include "canvas_gui.hpp"

// int run_logplay_gui(SimLogPlayOption option, bool init_log) {
//     using namespace simu;
//     using namespace simu::logplay;

//     auto& map_folder = option.map_folder;

//     // Initilization
//     auto font_dir = std::string(SIM_RESOURCE_DIR) + "/font/";
    
//     mircore::PuduMap pdmap;
//     MapItemExport map_service;
//     MapItemExport* ptr_map = nullptr;
//     if (!map_folder.empty()) {
//         pdmap = mircore::PuduMap::from_folder(map_folder);
//         if (!map_service.init(pdmap)) {
//             puts("Error, parse map error");
//             return -1;
//         }
//     }
//     ptr_map = &map_service;

//     std::string yahei_file = font_dir + "MSYHMONO.ttf";
//     std::string cjk_file = "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc";
//     CanvasData canvas(1200, 800, "SimLog Player", cjk_file);
//     if (!ImGui::SFML::Init(canvas.window)) {
//         fmt::print("ImGui::SFML initialization failed");
//         exit(-1);
//     }
//     ImPlot::CreateContext();

//     ImFontConfig config;
//     config.MergeMode = true;

//     ImGuiIO& io = ImGui::GetIO();
//     io.Fonts->Clear();
//     io.Fonts->AddFontFromFileTTF(
//         yahei_file.c_str(), 16.0f, nullptr,
//         io.Fonts->GetGlyphRangesDefault());
//     io.Fonts->AddFontFromFileTTF(
//         yahei_file.c_str(), 16.0f, &config,
//         io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
//     io.Fonts->AddFontFromFileTTF(
//         cjk_file.c_str(), 16.0f, &config,
//         io.Fonts->GetGlyphRangesJapanese());
//     io.Fonts->AddFontFromFileTTF(
//         cjk_file.c_str(), 16.0f, &config,
//         io.Fonts->GetGlyphRangesKorean());
//     io.Fonts->Build();
//     if (!ImGui::SFML::UpdateFontTexture())
//         fmt::print("Warning: failed to load font texture (imgui)\n");

//     AppData app(option, canvas, ptr_map);
//     app.log_data.load(option.logfile);
//     app.log_data.config.paused = true;

//     if (!option.map_folder.empty())
//         load_background(app.image_data, option.map_folder);

//     namespace fs = std::experimental::filesystem;

//     auto& window = canvas.window;
//     auto& coord = canvas.coord;
//     sf::Clock deltaClock;
//     int nnn = 0;

//     milliseconds dt_gui(30);
//     auto clk = std::chrono::steady_clock();
//     auto tnext = clk.now() + dt_gui;

//     fmt::print("\n---- Manager: GUI running ----\n");
//     while (window.isOpen()) {
//         // Event handling
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             ImGui::SFML::ProcessEvent(window, event);
//             canvas.handleEvent(event);
//         }
//         ImGui::SFML::Update(window, deltaClock.restart());

//         bool mouse_invalid = 
//             ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) ||
//             ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
        
//         canvas.enabled_events.enable_mouse(!mouse_invalid);

//         // app.handle_state_update(); // will lock
//         app.update_data(dt_gui.count() / 1000.0);
//         app.show_imgui_window();  // This is also event processing
//         canvas_ns::show_mouse_mesurement(canvas, app.mouse_measure);
        
//         // Drawing
//         window.clear(canvas.color_of<canvas_ns::ItemClass::Backgroud>());

//         app.draw_canvas();
//         app.draw_items();
//         app.draw_robots();
//         app.draw_progress_bar();

//         ImGui::SFML::Render(window);
//         window.display();

//         if (clk.now() < tnext) {
//             std::this_thread::sleep_until(tnext);
//         }
//         tnext += dt_gui;
//     }

//     ImPlot::DestroyContext();
//     return 0;
// }

// namespace simu::logplay {

// void AppData::update_data(double dt_gui) {
//     if (log_data.records.empty())
//         return ;

//     if (!log_data.config.paused) {
//         double dt = dt_gui * log_data.config.speed_ratio;
//         log_data.record_time = log_data.project_time(log_data.record_time + dt);
//         log_data.update_state();

//         double t0 = log_data.records.front().time.to_sec();
//         double t1 = log_data.records.back().time.to_sec();
//         if (t1 != t0) {
//             double r = (log_data.record_time - t0) / (t1 - t0);
//             progress.progress = (float)r;
//         }

//         auto tnow = Time::from_sec(log_data.record_time);
//         progress.text_center = fmt::format("{:02}:{:02}:{:02} {:03d}", 
//             tnow.hour, tnow.min, tnow.sec, tnow.msec);
//     }
    
//     for (auto& one: log_data.robot) {
//         gui_robots[one.first]; // create GUIRobot if not existed
//     }
// }


// void show_robot_options(AppData& app, bool GUIRobot::* ptr, const char* opt_name) {
//     auto& robots = app.log_data.robot;

//     if (ImGui::BeginTabItem(opt_name)) {
//         for (auto& one: app.gui_robots) {
//             ImGui::PushID(one.first);

//             auto it = robots.find(one.first);
//             if (it == robots.end()) {
//                 ImGui::PopID();
//                 continue;
//             }                

//             ImGui::Text("R%02d:%s", one.first, it->second.name.c_str());
//             ImGui::SameLine();

//             int select = one.second.*ptr;  // bool -> int
//             ImGui::RadioButton("Off", &select, 0); ImGui::SameLine();
//             ImGui::RadioButton("On", &select, 1);
//             one.second.*ptr = select == 1;

//             ImGui::PopID();
//         }
//         if (ImGui::Button("ShowAll")) {
//             for (auto& one: app.gui_robots)
//                 one.second.*ptr = true;
//         }
//         ImGui::SameLine();
//         if (ImGui::Button("HideAll")) {
//             for (auto& one: app.gui_robots)
//                 one.second.*ptr = false;
//         }
//         ImGui::EndTabItem();
//     }
// }

// static std::string extract_to_temp(const std::string& map_name, const std::string& dest_folder) {
//     if (dest_folder.empty())
//         throw std::invalid_argument("dest folder is empty");

//     auto mkdir = fmt::format("mkdir -p {}", dest_folder);
//     auto clear = fmt::format("rm {}/*", dest_folder);
//     auto decompress = fmt::format("unzip \"{}\" -d {}", map_name, dest_folder);
//     if (system(mkdir.c_str())== -1) puts("Warning: mkdir failed");
//     if (system(clear.c_str()) == -1) puts("Warning: rm failed");
//     if (system(decompress.c_str()) == -1) puts("Warning: unzip failed");
//     return dest_folder;
// }

// bool load_background(CanvasImage& data, const std::string& map_folder);

// void AppData::on_loadlog(const std::string& fname) {
//     log_data.load(fname);
//     gui_robots.clear();

//     if (log_data.records.empty()) {
//         progress.text_left = "00:00:00 000";
//         progress.text_right = "00:00:00 000";
//         progress.text_center = "";
//         progress.progress = 0.5f;
//     }
//     else {
//         auto& t0 = log_data.records.front().time;
//         auto& t1 = log_data.records.back().time;
//         progress.text_left = fmt::format("{:02}:{:02}:{:02} {:03d}", t0.hour, t0.min, t0.sec, t0.msec);
//         progress.text_right = fmt::format("{:02}:{:02}:{:02} {:03d}", t1.hour, t1.min, t1.sec, t1.msec);
//         progress.text_center = progress.text_left;
//         progress.progress = 0.0f;
//     }
// }
// void AppData::on_loadlog() {
//     if (log_folder.num_loaded() == 0) return;

//     on_loadlog(log_folder.selected_file());
// }

// std::string extract_to_temp(const std::string& map_name, const std::string& dest_folder);
// void AppData::on_loadmap(const std::string& fname) {
//     namespace fs = std::experimental::filesystem;
//     printf("--- Loadmap %s ---\n", fname.c_str());
//     printf("--- extract %s ---\n", fs::absolute(app_option.map_folder).string().c_str());

//     extract_to_temp(fname, app_option.map_folder);
//     mircore::PuduMap pdmap =
//         mircore::PuduMap::from_folder(app_option.map_folder);

//     current_map->clear();
//     current_map->init(pdmap);
//     load_background(image_data, app_option.map_folder);

//     printf("--- map md5 %s ---\n", current_map->map_md5.c_str());

//     cellgraph.clear();
//     working_graph.clear();
//     auto cellmap_file = fs::path(app_option.map_folder) / fs::path("cellmap.json");
//     if (fs::exists(cellmap_file)) {
//         bool good = newsch::maptool::try_load_cellgraph(cellmap_file.string(), cellgraph);
//         printf("--- loaded cellgraph from %s\n", cellmap_file.c_str());

//         working_graph.initialize(cellgraph);
//     }
// }

// void AppData::show_imgui_basic_info() {
//     namespace fs = std::experimental::filesystem;
//     if (ImGui::CollapsingHeader("Load Log/Map")) {
//         ImGui::BeginTabBar("Bar_LogMap");
//         if (ImGui::BeginTabItem("Log")) {
//             log_folder.show(false);
//             if (ImGui::Button("Load Log")) {
//                 const char* filter[1] = { "*.log" };
//                 char* fname = tinyfd_openFileDialog(
//                     "打开日志", 
//                     log_folder.selected_file().c_str(),
//                     1, filter, nullptr, 0);
//                 if (fname != nullptr && std::strlen(fname) > 0) {
//                     log_folder.load(fs::path(fname).remove_filename());
//                     on_loadlog(fname);
//                 }
//             }
//             ImGui::SameLine();
//             if (ImGui::Button("Use Map Folder")) {
//                 log_folder.load(map_folder.current_folder());
//             }
//             ImGui::EndTabItem();
//         }
//         if (ImGui::BeginTabItem("Map")) {
//             map_folder.show(false);
//             if (ImGui::Button("Load Map")) {
//                 const char* filter[1] = { "*.pdmap" };
//                 char* fname = tinyfd_openFileDialog(
//                     "打开地图", 
//                     map_folder.selected_file().c_str(),
//                     1, filter, nullptr, 0);
//                 if (fname != nullptr && std::strlen(fname) > 0) {
//                     map_folder.load(fs::path(fname).remove_filename());
//                     on_loadmap(fname);
//                 }
//             }
//             ImGui::SameLine();
//             if (ImGui::Button("Use Log Folder")) {
//                 map_folder.load(log_folder.current_folder());
//             }
//             ImGui::EndTabItem();
//         }
//         ImGui::EndTabBar();
//     }

//     ImGui::Text("-------------");
//     auto px = sf::Mouse::getPosition(canvas.window);
//     auto xy_raw = canvas.window.mapPixelToCoords(px);
//     auto xy = canvas.coord.toCoord(xy_raw);
//     ImGui::Text("Mouse: %.3f, %.3f", xy.x, xy.y);

//     ImGui::Text("-------------");

//     if (ImGui::Button("Pause##Sim")) {
//         log_data.config.paused = true;
//     }
//     ImGui::SameLine();
//     if (ImGui::Button("Resume##Sim")) {
//         log_data.config.paused = false;
//     }
//     if (log_data.config.paused) {
//         ImGui::SameLine();
//         ImGui::Text("Paused");
//     }
    
// }


// void AppData::change_speed(int diff_level) {
//     speed_level += diff_level;
//     if (speed_level >= 0) {
//         speed_level = std::min(speed_level, (int)speed_faster.size() - 1);
//         log_data.config.speed_ratio = speed_faster[speed_level];
//     }
//     else {
//         speed_level = std::max(speed_level, -(int)speed_slower.size() + 1);
//         log_data.config.speed_ratio = speed_slower[-speed_level];
//     }
// }

// void AppData::show_imgui_sim_config() {
//     ImGui::BeginTabBar("CanvasConfigBar");
//     if (ImGui::BeginTabItem("SimTime")) {
//         double ratio = log_data.config.speed_ratio;
//         ImGui::Text("Speed %6.2f", ratio);

//         ImGui::SameLine();
//         if (ImGui::Button("+##Speed")) {
//             change_speed(1);
//         }
//         ImGui::SameLine();
//         if (ImGui::Button("-##Speed")) {
//             change_speed(-1);
//         }
//         ImGui::SameLine();
//         if (ImGui::Button("Reset##Speed")) {
//             speed_level = 0;
//             log_data.config.speed_ratio = 1.0;
//         }

//         int move_time = 0;
//         if (ImGui::Button("<< ##Time")) {
//             log_data.move_time(-60);
//         }
//         ImGui::SameLine();
//         if (ImGui::Button(" < ##Time")) {
//             log_data.move_time(-30);
//         }
//         ImGui::SameLine();
//         if (ImGui::Button(" > ##Time")) {
//             log_data.move_time(30);
//         }
//         ImGui::SameLine();
//         if (ImGui::Button(" >>##Time")) {
//             log_data.move_time(60);
//         }
//         ImGui::SameLine();
//         if (ImGui::Button("Replay")) {
//             log_data.set_time_ratio(0.0);
//         }

//         if (!log_data.records.empty()) {
//             auto tm = log_data.curr_time();
//             auto tl = log_data.records.back().time;
//             ImGui::Text("Time: %02d:%02d:%02d %03d -- %02d:%02d:%02d %03d", 
//                 tm.hour, tm.min, tm.sec, tm.msec, 
//                 tl.hour, tl.min, tl.sec, tl.msec
//             );
//         }
//         ImGui::EndTabItem();
//     }
//     if (ImGui::BeginTabItem("SubWindow")) {
//         ImGui::Checkbox("taskinfo", &gui_config.show_taskinfo);
//         ImGui::Checkbox("loopinfo", &gui_config.show_loopinfo);
//         ImGui::EndTabItem();
//     }
//     ImGui::EndTabBar();
// }

// void AppData::show_imgui_robot_config() {
//     ImGui::BeginTabBar("RobotOptions");

//     if (ImGui::BeginTabItem("Draw##RobotOptions")) {
//         ImGui::BeginTabBar("DrawOptionBar");
//         if (ImGui::BeginTabItem("Canvas##Drawings")) {
//             ImGui::Checkbox("image", &gui_config.show_background); ImGui::SameLine();
//             ImGui::Checkbox("topo", &gui_config.show_topo); ImGui::SameLine();
//             ImGui::Checkbox("source", &gui_config.show_sources);

//             ImGui::Checkbox("label_src", &gui_config.label_source); ImGui::SameLine();
//             ImGui::Checkbox("label_node", &gui_config.label_node);

//             if (!cellgraph.empty()) {
//                 ImGui::Checkbox("cells", &gui_config.show_cells); ImGui::SameLine();
//                 ImGui::Checkbox("cell_edge", &gui_config.cellmap_config.show_edge);

//                 ImGui::Checkbox("cell_label", &gui_config.cellmap_config.label_cell); ImGui::SameLine();
//                 ImGui::Checkbox("edge_label", &gui_config.cellmap_config.label_edge); ImGui::SameLine();

//                 using LabelStype = newsch::maptool::gui::CellmapLabelStyle;
//                 LabelStype curr = LabelStype::_from_integral(gui_config.cellmap_config.label_style);
//                 LabelStype next = curr;

//                 if (ImGui::Button("style##cell_label"))
//                     ImGui::OpenPopup("label_style_popup");
//                 ImGui::SameLine();
//                 ImGui::TextUnformatted(curr._to_string());
//                 if (ImGui::BeginPopup("label_style_popup")) {
//                     ImGui::Text("%s", curr._to_string());
//                     ImGui::Separator();
//                     for (int index = 0; index < (int)LabelStype::_values().size(); ++index) {
//                         if (ImGui::Selectable(LabelStype::_values()[index]._to_string()))
//                             next = LabelStype::_values()[index];
//                     }
//                     ImGui::EndPopup();
//                 }
//                 if (curr != next) {
//                     gui_config.cellmap_config.label_style = next._to_integral();
//                 }
//             }

//             bool for_all = !gui_config.only_move_details;
//             ImGui::Checkbox("details_for_all", &for_all); gui_config.only_move_details = !for_all;
//             ImGui::Checkbox("highlight_stuck", &gui_config.highligh_stuck);
//             ImGui::Checkbox("full_id", &gui_config.show_full_name);

//             ImGui::Checkbox("sch_robot", &gui_config.robot_config.show_base);
//             ImGui::SetNextItemWidth(150);
//             ImGui::DragInt("robot_alpha", &gui_config.robot_alpha, 1.0, 0, 255);

//             ImGui::SetNextItemWidth(120);
//             ImGui::InputFloat("robot_radius", &gui_config.robot_radius);

//             int count = 0;
//             for (int i = 0; i < (int)gui_item_configs.size(); ++i) {
//                 GUIItemType type = GUIItemType::_from_integral(i);
//                 if (log_data.num_of_types(type) > 0) {
//                     auto& config = gui_item_configs[i];
//                     ImGui::Checkbox(type._to_string(), &config.show);
//                     ++count;
//                     if (count % 2 != 0)
//                         ImGui::SameLine();
//                 }
//             }

//             ImGui::EndTabItem();
//         }
//         // show_robot_options(*this, &GUIRobot::show_path, "Path");
//         show_robot_options(*this, &GUIRobot::show_goal, "Goal");
//         if (log_data.has_forward_path()) {
//             show_robot_options(*this, &GUIRobot::show_forward, "FwdPath");
//         }
        
//         ImGui::EndTabBar();

//         ImGui::EndTabItem();  
//     }
//     ImGui::EndTabBar();  // TabBar RobotOptions
// }

// void AppData::show_imgui_window() { 
//     // Command Window
//     ImGui::Begin("CommandCenter");

//     show_imgui_basic_info();
//     ImGui::Separator();
    
//     show_imgui_sim_config();
//     ImGui::Separator();

//     show_imgui_robot_config();

//     if (gui_config.show_taskinfo) {
//         show_imgui_task_info();
//     }
//     if (gui_config.show_loopinfo) {
//         show_imgui_loop_info();
//     }

//     ImGui::End();
// }

// int impl_time_format(double value, char* buff, int size, void* data) {
//     const VelocityData* vel = reinterpret_cast<VelocityData*>(data);
//     int t = vel->time_zero.to_int_ms() + static_cast<int>(std::round(value * 1000));
//     auto s = Time::from_ms(t);
//     if (s.msec == 0) {
//         if (value == 0)
//             return snprintf(buff,size,"%02d:%02d:%02d", s.hour, s.min, s.sec);
//         return snprintf(buff,size,"%02d:%02d", s.min, s.sec);
//     }
//     if (value == 0)
//         return snprintf(buff,size,"%02d:%02d:%02d %03d", s.hour, s.min, s.sec, s.msec);
//     return snprintf(buff,size,"%02d:%02d %03d", s.min, s.sec, s.msec);
// }

// void AppData::show_imgui_loop_info() {
//     ImGui::Begin("Agent's loop info");
//     static bool show_v = true;
//     static bool show_w = true;
//     static bool show_av = true;
//     static bool show_aw = true;
//     ImGui::Checkbox("Show V", &show_v); ImGui::SameLine();
//     ImGui::Checkbox("Show W", &show_w);
//     ImGui::Checkbox("Show aV", &show_av); ImGui::SameLine();
//     ImGui::Checkbox("Show aW", &show_aw);

//     auto& vel = log_data.velocity_info;
//     static float history = 10.0f;
//     ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

//     static double vticks[] = {-0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4};
//     if (!vel.empty() && show_v && ImPlot::BeginPlot("v(t)")) {
//         float t = vel.t.back();
//         ImPlot::SetupAxes(nullptr, nullptr, 0, 0);
//         ImPlot::SetupAxisLimits(ImAxis_X1, vel.time_now - history, vel.time_now + history, ImGuiCond_Always);
//         ImPlot::SetupAxisLimits(ImAxis_Y1, -0.2,1.5);
        
//         ImPlot::SetupAxisFormat(ImAxis_X1, impl_time_format, (void*)(&vel));
//         ImPlot::SetupAxisTicks(ImAxis_Y1, vticks, 9, nullptr, false);

//         ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
//         double t0 = vel.time_now;
//         ImPlot::PlotInfLines("now", &t0, 1);

//         ImPlot::PlotLine("v", vel.t.data(), vel.v.data(), vel.v.size(), 0, 0);
//         ImPlot::PlotLine("uv", vel.ut.data(), vel.uv.data(), vel.uv.size(), 0, 0);
//         ImPlot::PlotLine("vlim", vel.tl.data(), vel.vlim.data(), vel.vlim.size(), 0, 0);
//         ImPlot::EndPlot();
//     }
//     if (!vel.empty() && show_av && ImPlot::BeginPlot("a_v(t)")) {
//         float t = vel.t.back();
//         ImPlot::SetupAxes(nullptr, nullptr, 0, 0);
//         ImPlot::SetupAxisLimits(ImAxis_X1, vel.time_now - history, vel.time_now + history, ImGuiCond_Always);
//         ImPlot::SetupAxisLimits(ImAxis_Y1, -1.0, 1.0);
        
//         ImPlot::SetupAxisFormat(ImAxis_X1, impl_time_format, (void*)(&vel));
//         ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
//         double t0 = vel.time_now;
//         ImPlot::PlotInfLines("now", &t0, 1);

//         ImPlot::PlotLine("uv-v", vel.t.data(), vel.uav.data(), vel.uav.size(), 0, 0);
//         ImPlot::PlotLine("v'-v", vel.t.data(), vel.av.data(), vel.av.size(), 0, 0);
//         ImPlot::EndPlot();
//     }

//     // static double wticks[] = {-0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4};
//     if (!vel.empty() && show_w && ImPlot::BeginPlot("w(t)")) {
//         ImPlot::SetupAxes(nullptr, nullptr, 0, 0);
//         ImPlot::SetupAxisLimits(ImAxis_X1, vel.time_now - history, vel.time_now + history, ImGuiCond_Always);
//         // ImPlot::SetupAxisLimits(ImAxis_Y1, -90.0, 90.0);
        
//         ImPlot::SetupAxisFormat(ImAxis_X1, impl_time_format, (void*)(&vel));
//         // ImPlot::SetupAxisTicks(ImAxis_Y1, wticks, 9, nullptr, false);

//         ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
//         double t0 = vel.time_now;
//         ImPlot::PlotInfLines("now", &t0, 1);

//         ImPlot::PlotLine("w", vel.t.data(), vel.w.data(), vel.w.size(), 0, 0);
//         ImPlot::PlotLine("uw", vel.ut.data(), vel.uw.data(), vel.uw.size(), 0, 0);
//         ImPlot::PlotLine("wlim", vel.tl.data(), vel.wlim.data(), vel.wlim.size(), 0, 0);
//         ImPlot::EndPlot();
//     }

//     ImGui::End();
// }

// void AppData::show_imgui_task_info() {
//     ImGui::Begin("Agent's task info");

//     auto int_formatter = [](std::string posfix, double ratio = 1000.0) {
//         return [=](const nloh::ordered_json& data)->std::string { 
//             double value = data.get<int>() / ratio;
//             return fmt::format("{:.3f}{}", value, posfix);
//         };
//     };
//     auto time_formatter = [](const nloh::ordered_json& data)->std::string {
//         int time_ms = data.get<int>();
//         int ss = time_ms / 1000;

//         int ms = time_ms - ss * 1000;
        
//         int hh = ss / 3600;
//         ss -= hh * 3600;

//         int min = ss / 60;
//         ss -= min * 60;

//         return fmt::format("{:02d}:{:02d}:{:02d} {:03d}", hh, min, ss, ms);
//     };
//     gui_util::imgui_json_table("task info table", log_data.task_info_json, {
//         { "duration",     int_formatter(" s") },
//         { "dist_moved",   int_formatter(" m") },
//         { "angle_turned", int_formatter(" deg", 1000 * 3.1415926 / 180.0) },
//         { "stop_error",   int_formatter(" m") },
//         { "time_slow_move", int_formatter(" s") },
//         { "time_sch_wait", int_formatter(" s") },
//         { "time_sch_replan_wait", int_formatter(" s") },
//         { "time_high_acc", int_formatter(" s") },
//         { "time_start", time_formatter }
//     });
//     ImGui::End();
// }

// void AppData::draw_canvas() { 
//     using CellGraphView = newsch::maptool::gui::BasicCellGraphView;
//     using CellGraphWrapper = newsch::maptool::gui::CellGraphWrapper<
//         CellGraphView
//     >;

//     canvas_ns::DrawOption option;
//     option.src_label = gui_config.label_source;
//     option.node_label = gui_config.label_node;
//     option.dual_track = gui_config.show_dual_track;

//     if (gui_config.show_background && image_data.valid())
//         show_image(canvas, image_data);
    
//     if (gui_config.show_cells && !cellgraph.empty()) {
//         CellGraphView view(&working_graph, &gui_config.cellmap_config);
//         CellGraphWrapper g(canvas, view);
//         canvas.window.draw(g);
//     }

//     if (gui_config.show_topo && current_map)
//         drawMap(canvas, *current_map, option); 

//     if (gui_config.show_sources && current_map)
//         draw_special_source(canvas, *current_map, option);
// }

// void AppData::draw_items() {
//     policy::ItemWrapper data(policy::PolicyItems(this));
//     canvas.window.draw(data);
//     return ;
// }

// void AppData::draw_robots() {
//     policy::RobotWrapper data(policy::DrawPolicy(this));
//     canvas.window.draw(data);
//     return ;
// }

// void AppData::draw_progress_bar() {
//     canvas.window.draw(progress);
// }

// bool load_background_param(CanvasImage& image_data, const std::string& config_file) {
//     std::ifstream fin(config_file);
//     try
//     {
//         YAML::Node doc = YAML::Load( fin );
//         if ( doc.IsNull() ) {
//             fmt::print("confign file {} is empty\n", config_file);
//             return false;
//         }

//         auto origin = doc["origin"];
//         double x = origin[0].as<double>();
//         double y = origin[1].as<double>();
//         image_data.bottomLeft = sf::Vector2f{(float)x, (float)y};
//         image_data.resolution = (float)doc["resolution"].as<double>();
//         return true;
//     }
//     catch ( YAML::Exception e )
//     {
//         fmt::print("config file {} is error\n", config_file);
//         return false;
//     }
// }

// bool load_background(CanvasImage& image_data, const std::string& map_folder) {
//     auto name = canvas_ns::load_pudu_background(image_data, map_folder);
//     if (name.empty())
//         fmt::print("Failed to find background image\n");
//     else
//         fmt::print("Background image loaded from {}\n", name);

//     return !name.empty();
// }


// } // namespace simu::manager

