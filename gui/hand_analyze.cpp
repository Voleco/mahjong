// #include <chrono>
// #include <thread>
// #include <cstdio>
// #include <map>
// #include <queue>
// #include <unistd.h>
// #include <random>
// #include <fstream>
// #include <thread>
// #include <memory>
// #include <fmt/format.h>
// #include <regex>
// #include <set>
// #include "imgui-SFML.h"
// #include "hand_analyzer.hpp"

// namespace simu::logplay {

// using std::chrono::milliseconds;
// using namespace std::chrono_literals;
// using time_point = std::chrono::steady_clock::time_point;

// void build_shortnames(std::map<int, NameInfo>& index2name) {
//     auto sub_name = [&](const NameInfo& info, int len) {
//         int beg = (int)info.robot_id.size() - len;
//         beg = std::max(beg, 0);
//         return info.robot_id.substr(beg);
//     };
//     auto short_name_good = [&](int len) {
//         std::set<std::string> name;
//         for (auto& one: index2name) {
//             int L = (int)one.second.robot_id.size();
//             if (L < len) return false;
//             auto sub = sub_name(one.second, len);
//             if (name.find(sub) != name.end()) {
//                 fmt::print("Failed to use last {} chars as short name, conflict name {}\n", len, sub);
//                 return false;
//             }
//             name.insert(sub);
//         }
//         return true;
//     };
//     std::vector<int> sub_name_length = { 2, 5, 8, 11 };
//     for (int len: sub_name_length) {
//         if (short_name_good(len)) {
//             fmt::print("Using last {} chars as robots' short name\n", len);
//             for (auto& one: index2name) {
//                 one.second.short_name = sub_name(one.second, len);
//             }
//             break;
//         }
//     }
// }

// bool parse_time(const std::string& s, Time& time) {
//     if (s.size() < 12) return false;
//     if (s[2] != ':' || s[5] != ':') return false;
//     if (s[8] != ' ') return false;

//     try {
//         int hh = std::stoi(s.substr(0, 2));
//         int mm = std::stoi(s.substr(3, 2));
//         int ss = std::stoi(s.substr(6, 2));
//         int ms = std::stoi(s.substr(9, 3));
//         time.hour = hh;
//         time.min = mm;
//         time.sec = ss;
//         time.msec = ms;
//         return true;
//     }
//     catch(...) {
//         return false;
//     }
// }

// class RobotInfoParser {
// public:
//     RobotInfoParser() {
//         pat = std::regex(
//             "([0-9]{2}):([0-9]{2}):([0-9]{2})\\s([0-9]{3}).*"
//             "Scheduling: new info id:(\\w{2}:\\w{2}:\\w{2}:\\w{2}:\\w{2}:\\w{2}),\\smode:([0-9]+),\\s"
//             "pos\\(([0-9-\\.]+),([0-9-\\.]+)\\),\\s"
//             "goal\\(([0-9-\\.]+),([0-9-\\.]+)\\).*");
//     }
//     bool operator () (RobotInfo& one, const std::string& line, 
//         std::map<std::string, int>& id2index,
//         std::map<int, NameInfo>& index2name)
//     {
//         if (parse_record(one, line, id2index, index2name))
//             return true;

//         parse_default_info(line);
//         return false;
//     }

//     bool parse_default_info(const std::string& line);

//     bool parse_record(RobotInfo& one, const std::string& line, 
//         std::map<std::string, int>& id2index,
//         std::map<int, NameInfo>& index2name) const;

// private:
//     std::regex pat;
//     std::map<std::string, RobotInfoDefault> default_info;
// };

// bool RobotInfoParser::parse_record(RobotInfo& info, const std::string& line, 
//     std::map<std::string, int>& id2index,
//     std::map<int, NameInfo>& index2name) const
// {
//     if (line.find("Scheduling: new info id:") == std::string::npos) return false;

//     std::smatch match;
//     if (!std::regex_match(line, match, pat)) return false;

//     if (match.size() != 11) {
//         fmt::print("Error matching {}, group size != 11\n", line);
//         for (int i=0;i<match.size(); ++i) {
//             fmt::print("Match {}: {}\n", i, match[i].str());
//         }
//         return false;
//     }

//     int k = 5;

//     std::string robot_id = match[k++].str();
//     int sch_mode = std::stoi(match[k++].str());

//     double x0 = std::stod(match[k++].str());
//     double y0 = std::stod(match[k++].str());
//     double x1 = std::stod(match[k++].str());
//     double y1 = std::stod(match[k++].str());
    
