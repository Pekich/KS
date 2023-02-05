#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <functional>
#include <stdio.h>

using namespace std;


std::map<string,string> get_parsed_data(const string& stdin_data,const string& global_delim,const string& key_value_delim){
  map<string,string> mp;
  std::string::size_type delim_pos = stdin_data.find(key_value_delim);
  int curr_pos = 0;
  while(delim_pos != std::string::npos)
  {
    string key = stdin_data.substr(curr_pos,delim_pos-curr_pos);
    std::string::size_type global_delim_pos = stdin_data.find(global_delim,delim_pos);
    string value = stdin_data.substr(delim_pos+1,global_delim_pos-delim_pos-1);
    mp.insert({key,value});

    curr_pos = global_delim_pos+1;
    delim_pos = stdin_data.find(key_value_delim,delim_pos+1);
  }

  return mp;

}



pair<string,string> get_auth_data(const string& stdin_data){
    if(stdin_data != "")
    {
      map<string,string> auth_data = get_parsed_data(stdin_data,"&","=");

      auto find_login = auth_data.find("login");
      string login ="";
      string password = "";
      if(find_login != auth_data.end())
        login = find_login->second;

        auto find_pass = auth_data.find("password");
        if(find_pass != auth_data.end())
          password = find_pass->second;



          return make_pair(login,password);
    }
      return make_pair("error","error");

}


std::map<string,string> mock_data(){
  std::map<string,string> registered_users;
  registered_users["user1"] = "qwerty";
  registered_users["user2"] = "not_qwerty";

  return registered_users;
}

bool authorize(const pair<string,string>& auth_data){
  auto users = mock_data();
  string login;
  string password;
  auto find_login = users.find(auth_data.first);
  if(find_login != users.end()){
    login = find_login->first;
    password = find_login->second;

    if(password == auth_data.second)
      return true;

  }


    return false;
}

size_t get_hash(const string& inp){
  return hash<string>{}(inp);
}

string authorize_user(const string& input_data){
  pair<string,string> auth_data = get_auth_data(input_data);
  if(auth_data.first == "error")
  return "Internal error";

  if(authorize(auth_data))
    return "";

  return "Такого пользователя не существует или вы ввели неправильный пароль";

}

string read_all_lines(const string& filename){
  string text = "";
  string line;
  ifstream db (filename);
  if (db.is_open()){
    while ( getline (db,line) ){
      text += line;
      }
      db.close();
    }
   return text;
}

string insert_pattern(const string& filename,const string& inserted_data,const string& html_indicator){
  string text = read_all_lines(filename);
  size_t pos = text.find(html_indicator);
  if(pos != std::string::npos){
    text.insert(pos+html_indicator.length(),inserted_data);
  }
  return text;
}



void view(const string& filename,const string& mes="",const string& pattern=""){
  cout << "Content-Type: text/html; charset=utf-8\n";
  cout << "\n";
  cout << insert_pattern(filename,mes,pattern);
}


int main()
{
    string length = getenv("CONTENT_LENGTH")==0? 0 : getenv("CONTENT_LENGTH");
    string stdin_data((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());


    string err = authorize_user(stdin_data);
    string output;
    if(err == ""){
      view("first_page.html");
    }
    else
     view("index.html",err,"class=\"error\">");

    return 0;
}
