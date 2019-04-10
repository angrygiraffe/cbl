/*================================================================
*   Copyright (C) 2019 Horizon Robotics Ltd. All rights reserved.
*   
*   File name   : track_time.h
*   Author      : longx
*   Email       : xiao.long@horizon.ai
*   Created date: 2019-04-09 19:57:22
*   Description : 
*
*===============================================================*/

#pragma once
#include <string>
#include <list>
#include <vector>
#include <chrono>
#include <ratio>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include <iostream>

namespace cbl{

const char* kTGap[] = {
  " ",
  ",",
  " ",
};

const char* kSGap[] = {
  " ",
  "--",
};

const size_t kMaxDepth = 7;
const std::string kMeasure("ms");

const std::pair<std::string, std::string> kDelimiters[] = {
  {"[","]"},
  {"[<",">]"},
  {"[#<","#>]"},
  {"[<#<",">#>]"},
  {"[#<#<","#>#>]"},
  {"[#<#<#","#>#>#]"},
}; 

void split(std::vector<std::string> &tokens, const std::string &str,
    const std::string &delimiters = " ") {
  // Skip delimiters at beginning
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

  // Find first non-delimiter
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos) {
    // Found a token, add it to the vector
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next non-delimiter
    pos = str.find_first_of(delimiters, lastPos);
  }
}


class TimeMark{
public:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using Duration = std::chrono::duration<int64_t, std::milli>;
  using FDuration = std::chrono::duration<float, std::milli>;
  TimeMark(const std::string& name):name_(name),begin_(Clock::now()){
  }
  void Begin(){
    begin_ = Clock::now();
  }
  void End(){
    end_ = Clock::now();
  }
  TimeMark& Begin(const std::string& sub){
    sub_.emplace_back(sub);
    return sub_.back();
  }

  TimeMark& Get(const std::string& name){
    std::vector<std::string> tokens;
    split(tokens, name, ".");
    TimeMark *p = this;
    for(auto& t: tokens){
      if(p == nullptr || p->sub_.rbegin() == p->sub_.rend() || p->sub_.rbegin()->name_ != t){
        throw std::runtime_error("invalid paramter");  //TODO
      }
      p = &(p->sub_.back());
    }
    return *p;
  }

  std::string Trace(){
    endComplition(Clock::now());
    return trace();
  }
  std::string Show(){
    endComplition(Clock::now());
    TimePoint end;
    return show(begin_, end);
  }
  std::string Json(){
    endComplition(Clock::now());
    TimePoint end;
    size_t depth = 0;
    size_t gapno = 0;
    return "{" + json(begin_, end, depth, gapno) + "}";
  }

  //void Serialize();
  //void Deserialization();
private:
  std::string json(const TimePoint& pre, TimePoint& end, size_t depth, size_t& gapno){
    if(depth > kMaxDepth){
      throw std::runtime_error("nest too deep"); 
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);
    if(pre != begin_){
      ss << "\"gap" << ++gapno << "\":\"" << std::chrono::duration_cast<FDuration>(begin_ - pre).count() << kMeasure << "\",";
    }
    ss << "\""<< name_ << "\":{\"cost\":\"" 
      << std::chrono::duration_cast<FDuration>(end_ - begin_).count() << kMeasure << "\"";
    if(!sub_.empty()){
      TimePoint p = begin_;
      TimePoint e;
      for(auto& t: sub_){
        ss << "," <<  t.json(p, e, depth+1, gapno);
        p = e;
      }
      if(e != end_){
        ss << ",\"gap" << ++ gapno << "\":\"" << std::chrono::duration_cast<FDuration>(end_ - e).count() << kMeasure << "\"";
      }
    }
    ss << "}";

    end = end_;
    return ss.str();
  }

  std::string show(const TimePoint& pre, TimePoint& end, size_t depth = 0){
    if(depth > kMaxDepth){
      throw std::runtime_error("nest too deep"); 
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);
    if(pre != begin_){
      ss << kSGap[1] << std::chrono::duration_cast<FDuration>(begin_ - pre).count() << kMeasure << kSGap[1];
    }
    ss << kDelimiters[depth].first << name_ << kSGap[0] 
      << std::chrono::duration_cast<FDuration>(end_ - begin_).count() << kMeasure;
    if(!sub_.empty()){
      TimePoint p = begin_;
      TimePoint e;
      for(auto& t: sub_){
        ss << t.show(p, e, depth+1);
        p = e;
      }
      if(e != end_){
        ss << kSGap[1] << std::chrono::duration_cast<FDuration>(end_ - e).count() << kMeasure << kSGap[1];
      }
    }
    ss << kDelimiters[depth].second;

    end = end_;
    return ss.str();
  }

  std::string trace(size_t depth = 0){
    if(depth > kMaxDepth){
      throw std::runtime_error("nest too deep"); 
    }
    std::stringstream ss;
    ss << kDelimiters[depth].first << name_ << kTGap[0] 
      << std::chrono::duration_cast<Duration>(begin_.time_since_epoch()).count() << kTGap[1] 
      << std::chrono::duration_cast<Duration>(end_.time_since_epoch()).count(); 
    for(auto& t: sub_){
      ss << kTGap[2] << t.trace(depth+1);
    }
    ss << kDelimiters[depth].second;
    return ss.str();
  }

  TimePoint endComplition(const TimePoint& now){
    if(end_ == TimePoint()){
      std::cout << "end complition " << name_ << std::endl;
      end_ = now;
    }
    auto pre = now;
    for(auto rit = sub_.rbegin(); rit != sub_.rend(); rit ++){
      pre = rit->endComplition(pre);
    }
    return begin_;
  }

  std::string name_;
  TimePoint begin_;
  TimePoint end_;
  std::list<TimeMark> sub_;
};

}//namespace cbl