//     int robot_index = -1;
//     if (id2index.find(robot_id) == id2index.end()) {
//         robot_index = id2index.size() + 1;
//         id2index[robot_id] = robot_index;
//         index2name[robot_index] = { robot_id, robot_id };
//     }
//     else {
//         robot_index = id2index.find(robot_id)->second;
//     }

//     info.robot_index = robot_index;
//     info.scheduling_mode = sch_mode;
//     info.priority = 0;
//     info.pose << x0, y0, 0;
//     info.goal << x1, y1, 0;

//     auto iter = default_info.find(robot_id);
//     if (iter != default_info.end()) {
//         info.priority = iter->second.default_priority;
//     }

//     return true;
// }

// bool RobotInfoParser::parse_default_info(const std::string& line) {
//     const char* s = "/newsch_data:";
//     int k = line.find(s);
//     if (k == std::string::npos) return false;

//     k += std::strlen(s);
//     if (k >= (int)line.size()) return false;
//     if (line.find("\"type\":\"priority\"", k) == std::string::npos) return false;

//     int priority = 0;
//     std::string id;
//     try {
//         nloh::json js = nloh::json::parse(std::string_view(line.data() + k, line.size() - k));
//         js.at("id").get_to(id);
//         js.at("data").get_to(priority);
//     }
//     catch (...) {
//         return false;
//     }

//     default_info[id].default_priority = priority;
//     return true;
// }

// void load_logfile(const std::string& logfile, 
//     std::vector<OneRecord>& records, 
//     std::map<std::string, int>& id2index,
//     std::map<int, NameInfo>& index2name,
//     std::vector<int>& item_nums) 
// {
//     const int buffer_size = 1024 * 16;
//     char buffer[buffer_size];
//     std::ifstream file;
//     file.rdbuf()->pubsetbuf(buffer, buffer_size);
//     file.open(logfile.c_str());

//     records.clear();
//     id2index.clear();
//     index2name.clear();
//     item_nums.clear();
//     item_nums.resize(GuiItemType::NumGuiItems, 0);

//     records.reserve(65000);
//     std::string line;

//     GenealInfoParser general_parser;
//     RobotInfoParser robot_parser;
//     GeneralInfo general_data;
//     Time temp_time;
//     RobotInfo temp_info;
//     pdlog_util::NavLoopInfoBuilder loop_info;
//     loop_info.set_handler([&](pdlog_util::NavLoopInfo info) {
//         OneRecord one;
//         one.time = Time::from_ms(info.time);
//         one.data = info;
//         records.push_back(one);
//     });

//     using clock_t = std::chrono::high_resolution_clock;
//     auto t0 = clock_t::now();
//     while (std::getline(file, line)) {
//         if (!file) break;

//         if (!line.empty() && line.back() == '\r') line.erase(line.end() - 1);
//         if (!parse_time(line, temp_time)) continue;
        
//         loop_info.parse_and_build(line);
//         if (robot_parser(temp_info, line, id2index, index2name)) {
//             OneRecord one;
//             one.time = temp_time;
//             one.data = temp_info;
//             records.emplace_back(std::move(one));
//         }
//         else if (general_parser.parse(line, general_data)) {   
//             OneRecord one;
//             one.time = temp_time;
//             one.data = general_data;
//             ++item_nums[general_data.type];
//             records.emplace_back(std::move(one));
//         }
//         else {
//             continue;
//         }

//         if (records.size() % 10000 == 0)
//             fmt::print("-- loaded {:5} records\n", records.size());
//     }
//     auto t1 = clock_t::now();

//     fmt::print("Loaded {} records, using {:2f} seconds\n", 
//         records.size(), std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count());
// }


// template<class Vec3d>
// util::gui_robot::Pose2D create_pose2d(const Vec3d& vec) {
//     util::gui_robot::Pose2D pose;
//     pose.pos.x = vec.x();
//     pose.pos.y = vec.y();
//     pose.yaw   = vec.z();
//     return pose;
// }

// void impl_update_gui_state_from_data(
//     GUIRobotData& robot, 
//     const std::string& full_name,
//     const std::string& short_name,
//     const RobotInfo& record) 
// {
//     robot.id = record.robot_index;
//     if (robot.name.size() != short_name.size()) {
//         robot.name = short_name;
//     }
//     if (robot.full_name.size() != full_name.size()) {
//         robot.full_name = full_name;
//     }
    
