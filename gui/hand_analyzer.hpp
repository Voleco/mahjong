#pragma once

// #include <fmt/format.h>
#include <memory>
#include <variant>


// using GUIRobot = util::gui_robot::RobotOption;
// using GUIRobotData = util::gui_robot::RobotData;
// using GuiItemData = util::gui_items::GuiItemData;
// using GuiItemConfig = util::gui_items::ItemConfig;
// using GuiItemType = util::gui_items::ItemType;
// struct Time {
//     double to_sec() const {
//         return hour * 3600.0 + min * 60.0 + sec + msec / 1000.0;
//     }
//     static Time from_sec(double s) {
//         Time t;
//         t.msec = 1000 * (s - std::floor(s));
        
//         s = std::floor(s);
//         t.hour = (int)std::floor(s / 3600);

//         s -= 3600 * t.hour;
//         t.min = (int)std::floor(s / 60);

//         s -= 60 * t.min;
//         t.sec = (int)s;
//         return t;
//     }
//     static Time from_ms(int ms) {
//         Time t;
//         t.msec = ms % 1000;
//         ms /= 1000;
        
//         t.hour = ms / 3600;
//         ms -= t.hour * 3600;

//         t.min = ms / 60;
//         ms -= t.min * 60;

//         t.sec = ms;
//         return t;
//     }
//     int to_int_ms() const {
//         int s = hour * 3600 + min * 60 + sec;
//         return s * 1000 + msec;
//     }
//     bool operator < (const Time& t) const {
//         return to_int_ms() < t.to_int_ms();
//     }
//     bool operator > (const Time& t) const {
//         return to_int_ms() > t.to_int_ms();
//     }
//     bool operator == (const Time& t) const {
//         return to_int_ms() == t.to_int_ms();
//     }
//     bool operator != (const Time& t) const {
//         return !(*this == t);
//     }
//     std::string to_string(bool with_ms = true) const {
//         if (with_ms)
//             return fmt::format("{:02d}:{:02d}:{:02d} {:03d}", hour, min, sec, msec);
//         return fmt::format("{:02d}:{:02d}:{:02d}", hour, min, sec);
//     }

//     int hour;
//     int min;
//     int sec;
//     int msec;
// };

// struct RobotInfo {
//     int robot_index;
//     int scheduling_mode;
//     int priority;
//     Eigen::Vector3d pose, goal;
// };
// struct RobotInfoDefault {
//     int default_priority = -1;
// };

// using RecordData = std::variant<RobotInfo, GeneralInfo, pdlog_util::NavLoopInfo>;
// struct OneRecord {
//     Time time;
//     RecordData data;
// };


// struct SimuConfig {
//     bool paused = false;
//     double speed_ratio = 1.0;
// };
// struct NameInfo {
//     std::string robot_id;
//     std::string short_name;
// };


//     Time time_zero;
//     int time_padding = 15; // seconds
//     int tick_last = -1;
//     float time_now = 0;
//     std::vector<float> t, v, w, av, aw;
//     std::vector<float> ut, uv, uw, uav, uaw;
//     std::vector<float> tl, vlim, wlim;
// };

// struct SimuData {
//     SimuData(){}
//     ~SimuData(){}
//     void load(const std::string& fname);
//     double project_time(double t) const {
//         double tmin = records.front().time.to_sec();
//         double tmax = records.back().time.to_sec();
//         return std::max(std::min(t, tmax), tmin);
//     }
//     void move_time(double dt) {
//         record_time = project_time(record_time + dt);
//     }
//     void set_time_ratio(double r) {
//         r = std::max(std::min(r, 1.0), 0.0);
//         double tmin = records.front().time.to_sec();
//         double tmax = records.back().time.to_sec();
//         record_time = (1 - r) * tmin + r * tmax;
//     }
//     Time curr_time() const {
//         return records.at(curr_tick).time;
//     }
//     bool update_state();
//     std::string robot_id_of(int index) const {
//         auto it = index2name.find(index);
//         return it != index2name.end() ? it->second.robot_id : "unknown";
//     }
//     std::string short_name_of(int index) const {
//         auto it = index2name.find(index);
//         return it != index2name.end() ? it->second.short_name : "unknown";
//     }
//     int num_of_types(util::gui_items::ItemType type) const {
//         if (type < (int)item_nums_by_type.size()) {
//             return item_nums_by_type[type];
//         }
//         return 0;
//     }
//     void clear_gui_data() {
//         for (auto& one: robot) {
//             one.second.clear();
//         }
//         items.clear();
//         velocity_info.clear();
//     }
//     bool has_forward_path() const {
//         return !items.forward_path.empty();
//     }

// // GUI State
//     std::map<int, GUIRobotData> robot;
//     GuiItemData items;
//     VelocityData velocity_info;

// // Internal Data
//     SimuConfig config;
//     int curr_tick = 0;
//     double record_time = 0;

//     std::map<std::string, int> id2index;
//     std::map<int, NameInfo> index2name;
//     std::vector<OneRecord> records;
//     std::vector<int> item_nums_by_type;

// // Task info
//     int tick_info_event = 0;
//     newsch::taskinfo::TaskInfo task_info;
//     nloh::ordered_json task_info_json;
//     newsch::taskinfo::SharedInfo basic_info;
//     std::vector<newsch::taskinfo::EventAny> task_info_events;
//     std::unique_ptr<newsch::taskinfo::TaskInfoBuilder> task_info_builder;
//     newsch::taskinfo::TaskInfoHandler_Basic task_info_builder_callback;

//     std::string logfile;
// private:
//     void load_task_info_events(const std::string& fname);
// };

// void update_gui_state_from_data(SimuData& data, const RobotInfo& info);


// struct SimLogPlayOption {
//     bool pause_start = true;
//     int sim_wall_ms = 100;
//     int sim_algo_ms = 100;
//     int sim_simu_ms = 10;

//     std::string logfile;
//     std::string map_folder;
//     std::string log_folder;
// };