//     robot.pose = create_pose2d(record.pose);
//     robot.goal_pose = create_pose2d(record.goal);
//     robot.schedule_mode = record.scheduling_mode;
//     robot.priroity = record.priority;
//     robot.radius = 0.3f;
// }

// void update_gui_state_from_data(SimuData& data, int tick, const RobotInfo& info) {
//     auto& name_info = data.index2name.find(info.robot_index)->second;
//     impl_update_gui_state_from_data(
//         data.robot[info.robot_index], 
//         name_info.robot_id, name_info.short_name,
//         info);  
// }
// void update_gui_state_from_data(SimuData& data, int tick, const GeneralInfo& info) {
//     update_gui_state_from_data(data.items, info);
// }

// void VelocityData::on_info(int tick, const pdlog_util::NavLoopInfo& info) {
//     auto to_float = [](int val) { return static_cast<float>(val / 1000.0); };
//     auto to_degree = [](float value) { return static_cast<float>(value * 180 / 3.1415926); };
//     int dt = info.time - time_zero.to_int_ms();
//     t.push_back(to_float(dt));
//     v.push_back(to_float(info.v));
//     w.push_back(to_degree(to_float(info.w)));
//     ut.push_back(to_float(dt));
//     uv.push_back(to_float(info.uv));
//     uw.push_back(to_degree(to_float(info.uw)));
//     tl.push_back(to_float(dt));
//     vlim.push_back(to_float(info.limit_v));
//     wlim.push_back(to_degree(to_float(info.limit_w)));
    
//     float d_ut = 0.1f;
//     float dv = uv.back() - v.back();
//     float dw = uw.back() - w.back();
//     uav.push_back(dv/d_ut);
//     uaw.push_back(dw/d_ut);

//     if (v.size() <= 1 || w.size() <= 1) {
//         av.push_back(0.0);
//         aw.push_back(0.0);
//     }
//     else {
//         float dv = v[v.size() - 1] - v[v.size() - 2];
//         float dw = w[w.size() - 1] - w[w.size() - 2];
//         av.push_back(dv/d_ut);
//         aw.push_back(dw/d_ut);
//     }

//     tick_last = tick;
// }

// void update_gui_state_from_data(SimuData& data, int tick, const pdlog_util::NavLoopInfo& info) {
//     auto& vel = data.velocity_info;
//     if (vel.empty()) {
//         vel.clear();
//         vel.time_zero = Time::from_ms(info.time);
        
//         int tnow = info.time;
//         int tick_beg = tick - 1;
//         for (; tick_beg >= 0; --tick_beg) {
//             int t = data.records[tick_beg].time.to_int_ms();
//             if (tnow - t > vel.time_padding * 1000)
//                 break;
//         }
//         ++tick_beg;

//         for (; tick_beg < tick; ++tick_beg) {
//             const auto& one = data.records[tick_beg].data;
//             if (auto* info_it = std::get_if<pdlog_util::NavLoopInfo>(&one))
//                 vel.on_info(tick_beg, *info_it);
//         }
//     }

//     int tick_beg = vel.tick_last + 1;
//     if (vel.empty())
//         tick_beg = tick;
//     else if (tick > tick_beg > 100) {
//         int tlast = vel.time_zero.to_int_ms() + int(std::round(vel.t.back() * 1000));
//         if (info.time - tlast > vel.time_padding * 1000) {
//             vel.clear();
//             vel.time_zero = Time::from_ms(info.time);
//             tick_beg = tick;
//         }
//     }
    
//     for (; tick_beg < (int)data.records.size(); ++tick_beg) {
//         int t = data.records[tick_beg].time.to_int_ms();
//         if (t - info.time > vel.time_padding * 1000)
//             break;

//         const auto& one = data.records[tick_beg].data;
//         if (auto* info_it = std::get_if<pdlog_util::NavLoopInfo>(&one))
//             vel.on_info(tick_beg, *info_it);
//     }
//     vel.time_now = static_cast<float>((info.time - vel.time_zero.to_int_ms()) / 1000.0);
// }

// double time_of(const newsch::taskinfo::EventAny& e) {
//     using namespace newsch::taskinfo;
//     if (auto* data = std::get_if<EventState>(&e)) 
//         return data->time / 1000.0;
//     if (auto* data = std::get_if<EventFinalGoal>(&e))
//         return data->time / 1000.0;
//     if (auto* data = std::get_if<EventTask>(&e))
//         return data->time / 1000.0;
//     if (auto* data = std::get_if<EventNewPath>(&e))
//         return data->time / 1000.0;
//     if (auto* data = std::get_if<EventRun>(&e))
//         return data->time / 1000.0;
//     throw std::logic_error("invalid event any");
// }


// bool update_task_info_state(SimuData& data) {
//     if (data.task_info_events.empty()) {
//         data.tick_info_event = 0;
//         return false;
//     }
//     if (data.task_info_builder == nullptr)
//         return false;

//     int tick = data.tick_info_event;
//     tick = std::min(tick, (int)data.task_info_events.size() - 1);
//     tick = std::max(tick, 0);

//     auto& curr = data.task_info_events.at(tick);
//     double t = time_of(curr);
//     double tmin = time_of(data.task_info_events.front());
//     if (data.record_time < tmin) {
//         data.tick_info_event = 0;
//         data.task_info = {};
//         data.task_info_json.clear();
//         return false;
//     }

//     if (t <= data.record_time) {
//         for (; tick < data.task_info_events.size(); ++tick) {
//             auto& one = data.task_info_events.at(tick);
//             double tnow = time_of(one);
//             if (tnow <= data.record_time) {
//                 // its callback will update the task info
//                 data.task_info_builder->operator()(one);
//             }
//             else {
//                 break;
//             }
//         }
//         bool changed = tick - 1 != data.tick_info_event;
//         data.tick_info_event = tick - 1;
//         return changed;
//     }
//     else {
//         double thresh = data.record_time - 1;
//         if (data.task_info.time_start > 0)
//             thresh = std::max(0.0, data.task_info.time_start / 1000.0 - 1.0);
        
//         for (; tick > 0; --tick) {
//             auto& one = data.task_info_events.at(tick);
//             if (time_of(one) <= thresh)
//                 break;
//         }
//         // Need to reset task info
//         data.task_info_builder->reset();
//         data.task_info = {};
//         data.task_info_json.clear();

//         data.tick_info_event = tick;
//         return update_task_info_state(data);
//     }
// }

// bool update_tick_state(SimuData& data) {
//     if (data.records.empty()) {
//         data.curr_tick = 0;
//         return false;
//     }

//     int tick = data.curr_tick;
//     tick = std::min(tick, (int)data.records.size() - 1);
//     tick = std::max(tick, 0);
    
//     auto& curr = data.records.at(tick);
//     double t = curr.time.to_sec();
//     if (t <= data.record_time) {
//         for (; tick < data.records.size(); ++tick) {
//             auto& one = data.records.at(tick);
//             double tnow = one.time.to_sec();
//             if (tnow <= data.record_time) {
//                 std::visit([&,tick](auto&& info) {
//                     update_gui_state_from_data(data, tick, info);
//                 }, one.data);
//             }
//             else {
//                 break;
//             }
//         }
//         bool changed = tick - 1 != data.curr_tick;
//         data.curr_tick = tick - 1;
//         return changed;
//     }
//     else {
//         for (; tick > 0; --tick) {
//             auto& one = data.records.at(tick);
//             if (one.time.to_sec() <= data.record_time - 1)
//                 break;
//         }
//         data.clear_gui_data();
//         data.curr_tick = tick;
//         return update_tick_state(data);
//     }
// }



// void SimuData::load_task_info_events(const std::string& fname) {
//     auto info = pdlog_util::extract_basic_info(fname);
//     basic_info.robot_id = info.robot_id;
//     basic_info.map_flag = info.map_flag;
    
//     task_info_builder = std::unique_ptr<newsch::taskinfo::TaskInfoBuilder>(
//         newsch::taskinfo::create_default_builder()
//     );
//     task_info_builder_callback = {};
//     auto on_task = [this](const auto& one) {
//         this->task_info = one;
//         this->task_info_json = one;
//     };
//     task_info_builder_callback.func_update = on_task;
//     task_info_builder_callback.func_complete = on_task;
//     task_info_builder->setInfoHandler(&task_info_builder_callback);
//     task_info_builder->setSharedInfo(basic_info);
//     task_info_builder->reset();

//     task_info_events.clear();
//     basic_info = {};
//     task_info = {};
//     task_info_json.clear();

//     newsch::taskinfo::TaskEventHandler_Basic callback;
//     auto on_event = [this](const auto& one)->bool { 
//         this->task_info_events.push_back(one); return false;
//     };
//     callback.on_state = on_event;
//     callback.on_goal = on_event;
//     callback.on_task = on_event;
//     callback.on_path = on_event;
//     callback.on_run = on_event;

//     newsch::taskinfo::PdlogEventParser parser;
    
//     const int buffer_size = 1024 * 16;  // 16 k
//     char buffer[buffer_size];
//     std::ifstream file;
//     file.rdbuf()->pubsetbuf(buffer, buffer_size);
//     file.open(fname.c_str());

//     std::string line;
//     while (std::getline(file, line)) {
//         parser(line, callback);
//     }
//     file.close();
// }

// bool SimuData::update_state() {
//     update_task_info_state(*this);
//     return update_tick_state(*this);
// }

// void SimuData::load(const std::string& fname) {
//     records.clear();
//     index2name.clear();
//     id2index.clear();
//     robot.clear();
//     load_logfile(fname, records, id2index, index2name, item_nums_by_type);
//     load_task_info_events(fname);
//     build_shortnames(index2name);
//     clear_gui_data();
    
//     curr_tick = 0;
//     if (!records.empty()) {
//         record_time = records.front().time.to_sec();
//         while (curr_tick + 1 < records.size()) {
//             double t2 = records.at(curr_tick + 1).time.to_sec();
//             if (t2 - record_time > 2.5) {
//                 ++curr_tick;
//                 record_time = t2;
//             }
//             else {
//                 break;
//             }
//         }
//         if (curr_tick != 0) {
//             fmt::print("skipped first {} record(s)\n", curr_tick);
//         }
//     }
// }

// } // namespace simu::logplay


// std::string extract_to_temp(const std::string& map_name, const std::string& dest_folder) {
//     auto mkdir = fmt::format("mkdir -p {}", dest_folder);
//     auto clear = fmt::format("rm {}/*", dest_folder);
//     auto decompress = fmt::format("unzip {} -d {}", map_name, dest_folder);
//     if (system(mkdir.c_str())== -1) puts("Warning: mkdir failed");
//     if (system(clear.c_str()) == -1) puts("Warning: rm failed");
//     if (system(decompress.c_str()) == -1) puts("Warning: unzip failed");
//     return dest_folder;
// }

// #include "CLI11.hpp"
// #include <experimental/filesystem>
// int run_logplay_gui(SimLogPlayOption option, bool init_log);
// int main(int argc, const char* argv[]) {
//     namespace fs = std::experimental::filesystem;

//     std::string map_file;
//     std::string log_file;
//     std::string log_folder;
//     std::string map_or_log;
    
//     CLI::App parser("SimLogplayer");
//     parser.add_option("map_or_log", map_or_log, "a pdmap or a log file");
//     parser.add_option("-m,--map", map_file, ".pdmap file");
//     parser.add_option("-f,--file", log_file, ".log file");
//     parser.add_option("-d,--dir", log_folder, "folder of log files");

//     try {
//         parser.parse(argc, argv);
//     } catch (const CLI::ParseError &e) {
//         fmt::print("Error: {}\n", e.what());
//         exit(-1);
//     }
//     if (!map_or_log.empty()) {
//         if (!fs::path(map_or_log).has_extension()) {
//             fmt::print("Error: map/log file has no extension, {}\n", map_or_log);
//             exit(-1);
//         }
//         std::string ext = fs::path(map_or_log).extension().string();
//         if (ext == ".log") {
//             log_file = map_or_log;
//         }
//         if (ext == ".pdmap") {
//             map_file = map_or_log;
//         }
//     }
//     if (log_folder.empty() && !log_file.empty()) {
//         int pos = log_file.find_last_of('/');
//         if (pos == std::string::npos)
//             pos = log_file.find_last_of('\\');
//         if (pos == std::string::npos)
//             log_folder = "./";
//         else
//             log_folder = log_file.substr(0, pos);
//     }

//     std::string map_folder;
//     if (!map_file.empty()) {
//         map_folder = "temp_simlog_map";
//         extract_to_temp(map_file, map_folder);
//         fmt::print("--- Loadmap {} ---\n", fs::absolute(map_file).string());
//         fmt::print("--- extract {} ---\n", fs::absolute(map_folder).string());
//     }
//     if (!log_file.empty()) {
//         fmt::print("--- Load log {} ---\n", fs::absolute(log_file).string());
//     }
//     if (!log_folder.empty()) {
//         fmt::print("--- Use log folder {} ---\n", fs::absolute(log_folder).string());
//     }
    

//     SimLogPlayOption log_play;
//     log_play.map_folder = map_folder;
//     log_play.logfile = log_file;
//     log_play.log_folder = log_folder;
//     log_play.pause_start = true;

//     run_logplay_gui(log_play, false);

//     ImGui::SFML::Shutdown();
//     return 0;
// }